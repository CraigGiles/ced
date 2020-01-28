#include "osx_main_terminal.h"

#include "ced.c"

internal void
move_cursor_to(s32 x, s32 y)
{
    char tmp[64];
    sprintf(tmp, TERM_MOVE_CURSOR_TO, y+1, x+1);
    printf("%s", tmp);
}

global_variable Terminal *terminal;
global_variable struct termios orig_termios;   /* In order to restore at exit.*/
global_variable struct termios raw_state;      /* values for editor mode */

#if 1
internal Position
get_cursor_position(int ifd, int ofd)
{
    Position result = {};

    char buf[32];
    unsigned int i = 0;
    int rows;
    int cols;

    if (write(ofd, TERM_GET_CURSOR_LOCATION, 4) != 4) return result;

    /* Read the response: ESCAPE [ rows ; cols R */
    while (i < sizeof(buf)-1) {
        if (read(ifd,buf+i,1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';

    /* Parse it. */
    if (buf[0] != ESCAPE || buf[1] != '[') return result;
    if (sscanf(buf+2,"%d;%d",&rows,&cols) != 2) return result;

    result.y = rows - 1;
    result.x = cols - 1;
    return result;
}
#endif

internal void
terminal_open(Terminal *term, int fd)
{
    if (!term->rawmode && isatty(STDIN_FILENO))
    {
        printf(TERM_TO_ALTERNATE_SCREEN); // switch to alternate screen

        tcgetattr(fd, &orig_termios);
        raw_state = orig_termios;          /* modify the original mode */

        /* input modes: no break, no CR to NL, no parity check, no strip char,
         * no start/stop output control. */
        raw_state.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

        /* output modes - disable post processing */
        raw_state.c_oflag &= ~(OPOST);

        /* control modes - set 8 bit chars */
        raw_state.c_cflag |= (CS8);

        /* local modes - choing off, canonical off, no extended functions,
         * no signal chars (^Z,^C) */
        raw_state.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

        // NOTE: this following part actually causes the input to be
        //   read in a loop rather than waiting for the character to
        //   be typed.
        #if 1
        /* control chars - set return condition: min number of bytes and timer. */
        raw_state.c_cc[VMIN] = 0;    /* Return each byte, or zero for timeout. */
        raw_state.c_cc[VTIME] = 1;   /* 100 ms timeout (unit is tens of second). */
        #endif

        /* put terminal in raw mode after flushing */
        tcsetattr(fd, TCSAFLUSH, &raw_state); 

        term->rawmode = 1;
    }
}

internal void
terminal_close(Terminal *term, s32 fd)
{
    tcsetattr(fd, TCSAFLUSH, &orig_termios);
    term->rawmode = 0;
    printf(TERM_FROM_ALTERNATE_SCREEN); // switch from alternate screen
    move_cursor_to(term->original_position.x, term->original_position.y);
}

void terminal_close_all()
{
    terminal_close(terminal, STDIN_FILENO);
}

internal void
get_terminal_dimensions(s32 infile, s32 outfile, s32 *rows, s32 *columns)
{
    struct winsize ws;

    s32 err = ioctl(1, TIOCGWINSZ, &ws);

    if (err != -1 && ws.ws_col != 0)
    {
        *rows = ws.ws_row;
        *columns = ws.ws_col;
    }
}

internal s16
get_key(s32 fd)
{
    #if 0
    s16 result = fgetc(stdin);

    switch (result)
    {
        case META:
        case CTLX:
        {
            // TODO get more than one more key-press
            result = fgetc(stdin);

            /* after teh first key press... 
               Treat control characters and lowercase the same as upper case */
            if (result >= 'a' && result <= 'z')
            {
                result = to_upper(result);
            }
            else if (result >= CTRL('A') && result <= CTRL('Z'))
            {
                result += 0x40;
            }
            else if (result == 38) {
                result = ARROW_UP;
            }
        } break;
    }

    return result;
    #endif

    // TODO: maybe move this into the editor as a 'process_key_input'
    //   because, really...  the presentation layer needs to be able
    //   to get the key, and then the raw key code should be converted
    //   to the editor specific stuff in the ced.c file.
    int nread;
    char c, seq[3];
    while ((nread = read(fd,&c,1)) == 0);
    if (nread == -1) exit(1);

    while(1)
    {
	switch(c)
	{
	    case ESCAPE:    /* escape sequence */
		/* If this is just an ESC, we'll timeout here. */
		if (read(fd,seq,1) == 0) return ESCAPE;
		if (read(fd,seq+1,1) == 0) return ESCAPE;

		/* ESC [ sequences. */
		if (seq[0] == '[') {
		    if (seq[1] >= '0' && seq[1] <= '9') {
			/* Extended escape, read additional byte. */
			if (read(fd,seq+2,1) == 0) return ESCAPE;
			if (seq[2] == '~') {
			    switch(seq[1]) {
				case '3': return DELETE_KEY;
				case '5': return PAGE_UP;
				case '6': return PAGE_DOWN;
			    }
			}
		    } else {
			switch(seq[1]) {
			    case 'A': return ARROW_UP;
			    case 'B': return ARROW_DOWN;
			    case 'C': return ARROW_RIGHT;
			    case 'D': return ARROW_LEFT;
			    case 'H': return HOME_KEY;
			    case 'F': return END_KEY;
			}
		    }
		}

		/* ESCAPE O sequences. */
		else if (seq[0] == 'O') {
		    switch(seq[1]) {
			case 'H': return HOME_KEY;
			case 'F': return END_KEY;
		    }
		}
		break;
	    default:
		return c;
	}
    }
}

internal Terminal*
create_terminal()
{
    Terminal *result = malloc(sizeof(Terminal));

    terminal_open(result, STDIN_FILENO);
    get_terminal_dimensions(STDIN_FILENO, STDOUT_FILENO, &result->max_row_count, &result->max_column_count);
    atexit(terminal_close_all);

    Editor *ed = &result->editor;

    ed->window_count = 1;

    // NOTE IMPORTANT CLEANUP NOTE: test
    ed->windows[0].cursor_position.x = 0;
    ed->windows[0].cursor_position.y = 0;
    buffer_init(&ed->windows[0].buffer, MAX_INITIAL_LINES * sizeof(char)); // TODO how big should this thing be?

    return result;
}

internal void
render(s32 window_count, Window *windows)
{
    // write(STDOUT_FILENO, display.contents, display.length);
    write(STDOUT_FILENO, TERM_CLEAR_SCREEN, 2); /* clear screen */
    write(STDOUT_FILENO, TERM_HIDE_CURSOR, 6); /* Hide cursor. */
    write(STDOUT_FILENO, TERM_MOVE_CURSOR_HOME, 3); /* Go home. */

    for (s32 window_index = 0;
	 window_index < window_count;
	 ++window_index)
    {
	Window *w = windows + window_index;
	Buffer *buffer = &w->buffer;
	Position pos = w->cursor_position;

	buffer_write(buffer, stdout);

	move_cursor_to(pos.x, pos.y);
    }

    write(STDOUT_FILENO, TERM_SHOW_CURSOR, 6); /* Show cursor. */
    write(STDOUT_FILENO, TERM_CLEAR_RIGHT, 3); // clear the rest of the line
}

#include <sys/mman.h>
#include <fcntl.h>

global_variable Terminal *term;
#include <signal.h>

internal void
sigwinch_handler()
{
    struct winsize ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ws);

    terminal->max_row_count = ws.ws_row;
    terminal->max_column_count = ws.ws_col;
}

internal void
setup_sigwatch(Terminal *terminal)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigwinch_handler;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigwinch_handler;
}

int main(s32 argc, char *argv[])
{
    // NOTE: Memory Mapping a File
    #if 0
    int fd = open("/usr/share/dict/words", O_RDONLY);
    // int fd = open("data/files/hello_sailor.txt", O_RDONLY);
    u64 pagesize = getpagesize();

    char * region = mmap((void*) (pagesize * (1 << 20)),
			 pagesize,
			 PROT_READ,
			 MAP_FILE|MAP_PRIVATE,
			 fd, 0);

    fwrite(region, 1, pagesize, stdout);
    int unmap_result = munmap(region, pagesize);
    close(fd);
    #endif

    #if 1
    terminal = create_terminal();
    terminal->original_position = get_cursor_position(STDIN_FILENO, STDOUT_FILENO);

    Editor *editor = &terminal->editor;
    editor->mode = EditorMode_Insert;

    #if 1
    setup_sigwatch(terminal);
    #endif 

    b32 running = true;
    while (running)
    {
	Window *active_window = editor->windows + editor->active_window;
	move_cursor_to(active_window->cursor_position.x, active_window->cursor_position.y);

	// -- Render
	// ----------------------------------
	
	// NOTE: status line
#if 1
	// TODO: move this to the editor not the presentation layer
	Position pos = {};
	char tmp[1024];
	sprintf(tmp, "[%s][CPos(%i, %i)][TPos(%i, %i)][Terminal(%i, %i)]",
		mode_to_string(editor->mode), 
		active_window->cursor_position.x, active_window->cursor_position.y,
		pos.x, pos.y, terminal->max_row_count, terminal->max_column_count);

	move_cursor_to(0, terminal->max_row_count-1);
	printf("%s%s", "\x1b[43m", TERM_CLEAR_RIGHT); // yello background and clear right
	printf("%s", tmp);
	printf(TERM_COLOR_RESET); // reset color
	printf(TERM_MOVE_CURSOR_HOME);
#endif

	// render state
	render(editor->window_count, editor->windows);
	 
	fflush(stdout);
	// NOTE: end rendering

	// -- Update
	// ----------------------------------

	// update state

	// -- Read Input
	// ----------------------------------
	s16 key_code = get_key(STDIN_FILENO);
	if (key_code == CTRL('Q')) running = false;
	
	switch (editor->mode)
	{
	    // TODO remove 'terminal' from these api calls
	    case EditorMode_Normal: { handle_input_normal_mode(editor, active_window, key_code); } break;
	    case EditorMode_Insert: { handle_input_insert_mode(editor, active_window, key_code); } break;
	}
	
    }
    
    terminal_close(terminal, STDIN_FILENO);

    printf("\n--------------------------------------\n -- Final Buffer Contents:\n");
    s32 window_count = editor->window_count;
    Window *windows = editor->windows;


    for (s32 window_index = 0;
	 window_index < window_count;
	 ++window_index)
    {
	Window *w = windows + window_index;
	Buffer *buffer = &w->buffer;

	buffer_write(buffer, stdout);
	printf("\n");
    }

    #endif

    return 0;
}


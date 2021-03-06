#include "osx_main_terminal.h"

#include "ced.c"

global_variable Terminal *terminal;
global_variable struct termios orig_termios;   /* In order to restore at exit.*/
global_variable struct termios raw_state;      /* values for editor mode */

internal void
move_cursor_to(s32 x, s32 y)
{
    char tmp[64];
    sprintf(tmp, TERM_MOVE_CURSOR_TO, y+1, x+1);
    printf("%s", tmp);
}

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

        /* control chars - set return condition: min number of bytes and timer. */
        raw_state.c_cc[VMIN] = 0;    /* Return each byte, or zero for timeout. */
        raw_state.c_cc[VTIME] = 1;   /* 100 ms timeout (unit is tens of second). */

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

internal void
initialize_terminal(Terminal *result, s32 argc, char* argv[])
{
    terminal_open(result, STDIN_FILENO);
    get_terminal_dimensions(STDIN_FILENO, STDOUT_FILENO, &result->max_row_count, &result->max_column_count);
    atexit(terminal_close_all);

    Editor *ed = &result->editor;
    ed->window_count = 1;

    // TODO if argc > 1 load file

    ed->windows[0].buffer.line_count = 0;

    if (argc > 1)
    {
	// buffer name is file_name.ext
	buffer_initialize_and_open(&ed->windows[0].buffer, argv[1]);
    }
    else
    {
	buffer_initialize(&ed->windows[0].buffer, "*scratch*");
    }
}

internal void
render(s32 window_count, Window *windows)
{
    write(STDOUT_FILENO, TERM_CLEAR_SCREEN, 2);        // clear screen
    write(STDOUT_FILENO, TERM_HIDE_CURSOR, 6);         // Hide cursor.
    write(STDOUT_FILENO, TERM_MOVE_CURSOR_HOME, 3);    // Go home.

    for (s32 window_index = 0;
	 window_index < window_count;
	 ++window_index)
    {
	Window *w = windows + window_index;
	Buffer *buffer = &w->buffer;

	// TODO render only the visible portion of the buffer
	s32 line_start = 0;
	s32 line_end   = terminal->max_row_count - 2; // TODO: don't overwrite status line

	for (s32 line_index = line_start;
	     line_index < line_end;
	     ++line_index)
	{
	    if (line_index < buffer->line_count)
	    {
		move_cursor_to(0, line_index);
		Line *line = buffer->lines + line_index;
		fwrite(line->text, 1, line->length, stdout);
		move_cursor_to(line->length, line_index);
		fwrite(TERM_CLEAR_RIGHT, 1, 3, stdout);
	    } else {
		move_cursor_to(0, line_index);
		fwrite(TERM_CLEAR_RIGHT, 1, 3, stdout);
	    }
	}

	move_cursor_to(buffer->cursor_index, buffer->cursor_row);
    }

    write(STDOUT_FILENO, TERM_SHOW_CURSOR, 6);         // Show cursor.
    write(STDOUT_FILENO, TERM_CLEAR_RIGHT, 3);         // clear the rest of the line
}

global_variable Terminal *term;

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

#include <errno.h>

#if 0
/* Load the specified program in the editor memory and returns 0 on success
 * or 1 on error. */
Position editor_open(Buffer *buffer, char *filename)
{
    // TODO use pread
    s32 errno;
    FILE *fp;
    Position pos = {};

    fp = fopen(filename,"r");
    if (!fp) {
        if (errno != ENOENT) {
            perror("Opening file");
            exit(1);
        }
        return pos;
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    while((linelen = getline(&line,&linecap,fp)) != -1)
    {
        if (linelen && (line[linelen-1] == '\n' || line[linelen-1] == '\r'))
	{
	    line[--linelen] = '\0';
	    #if 0
	    buffer_insert_string(buffer, line);
	    buffer_insert_string(buffer, "\r\n");
	    buffer_insert_string(buffer, TERM_CLEAR_RIGHT);
	    #endif
	}

    }

    free(line);
    fclose(fp);

    return pos;
}
#endif

internal void
handle_input(Editor *editor, Window *window, s16 key_code)
{
    Buffer *buffer = &window->buffer;

    switch (key_code)
    {
        case ESCAPE:
        {
            editor->mode = EditorMode_Normal;
        } break;

        case BACKSPACE:
        {
	    buffer_backspace(buffer);
        } break;

	case ARROW_UP:
	{
	    buffer_up(buffer, 1);
	} break;

	case ARROW_DOWN:
	{
	    buffer_down(buffer, 1);
	} break;

	case ARROW_LEFT:
	case CTRL('H'): // left
	{
	    buffer_backwards(buffer);
	} break;

	case ARROW_RIGHT:
	case CTRL('L'): // right
	{
	    buffer_forwards(buffer);
	} break;

        default:
        {
	    if (key_code == '\r')
	    {
	    	buffer_insert_newline(buffer);
	    }
	    else if (key_code == '\n')
	    {
	    	buffer_insert_newline(buffer);
	    }
	    else
	    {
		buffer_insert(buffer, key_code);
	    }
	}
    }
}

int main(s32 argc, char *argv[])
{
    Terminal t = {};
    terminal = &t;

    initialize_terminal(terminal, argc, argv);
    terminal->original_position = get_cursor_position(STDIN_FILENO, STDOUT_FILENO);
    setup_sigwatch(terminal);

    Editor *editor = &terminal->editor;
    Window *active_window = editor->windows + editor->active_window;

    b32 running = true;
    while (running)
    {
	active_window = editor->windows + editor->active_window;
	Buffer *active_buffer = &active_window->buffer;

	// -- Render
	// ----------------------------------
	
	// NOTE: status line
	// TODO: move this to the editor not the presentation layer
#if 1
	Line *line = &active_buffer->lines[active_buffer->cursor_row];
	Position pos = {};
	char tmp[1024];
	sprintf(tmp, "[-%s-][%s][lines:%i][cursor(%i:%i)][line(idx:%i,len:%i)][terminal(row:%i,col:%i)]",
		mode_to_string(editor->mode), 
		active_window->buffer.name,

		active_buffer->line_count, 
		active_buffer->cursor_index, active_buffer->cursor_row,
	        line->index, line->length,
	        terminal->max_row_count, terminal->max_column_count);

	move_cursor_to(0, terminal->max_row_count-2);
	printf("%s%s", "\x1b[43m", TERM_CLEAR_RIGHT); // yello background and clear right
	printf("%s", tmp);
	printf(TERM_COLOR_RESET); // reset color
	printf(TERM_MOVE_CURSOR_HOME);
#endif

	// render state
	render(editor->window_count, editor->windows);
	fflush(stdout);

	// NOTE: end rendering

	// ----------------------------------
	// -- Update
	// ----------------------------------

	// -- Read Input
	// ----------------------------------
	s16 key_code = get_key(STDIN_FILENO);
	if (key_code == CTRL('Q')) running = false;
	
	handle_input(editor, active_window, key_code);
    }
    
    terminal_close(terminal, STDIN_FILENO);

#if 1
    printf("\n--------------------------------------\n");
    printf(" -- Final Buffer Contents:\n");
    printf("--------------------------------------\n");

    s32 window_count = editor->window_count;
    Window *windows = editor->windows;
    Buffer *b = &windows[0].buffer;

    s32 line_start = 0;
    s32 line_end   = b->line_count;
    for (s32 line_index = line_start;
	 line_index < line_end;
	 ++line_index)
    {
	Line *line = b->lines + line_index;
	printf("%s\n", line->text);
    }
#endif

    return 0;
}


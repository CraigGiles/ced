
#include "osx_main_terminal.h"
#include "ced.c"


internal void
editor_open_file(Buffer *buffer, char *filename)
{
    FILE *fp;

    // buffer->dirty = 0;
    // free(buffer->filename);
    strncpy(buffer->file, filename, BUFFER_NAME_SIZE); // TODO: ensure we're only copying filename

    fp = fopen(filename,"r");

    // TODO: error checking
#if 0
    if (!fp)
    {

	if (errno != ENOENT)
	{
	    perror("Opening file");
	    exit(1);
	}

    }
#endif

    if (fp)
    {
	char *line = NULL;

	size_t linecap = 0;
	ssize_t linelen;

	while((linelen = getline(&line, &linecap, fp)) != -1) {
	    if (linelen && (line[linelen - 1] == '\n' || line[linelen - 1] == '\r'))
	    {
		line[--linelen] = '\0';
	    }

	    buffer_insert_line(buffer, buffer->line_count, line, linelen);
	}
	free(line);
	fclose(fp);
    }

    buffer->cursor_row = 0;
    buffer->cursor_index = 0;
}

internal void
hello_sailor(Buffer *b)
{
    buffer_initialize(b, "*scratch*");
    buffer_insert(b, 'h');
    buffer_insert(b, 'e');
    buffer_insert(b, 'l');
    buffer_insert(b, 'l');
    buffer_insert(b, 'o');
    buffer_insert_newline(b);
    buffer_insert(b, 's');
    buffer_insert(b, 'a');
    buffer_insert(b, 'i');
    buffer_insert(b, 'l');
    buffer_insert(b, 'o');
    buffer_insert(b, 'r');
    // buffer_insert_string(b, "Hello Sailor");
    buffer_backspace(b);
    buffer_backspace(b);
    buffer_insert(b, 'b');
    buffer_insert(b, 'o');
    buffer_insert(b, 'a');
    buffer_insert(b, 't');
    // buffer_insert_newline(b);
}

internal void
open_file_test(Buffer *b)
{
    buffer_initialize(b, "*scratch*");
    editor_open_file(b, "/Users/gilesc/Development/ced/test.c");
}

int main()
{
    Buffer b = {};

#if 0
    hello_sailor(&b);
#endif

#if 1
    open_file_test(&b);
#endif


    printf("Final Buffer Contents\n");

    // buffer_write(&b, stdout);
    // TODO render only the visible portion of the buffer
    s32 line_start = 0;
    s32 line_end   = b.line_count;
    for (s32 line_index = line_start;
	 line_index < line_end;
	 ++line_index)
    {
	Line *line = b.lines + line_index;
	printf("%s\n", line->text);
    }

    printf("\n");
}

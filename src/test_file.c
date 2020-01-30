
#include "osx_main_terminal.h"
#include "ced.c"

int main()
{
    Buffer b = {};
    buffer_initialize(&b, "*scratch*");
    buffer_insert(&b, 'h');
    buffer_insert(&b, 'e');
    buffer_insert(&b, 'l');
    buffer_insert(&b, 'l');
    buffer_insert(&b, 'o');
    buffer_insert(&b, ' ');
    buffer_insert(&b, 's');
    buffer_insert(&b, 'a');
    buffer_insert(&b, 'i');
    buffer_insert(&b, 'l');
    buffer_insert(&b, 'o');
    buffer_insert(&b, 'r');
    // buffer_insert_string(&b, "Hello Sailor");
    buffer_backspace(&b);
    buffer_backspace(&b);
    buffer_insert_newline(&b);
    buffer_insert(&b, 'b');
    buffer_insert(&b, 'o');
    buffer_insert(&b, 'a');
    buffer_insert(&b, 't');
    // buffer_insert_newline(&b);

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

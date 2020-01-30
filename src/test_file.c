
#include "osx_main_terminal.h"
#include "ced.c"

int main()
{
    Buffer b = {};
    buffer_initialize(&b);
    buffer_insert(&b, 'h');
    buffer_insert(&b, 'e');
    buffer_insert(&b, 'l');
    buffer_insert(&b, 'l');
    buffer_insert(&b, 'o');
    buffer_insert_newline(&b);
    buffer_insert_string(&b, "Hello Sailor");
    buffer_backspace(&b);
    buffer_backspace(&b);
    buffer_insert_newline(&b);

    printf("Final Buffer Contents\n");

    buffer_write(&b, stdout);

    printf("\n");
}

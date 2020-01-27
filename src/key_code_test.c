
#include <stdio.h>

#include "osx_main_terminal.h"

internal u8
to_upper(u8 c)
{
    u8 result = c;
    if (c >= 'a' && c <= 'z') result = c - 0x20;
    return result;
}

// TODO if i ever want to just hook into the main programs' version of
//   this then it needs to be extracted to a different file
internal s16
get_key(s32 fd)
{
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
        } break;
    }

    return result;
}

int main()
{
    b32 running = 1;
    while (running)
    {
        s16 key_code = get_key(STDIN_FILENO);
        printf("KeyCode(0x%.6X)", key_code);
    }
}

#include "ced_buffer.c"

internal u8
to_upper(u8 c)
{
    u8 result = c;
    if (c >= 'a' && c <= 'z') result = c - 0x20;
    return result;
}

internal char*
mode_to_string(EditorMode mode)
{
    switch(mode)
    {
        case EditorMode_Normal: return "normal";
        case EditorMode_Insert: return "insert";
    };
}

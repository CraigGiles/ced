internal void
handle_input_insert_mode(Editor *editor, Window *window, s16 key_code)
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
            window->cursor_position.x -= 1;
	    buffer_backspace(buffer);
        } break;

        case '\n':
        case '\r':
        {
            window->cursor_position.x = 0;
            window->cursor_position.y += 1;
	    buffer_inserts(buffer, "\r\n");
        } break;

        default:
        {
            s32 at = window->cursor_position.x;
	    buffer_insert(buffer, key_code);

            window->cursor_position.x += 1; // TODO when dealing with UTF8 this wont work
        }
    }
}

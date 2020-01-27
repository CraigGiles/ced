
internal void
handle_input_normal_mode(Editor *editor, Window *window, s16 key_code)
{

    Buffer *buffer = &window->buffer;

    #if 1
    switch (key_code)
    {
        case 'i':
        case 'I':
        {
            editor->mode = EditorMode_Insert;
        } break;

        case 'h':
        case 'H': // left
        {
	    buffer_backward(buffer);
            window->cursor_position.x -= 1;
            if (window->cursor_position.x <= 0)
                window->cursor_position.x = 0;
        } break;

        case 'j':
        case 'J': // down
        {
            window->cursor_position.y += 1;

            // TODO clamp
        } break;

        case 'k':
        case 'K': // up
        {
            window->cursor_position.y -= 1;
            if (window->cursor_position.y <= 0)
            {
                window->cursor_position.y = 0;
            }
        } break;

        case 'l':
        case 'L': // right
        {
	    // TODO: guard against moving past the end of the buffer
	    buffer_forward(buffer);
            window->cursor_position.x += 1;
            // TODO clamp
        } break;
    }
    #endif
}

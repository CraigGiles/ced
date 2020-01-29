#if 1
#define BUFFER_INITIAL_GAP 1024

// Inserting text
// -----------------------------------------------
internal void buffer_insert_newline(Buffer *b);

internal void
buffer_insert(Buffer *b, s32 c)
{
    if (c == '\n' || c == '\r')
    {
	buffer_insert_newline(b);
    }
    
    if (!b->gap)
    {
        u64 back = b->total - b->front;
        b->gap = b->total;
        b->total *= 2;
        b->text = realloc(b->text, b->total);
        memmove(b->text + b->front + b->gap, b->text + b->front, back);
    }

    b->text[b->front] = c;
    b->front++;
    b->gap--;
}

internal void
buffer_insert_string(Buffer *b, char *s)
{
    u64 len = strlen(s);

    while (b->gap < len)
    {
        b->gap = 0;
        buffer_insert(b, 0);
        b->front--;
    }

    memcpy(b->text + b->front, s, len);
    b->front += len;
    b->gap -= len;
}


internal void
buffer_insert_newline(Buffer *b)
{
    buffer_insert_string(b, "\r\n");
    // TODO add an index to newline in buffer header
}

// Removing Text
// -----------------------------------------------
internal void
buffer_delete(Buffer *b)
{
    if (b->total > b->front + b->gap)
        b->gap++;
}

internal void
buffer_backspace(Buffer *b)
{
    if (b->front) {
        b->front--;
        b->gap++;
    }
}

// Moving Around Text
// -----------------------------------------------
internal void
buffer_backward(Buffer *b)
{
    if (b->front > 0) {
        b->text[b->front + b->gap - 1] = b->text[b->front - 1];
        b->front--;
    }
}

internal void
buffer_forward(Buffer *b)
{
    size_t back = b->total - b->front + b->gap;
    if (back > 0) {
        b->text[b->front] = b->text[b->front + b->gap];
        b->front++;
    }
}

internal void
buffer_move_to_beginning(Buffer *b)
{
    while (b->front)
    {
	buffer_backward(b);
    }
}

// Initialization / Destruction
// -----------------------------------------------
internal void
buffer_initialize(Buffer *b)
{
    b->total = b->gap = BUFFER_INITIAL_GAP;
    b->front = 0;
    b->text = malloc(BUFFER_INITIAL_GAP);
}

internal void
buffer_initialize_with_text(Buffer *b, char* text)
{
    buffer_initialize(b);
    buffer_insert_string(b, text);

    // TODO rather than doing an iteration all..the..time.. lets just set the values as needed
    buffer_move_to_beginning(b);
}

internal void
buffer_release(Buffer *b)
{
    free(b->text);
    b->text = 0;
}
#endif

#if 0
internal void
buffer_move(Buffer *b, ptrdiff_t amt)
{
    size_t len;
    char *dst, *src;
    if (amt < 0) {
        len = -amt;
        if (len > b->front)
            len = b->front;
        dst = b->text + b->front + b->gap - len;
        src = b->text + b->front - len;
        b->front -= len;
    } else {
        size_t back = b->total - b->front - b->gap;
        len = amt;
        if (len > back)
            len = back;
        dst = b->text + b->front;
        src = b->text + b->front + b->gap;
        b->front += len;
    }
    memmove(dst, src, len);
}

internal void
buffer_backward(Buffer *b)
{
    if (b->front > 0) {
        b->text[b->front + b->gap - 1] = b->text[b->front - 1];
        b->front--;
    }
}

internal void
buffer_forward(Buffer *b)
{
    size_t back = b->total - b->front + b->gap;
    if (back > 0) {
        b->text[b->front] = b->text[b->front + b->gap];
        b->front++;
    }
}

internal char
buffer_char_under_cursor(Buffer *b)
{
    char result = b->text[b->front];
    return result;
}

internal void
buffer_up(Buffer *b)
{
    while (buffer_char_under_cursor(b) != '\n')
    {
	buffer_backward(b);
    }
}

internal void
buffer_down(Buffer *b)
{
    while (buffer_char_under_cursor(b) != '\n')
    {
	buffer_forward(b);
    }
}
#endif

internal void
buffer_write(Buffer *b, FILE *out)
{
    char *back_start = b->text + b->front + b->gap;
    u64 back_len = b->total - b->front - b->gap;

    fwrite(b->text, 1, b->front, out);    // render before the gap
    fwrite(back_start, 1, back_len, out); // render after the gap
}

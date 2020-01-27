internal void
buffer_init(Buffer *b, size_t init)
{
    b->total = b->gap = init;
    b->front = 0;
    b->text = malloc(init);
}

internal void
buffer_destroy(Buffer *b)
{
    free(b->text);
    b->text = 0;
}

internal void
buffer_insert(Buffer *b, int c)
{
    if (!b->gap) {
        size_t back = b->total - b->front;
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
buffer_inserts(Buffer *b, const char *s)
{
    size_t len = strlen(s);
    while (b->gap < len) {
        b->gap = 0;
        buffer_insert(b, 0);
        b->front--;
    }
    memcpy(b->text + b->front, s, len);
    b->front += len;
    b->gap -= len;
}

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

internal void
buffer_write(const Buffer *b, FILE *out)
{
    fwrite(b->text, 1, b->front, out);
    char *back_start = b->text + b->front + b->gap;
    size_t back_len = b->total - b->front - b->gap;
    fwrite(back_start, 1, back_len, out);
}

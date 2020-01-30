internal void
buffer_insert_line(Buffer *b, s32 index, char *text, s32 len)
{
    // appending a new line to the buffer
    Line *tmp = realloc(b->lines, sizeof(Line) * b->line_count + 1);
    if (!tmp)
    {
	// TODO: Could not allocate new line
    }
    else
    {
	b->lines = tmp;
	b->line_count++;
	b->lines[index].text = malloc(len + 1);
	memcpy(b->lines[index].text, text, len + 1);
	b->lines[index].index = index;
    }

    #if 0
    if (index <= b->line_count)
    {
	if (index != b->line_count)
	{
	    memmove(b->lines + index + 1,b->lines + index, sizeof(b->lines[0]) * (b->line_count - index));
	    for (s32 j = index + 1;
		 j <= b->line_count;
		 j++)
	    {
		b->lines[j].index++;
	    }
	}
	
	b->lines = realloc(b->lines, sizeof(Line) * b->line_count + 1);
	b->line_count++;

	b->lines[index].length = len;
	b->lines[index].text = malloc(len + 1);
	memcpy(b->lines[index].text, text, len + 1);
	b->lines[index].index = index;
    }
    #endif
}

internal void
buffer_initialize(Buffer *b, char *name)
{
    s32 len = strlen(name);
    len = (len < BUFFER_NAME_SIZE) ? len : BUFFER_NAME_SIZE;
    strncpy(b->name, name, len);

    buffer_insert_line(b, 0, "", 1);

    b->cursor_row = 0;
    b->cursor_index = 0;
}

internal void // TODO: should we return 'true' or 'false'
buffer_insert(Buffer *b, s32 c)
{
    Line *row = b->lines + b->cursor_row;
    s32 at = b->cursor_index;

    if (at > row->used) {
        /* Pad the string with spaces if the insert location is outside the
         * current length by more than a single character. */
        int padlen = at - row->used;

        /* In the next line +2 means: new char and null term. */
        row->text = realloc(row->text, row->used + padlen + 2);
        memset(row->text + row->used, ' ', padlen);
        row->text[row->used + padlen + 1] = '\0';
        row->used += padlen + 1;
    }
    else
    {
        /* If we are in the middle of the string just make space for 1 new
         * char plus the (already existing) null term. */
        row->text = realloc(row->text, row->used + 2);
        memmove(row->text + at + 1, row->text + at, row->used - at + 1);
        row->used++;
    }

    
    s32 index = b->cursor_index++;
    row->text[index] = c;
}

internal void
buffer_insert_newline(Buffer *b)
{
    b->cursor_row++;
    b->cursor_index = 0;
    buffer_insert_line(b, b->cursor_row, "", 0);
#if 0
    int fileline = b->cursor_row;
    int filecol = b->cursor_index;
    Line *line = (fileline >= b->line_count) ? NULL : &b->lines[fileline];

    if (!line) {
	if (fileline == b->line_count) {
	    buffer_insert_line(b, fileline, "", 0);

	    if (b->cursor_row == b->line_count - 1) {
		// b->rowoff++; // TODO useful for when rendering a portion of the row
	    } else {
		b->cursor_row++;
	    }
	    b->cursor_index = 0;
	}
	return;
    }
    /* If the cursor is over the current line size, we want to conceptually
     * think it's just over the last character. */
    if (filecol >= line->length) filecol = line->length;
    if (filecol == 0) {
	buffer_insert_line(b, fileline, "", 0);
    } else {
	/* We are in the middle of a line. Split it between two lines. */
	buffer_insert_line(b, fileline + 1, line->text + filecol, line->length - filecol);

	line = &b->lines[fileline];
	line->text[filecol] = '\0';
	line->length = filecol;
    }
#endif

#if 0
    int filerow = b->rowoff + b->cursor_row;
    int filecol = b->coloff + b->cursor_index;
    erow *row = (filerow >= b->numrows) ? NULL : &b->row[filerow];

    if (!row) {
	if (filerow == b->numrows) {
	    editorInsertRow(filerow, "", 0);
	    goto fixcursor;
	}
	return;
    }
    /* If the cursor is over the current line size, we want to conceptually
     * think it's just over the last character. */
    if (filecol >= row->size) filecol = row->size;
    if (filecol == 0) {
	editorInsertRow(filerow, "", 0);
    } else {
	/* We are in the middle of a line. Split it between two rows. */
	editorInsertRow(filerow + 1, row->chars + filecol, row->size-filecol);
	row = &b->row[filerow];
	row->chars[filecol] = '\0';
	row->size = filecol;
	editorUpdateRow(row);
    }
#endif
}

internal void
buffer_backspace(Buffer *b)
{
    Line *row = b->lines + b->cursor_row;
    s32 at = b->cursor_index - 1;

    if (at < 0)
    {
	// TODO: move the row to the previous row and delete this one
    }

    // if the cursor is placed beyond the actual end of line
    if (row->used <= at) return;

    memmove(row->text + at, row->text + at + 1, row->used - at);
    row->used--;
    b->cursor_index -= 1;
}

#if 0
#define LINE_DEFAULT_INITIAL_SIZE 64

internal Line*
make_line(u64 size)
{
    Line *result = malloc(sizeof(Line));
    result->next = 0;
    result->prev = 0;
    result->size_used = 0;
    result->size_allocated = size;
    result->text = malloc(size);
    return result;
}

// Inserting text
// -----------------------------------------------
internal void
buffer_insert_string(Buffer *b, char *s)
{
    // TODO make more efficient
    s32 len = strlen(s);
    for (s32 index = 0;
	 index < len;
	 ++index)
    {
	buffer_insert(b, s[index]);
    }
}


internal void
buffer_insert_newline(Buffer *b)
{
    buffer_insert(b, '\n');
    Line *newline = make_line(LINE_DEFAULT_INITIAL_SIZE);

    Line *old_newline = b->line_cursor->next;
    b->line_cursor->next = newline;

    newline->next = old_newline;
    newline->prev = b->line_cursor;

    if (old_newline)
    {
	old_newline->prev = newline;
    }
     
    b->line_cursor = newline;
    b->cursor_position.x = 0;
    b->cursor_position.y += 1;
}

// Removing Text
// -----------------------------------------------
internal void
buffer_delete(Buffer *b)
{
}

internal void
buffer_backspace(Buffer *b)
{
    Line *row = b->line_cursor;
    s32 at = b->cursor_position.x - 1;

    if (at < 0)
    {
	// TODO: move the row to the previous row and delete this one
    }

    // if the cursor is placed beyond the actual end of line
    if (row->size_used <= at) return;

    memmove(row->text + at, row->text + at + 1, row->size_used - at);
    row->size_used--;
    b->cursor_position.x -= 1;
}

// Moving Around Text
// -----------------------------------------------
internal void
buffer_backward(Buffer *b)
{
}

internal void
buffer_forward(Buffer *b)
{
}

internal void
buffer_move_to_beginning(Buffer *b)
{
}

// Initialization / Destruction
// -----------------------------------------------

internal void
buffer_initialize(Buffer *b)
{
    Line *header = make_line(LINE_DEFAULT_INITIAL_SIZE);
    b->line_header = header;
    b->line_cursor = header;
}

internal void
buffer_initialize_with_text(Buffer *b, char* text)
{
}

internal void
buffer_release(Buffer *b)
{
}

internal void
buffer_write(Buffer *b, FILE *out)
{
    Line *current_line = b->line_header;

    while (current_line)
    {
	// TODO while current_line != 0;
	u64 len = current_line->size_used;
	fwrite(current_line->text, 1, len, out);
	current_line = current_line->next;

	// char *back_start = b->text + b->front + b->gap;
	// u64 back_len = b->total - b->front - b->gap;

	// fwrite(b->text, 1, b->front, out);    // render before the gap
	// fwrite(back_start, 1, back_len, out); // render after the gap
    }
}
#endif

#if 0
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

internal void
buffer_write(Buffer *b, FILE *out)
{
    char *back_start = b->text + b->front + b->gap;
    u64 back_len = b->total - b->front - b->gap;

    fwrite(b->text, 1, b->front, out);    // render before the gap
    fwrite(back_start, 1, back_len, out); // render after the gap
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


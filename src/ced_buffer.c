
internal Line*
buffer_insert_line(Buffer *b, s32 index, char *text, s32 len)
{
    Line *result = 0;
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

	result = &b->lines[index];
	result->length = len;
	result->text = malloc(len + 1);
	result->index = index;
	memcpy(result->text, text, len + 1);
	b->lines[index].index;
    }

    return result;
}

internal void
buffer_initialize(Buffer *b, char *name)
{
    s32 len = strlen(name);
    len = (len < BUFFER_NAME_SIZE) ? len : BUFFER_NAME_SIZE;
    strncpy(b->name, name, len);

    Line *l = buffer_insert_line(b, 0, "", 1);
    l->text[0] = '\0';

    b->cursor_row = 0;
    b->cursor_index = 0;
}

internal void
buffer_initialize_and_open(Buffer *b, char *name)
{
    s32 len = strlen(name);
    len = (len < BUFFER_NAME_SIZE) ? len : BUFFER_NAME_SIZE;
    strncpy(b->name, name, len);

    FILE *fp;

    // buffer->dirty = 0;
    // free(buffer->filename);
    strncpy(b->file, name, BUFFER_NAME_SIZE); // TODO: ensure we're only copying filename

    fp = fopen(name,"r");

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

	while((linelen = getline(&line, &linecap, fp)) != -1)
	{
	    if (linelen && (line[linelen - 1] == '\n' ||
			    line[linelen - 1] == '\r'))
	    {
		line[--linelen] = '\0';
	    }

	    buffer_insert_line(b, b->line_count, line, linelen);
	}
	free(line);
	fclose(fp);
    }

    b->cursor_row = 0;
    b->cursor_index = 0;
}

internal void // TODO: should we return 'true' or 'false'
buffer_insert(Buffer *b, s32 c)
{
    Line *row = b->lines + b->cursor_row;
    s32 at = b->cursor_index;

    if (at > row->length) {
        /* Pad the string with spaces if the insert location is outside the
         * current length by more than a single character. */
        int padlen = at - row->length;

        /* In the next line +2 means: new char and null term. */
        row->text = realloc(row->text, row->length + padlen + 2);
        memset(row->text + row->length, ' ', padlen);
        row->text[row->length + padlen + 1] = '\0';
        row->length += padlen + 1;
    }
    else
    {
        /* If we are in the middle of the string just make space for 1 new
         * char plus the (already existing) null term. */
        row->text = realloc(row->text, row->length + 2);
        memmove(row->text + at + 1, row->text + at, row->length - at + 1);
        row->length++;
    }
    
    s32 index = b->cursor_index++;
    row->text[index] = c;
}

internal void
buffer_insert_newline(Buffer *b)
{
    s32 current_row = b->cursor_row;
    s32 current_col = b->cursor_index;
    Line *line = (current_row >= b->line_count) ? 0 : &b->lines[current_row];

    if (!line) // cursor is past the end of editable text
    {
	// TODO cursor is past the end of editable text.. allow it?
	return;
    }

    if (line && current_col < line->length)
    {
	buffer_insert_line(b, b->cursor_row + 1, line->text + b->cursor_index, line->length - b->cursor_index + 1);

	line->text[current_col] = '\0';
	line->length = b->cursor_index;
	b->cursor_index = line->length - b->cursor_index;
	b->cursor_row++;
    }
    else
    {
	Line *l = buffer_insert_line(b, b->cursor_row + 1, "", 1);
	l->text[0] = '\0';
	b->cursor_row++;
	b->cursor_index = 0;
    }
}

internal void
buffer_insert_string(Buffer *b, Line *row, char* text, s32 len)
{
    row->text = realloc(row->text,row->length+len+1);
    memcpy(row->text + row->length, text, len);
    row->length += len;
    row->text[row->length] = '\0';
}

internal void
buffer_delete_row(Buffer *b, s32 index)
{
    Line *row;

    if (index >= b->line_count) return;
    row = b->lines + index;
    // editor_free_row(row); // TODO
    memmove(b->lines + index, b->lines + index + 1,sizeof(b->lines[0]) * (b->line_count - index - 1));
    for (int j = index; j < b->line_count - 1; j++) b->lines[j].index++;
    b->line_count--;
}

internal void
buffer_backspace(Buffer *b)
{
    Line *row = b->lines + b->cursor_row;
    s32 row_index = b->cursor_row;
    s32 at = b->cursor_index;

    if (at > 0)
    {
	s32 delete_index = at - 1;
	memmove(row->text + delete_index, row->text + delete_index + 1, row->length - delete_index);
	row->length--;
	b->cursor_index -= 1;
    }
    else if (at == 0 && row_index > 0)
    {
        b->cursor_index = b->lines[row_index - 1].length;
        buffer_insert_string(b, &b->lines[row_index - 1], row->text, row->length);
        buffer_delete_row(b, row_index);
        row = 0;
	if (b->cursor_row > 0) b->cursor_row--;
    }

}

// Moving Around Text
// -----------------------------------------------
internal void
buffer_backwards(Buffer *b)
{
    Line *line = b->lines + b->cursor_row;
    s32 at = b->cursor_index;

    if (at > 0)
    {
	b->cursor_index -= 1;
    }
}

internal void
buffer_forwards(Buffer *b)
{
    Line *line = b->lines + b->cursor_row;
    s32 at = b->cursor_index;

    if (at < line->length - 2) // can we move to the position before the EOL
    {
	b->cursor_index += 1;
    }
}

internal void
buffer_up(Buffer *b, s32 count)
{
    if (b->cursor_row > count)
    {
	b->cursor_row -= count;
    }
    else
    {
	b->cursor_row = 0;
    }

    Line *line = b->lines + b->cursor_row;
    if (b->cursor_index > line->length)
    {
	b->cursor_index = line->length;
    }
}

internal void
buffer_down(Buffer *b, s32 count)
{
    if (b->cursor_row + count > b->line_count-1)
    {
	b->cursor_row = b->line_count - 1;
    }
    else
    {
	b->cursor_row += count;
    }

    Line *line = b->lines + b->cursor_row;
    if (b->cursor_index > line->length)
    {
	b->cursor_index = line->length;
    }
}

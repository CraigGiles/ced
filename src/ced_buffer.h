#if !defined(CED_BUFFER_H)

#define CED_BUFFER_H

typedef struct Line {
    struct Line *next;
    struct Line *prev;
    s32 size_allocated;
    s32 size_used;
    char* text;
} Line;

#define MAX_BUFFER_NAME 64
typedef struct {
    char filename[MAX_BUFFER_NAME]; // File being edited
    Line *line_cursor;              // Link to the line which the cursor rests on
    Line *line_header;              // Link to the header line for this buffer
    Position cursor_position;
} Buffer;

#if 0
// NOTE: this is here mainly for documentation on what i had done before
typedef struct {
    char *filename;
    char *text;
    u64 total;                          /* total size of buffer */
    u64 front;                          /* size of content before cursor */
    u64 gap;                            /* size of the gap */
} Buffer;
#endif


#endif

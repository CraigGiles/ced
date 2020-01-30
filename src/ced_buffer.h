#if !defined(CED_BUFFER_H)

#define CED_BUFFER_H

#define BUFFER_NAME_SIZE 80

typedef struct {
    s32 index;
    s16 size;
    s16 used;
    char *text;
} Line;
    
typedef struct {
    s32 line_count;
    s32 cursor_index;
    s32 cursor_row;
    Line *lines;
    char name[BUFFER_NAME_SIZE];
    char file[BUFFER_NAME_SIZE];
} Buffer;

#endif

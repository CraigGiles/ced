#if !defined(CED_BUFFER_H)

#define CED_BUFFER_H

#if 1
typedef struct {
    char *filename;
    char *text;
    u64 total;                          /* total size of buffer */
    u64 front;                          /* size of content before cursor */
    u64 gap;                            /* size of the gap */
} Buffer;
#endif


#endif

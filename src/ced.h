#if !defined(CED_H)
#define CED_H


#define USE_GILESC_TYPES 1
#include "gilesc_types.h"

#define USE_GILESC_MEMORY 1
#include "gilesc_memory.h"

#define ArrayCount(array)  (sizeof(array) / sizeof(array[0]))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stddef.h>

#include "ced_buffer.h"

#define MAX_WINDOW_COUNT 16

#define MAX_INITIAL_LINES 10000
#define MAX_INITIAL_CHARS 256

typedef enum {
    EditorMode_Normal,
    EditorMode_Insert,
} EditorMode;

typedef struct {
    s32 x;
    s32 y;
} Position;

typedef struct {
    u8 flags;
    Buffer buffer;
    Position cursor_position;
} Window;

typedef struct {
    EditorMode mode;

    s32 window_count;
    s32 active_window;
    Window windows[MAX_WINDOW_COUNT];
} Editor;


#endif

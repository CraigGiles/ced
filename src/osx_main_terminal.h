#if !defined(MAIN_H)
#define MAIN_H

#include "ced.h"

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define TERM_TO_ALTERNATE_SCREEN   "\x1b[?1049h"
#define TERM_FROM_ALTERNATE_SCREEN "\x1b[?1049l"
#define TERM_CLEAR_SCREEN          "\x1b[2J"
#define TERM_HIDE_CURSOR           "\x1b[?25l"
#define TERM_SHOW_CURSOR           "\x1b[?25h"
#define TERM_MOVE_CURSOR_UP        "\x1b[%dA"
#define TERM_MOVE_CURSOR_RIGHT     "\x1b[%dC"
#define TERM_MOVE_CURSOR_DOWN      "\x1b[%dB"
#define TERM_MOVE_CURSOR_LEFT      "\x1b[%dD"
#define TERM_MOVE_CURSOR_TO        "\x1b[%d;%dH"
#define TERM_MOVE_CURSOR_HOME      "\x1b[H"
#define TERM_RESET_COLOR           "\x1b[0m"
#define TERM_GET_CURSOR_LOCATION   "\x1b[6n"
#define TERM_CLEAR_RIGHT           "\x1b[K"
#define TERM_COLOR_RESET           "\x1b[0m"

#if 0
#define BUFFER_NAME_LENGTH 80
#define ROW_COUNT          24
#define COLUMN_COUNT       80
#endif

#if 0
#define true 1
#define false 0
#endif

typedef struct {
    Editor editor;

    s32 max_row_count;
    s32 max_column_count;

    Position original_position;
    b32 rawmode;                        /* Is terminal raw mode enabled? */
} Terminal;

#endif

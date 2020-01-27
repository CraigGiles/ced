#if !defined(MAIN_H)
#define MAIN_H

#include "ced.h"

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

// TODO: move this to ced.h
#define ARROW_UP      0x4800      /* Up arrow key                 */
#define ARROW_DOWN    0x5000      /* Down arrow key               */
#define ARROW_RIGHT   0x4D00      /* Right arrow key              */
#define ARROW_LEFT    0x4B00      /* Left arrow key               */
#define F1KEY         0x3B00
#define F2KEY         0x3C00
#define F3KEY         0x3D00
#define F4KEY         0x3E00
#define F5KEY         0x3F00
#define F6KEY         0x4000
#define F7KEY         0x4100
#define F8KEY         0x4200
#define F9KEY         0x4300
#define F10KEY        0x4400
#define F12KEY        0x4600
#define AltF1KEY      0x6800
#define AltF2KEY      0x6900
#define AltF3KEY      0x6A00
#define AltF4KEY      0x6B00
#define AltF5KEY      0x6C00
#define AltF6KEY      0x6D00
#define AltF7KEY      0x6E00
#define AltF8KEY      0x6F00
#define AltF9KEY      0x7000
#define AltF10KEY     0x7100
#define ALTB          0x3000
#define ALTC          0x2E00
#define ALTD          0x2000
#define ALTE          0x1200
#define ALTF          0x2100
#define ALTH          0x2300
#define ALTM          0x3200
#define ALTX          0x2D00
#define ALTZ          0x2C00
#define InsKEY        0x5200
#define CtrlHome      0x7700
#define CtrlEnd       0x7500
#define DELETE_KEY    0x5300
#define CtrlRTKEY     0x7400
#define CtrlLFKEY     0x7300
#define HOME_KEY      0x4700
#define END_KEY       0x4F00
#define PAGE_UP       0x4900
#define PAGE_DOWN     0x5100

#define MENU_BUTTON     ALTH

#undef  CTRL
#define CTRL(c) ((c)&0x1F)              /* Control flag, or'ed in       */

#define CVMVAS  1                       /* C-V, M-V arg. in screens.    */

#define NFILEN  80                      /* # of bytes, file name        */
#define NBUFN   80 /*16*/               /* # of bytes, buffer name      */
#define NLINE   256                     /* # of bytes, line             */
#define NKBDM   256                     /* # of strokes, keyboard macro */
#define NPAT    180                     /* # of bytes, pattern          */
#define HUGE    1000                    /* Huge number                  */

#define AGRAVE  0x60                    /* M- prefix,   Grave (LK201)   */
#define METACH  0x1B                    /* M- prefix,   Control-[, ESC  */
#define CTMECH  0x1C                    /* C-M- prefix, Control-\       */
#define EXITCH  0x1D                    /* Exit level,  Control-]       */
#define CTRLCH  0x1E                    /* C- prefix,   Control-^       */
#define HELPCH  0x1F                    /* Help key,    Control-_       */

#define META    METACH                  /* Meta flag, or'ed in          */
#define CTLX    CTRL('X')               /* ^X flag, or'ed in            */
#define ESCAPE  METACH

#define BACKSPACE 127

typedef struct {
    Editor editor;

    s32 max_row_count;
    s32 max_column_count;

    Position original_position;
    b32 rawmode;                        /* Is terminal raw mode enabled? */
} Terminal;
#endif

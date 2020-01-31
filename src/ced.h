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
#include <stddef.h>

typedef struct {
    s32 x;
    s32 y;
} Position;

#include "ced_buffer.h"

#define MAX_WINDOW_COUNT 16

#define MAX_INITIAL_LINES 10000
#define MAX_INITIAL_CHARS 256

typedef enum {
    EditorMode_Normal,
    EditorMode_Insert,
} EditorMode;

typedef struct {
    u8 flags;
    Buffer buffer;
} Window;

typedef struct {
    EditorMode mode;

    s32 window_count;
    s32 active_window;
    Window windows[MAX_WINDOW_COUNT];
} Editor;

// NOTE: a lot of these came from the micro emacs editor from Walter
//   Bright. I haven't hooked into the functionality for the majority
//   of these key codes
#define ARROW_UP       0x4800      /* Up arrow key                 */
#define ARROW_DOWN     0x5000      /* Down arrow key               */
#define ARROW_RIGHT    0x4D00      /* Right arrow key              */
#define ARROW_LEFT     0x4B00      /* Left arrow key               */

#define F1_KEY         0x3B00
#define F2_KEY         0x3C00
#define F3_KEY         0x3D00
#define F4_KEY         0x3E00
#define F5_KEY         0x3F00
#define F6_KEY         0x4000
#define F7_KEY         0x4100
#define F8_KEY         0x4200
#define F9_KEY         0x4300
#define F10_KEY        0x4400
#define F12_KEY        0x4600
#define ALT_F1_KEY     0x6800
#define ALT_F2_KEY     0x6900
#define ALT_F3_KEY     0x6A00
#define ALT_F4_KEY     0x6B00
#define ALT_F5_KEY     0x6C00
#define ALT_F6_KEY     0x6D00
#define ALT_F7_KEY     0x6E00
#define ALT_F8_KEY     0x6F00
#define ALT_F9_KEY     0x7000
#define ALT_F10_KEY    0x7100
#define ALT_B          0x3000
#define ALT_C          0x2E00
#define ALT_D          0x2000
#define ALT_E          0x1200
#define ALT_F          0x2100
#define ALT_H          0x2300
#define ALT_M          0x3200
#define ALT_X          0x2D00
#define ALT_Z          0x2C00
#define INSERT         0x5200
#define CTRL_HOME      0x7700
#define CTRL_END       0x7500
#define DELETE_KEY     0x5300
#define CTRL_RIGHT_KEY 0x7400
#define CTRL_LEFT_KEY  0x7300
#define HOME_KEY       0x4700
#define END_KEY        0x4F00
#define PAGE_UP        0x4900
#define PAGE_DOWN      0x5100

#undef  CTRL
#define CTRL(c) ((c)&0x1F)              /* Control flag, or'ed in       */

#if 0
#define CVMVAS  1                       /* C-V, M-V arg. in screens.    */

#define NFILEN  80                      /* # of bytes, file name        */
#define NBUFN   80 /*16*/               /* # of bytes, buffer name      */
#define NLINE   256                     /* # of bytes, line             */
#define NKBDM   256                     /* # of strokes, keyboard macro */
#define NPAT    180                     /* # of bytes, pattern          */
#define HUGE    1000                    /* Huge number                  */
#endif

#define AGRAVE    0x60                  /* M- prefix,   Grave (LK201)   */
#define ESCAPE    0x1B                  /* M- prefix,   Control-[, ESC  */
#define CTMECH    0x1C                  /* C-M- prefix, Control-\       */
#define EXITCH    0x1D                  /* Exit level,  Control-]       */
#define CTRLCH    0x1E                  /* C- prefix,   Control-^       */
#define HELPCH    0x1F                  /* Help key,    Control-_       */
#define BACKSPACE 0x7F

#define META    ESCAPE                  /* Meta flag, or'ed in          */
#define CTLX    CTRL('X')               /* ^X flag, or'ed in            */

#endif

#ifndef DOTR_EE_CAMP_HELP_H
#define DOTR_EE_CAMP_HELP_H

/* Taku_Camp_Help_Window (0x23bce0): campaign help window, drawn with Taku_Window_Set,
 * SzDuel_DrawWindow_Font_Pipe and Taku_Bunsyou_Set.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#ifdef __MWERKS__
typedef int DotrHelpWord;
#else
#include <stdint.h>
typedef intptr_t DotrHelpWord;
#endif

/* Four 10-byte window records at 0x29b020, copied whole to the stack (two 16-byte
 * loads and a doubleword); the same record layout as camp_tikei's DotrTikeiWindow. */
struct DotrHelpWindow {
    short x, y, w, h;
    char unknown_08[2];
};
struct DotrHelpWindows {
    struct DotrHelpWindow w[4];
};
extern struct DotrHelpWindows data_0029b020;

/* Byte 0 of the camp state at 0x299e10 (camp_tikei.h declares the same view). */
extern signed char data_00299e10[];
extern char data_0030f113[];

/* The camp work block at 0x1ed0000 is addressed with literal addresses; the selected
 * line is read in the loop through a base pointer kept in a register. The host build
 * points the same names at an object the test defines. */
#ifdef __MWERKS__
#define DOTR_HELP_BLOCK ((signed char *)0x1ed0000)
#define DOTR_HELP_OPEN (*(signed char *)0x1ed543c)
#define DOTR_HELP_STEP (*(signed char *)0x1ed543d)
#define DOTR_HELP_PICK (*(signed char *)0x1ed546b)
#else
extern signed char dotr_help_block_01ed0000[0x5470];
#define DOTR_HELP_BLOCK dotr_help_block_01ed0000
#define DOTR_HELP_OPEN dotr_help_block_01ed0000[0x543c]
#define DOTR_HELP_STEP dotr_help_block_01ed0000[0x543d]
#define DOTR_HELP_PICK dotr_help_block_01ed0000[0x546b]
#endif

/* Callees, assumed prototypes from this call site. Taku_Window_Set (0x2195e0, taku_window)
 * takes a ninth argument on the stack; its sixth is an unsigned short here (the sum is kept
 * unmasked and masked at each call), where camp_tikei.h and camp_dare.h have int.
 * Taku_Bunsyou_Set (0x21c450, side_table) is declared in draw_state.h with seven integer
 * parameters; this call site also loads 1.0f into f12 and f13. The tests stub all three. */
extern DotrHelpWord Taku_Window_Set(DotrHelpWord cursor, struct DotrHelpWindow *window,
                                    unsigned short flag, int a3, unsigned short x,
                                    unsigned short y, int a6, int a7, int a8);
extern DotrHelpWord SzDuel_DrawWindow_Font_Pipe(DotrHelpWord cursor, int x, int y, int a,
                                                int flags, int n, const char *text, int c);
extern DotrHelpWord Taku_Bunsyou_Set(DotrHelpWord cursor, int index, int no, int x, int y,
                                     int unused, unsigned char *col, float sx, float sy);

DotrHelpWord Taku_Camp_Help_Window(DotrHelpWord cursor);

#endif

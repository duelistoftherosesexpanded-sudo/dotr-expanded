#ifndef DOTR_EE_CAMP_SINEMA_CUT_H
#define DOTR_EE_CAMP_SINEMA_CUT_H

/* Functions that draw the two cinema bars of the campaign map from the cut counters in the work
 * area at 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#include "dotr/ee/camp_tikei.h"

/* View of the work area at the literal address 0x1ed0000 (lui/ori constants, not a relocated
 * symbol) with the fields 0x234040 uses. Assumed: every name and meaning. The three cut
 * counters are scalars at +30, +32 and +34. `hold` at +108 is the halfword camp_tikei.h calls
 * DOTR_TIKEI_SCROLL. */
struct DotrSinemaWork {
    short unknown_00[13];
    short mode;                 /* 0x1ed001a: 0-3 with fade == 0; 0 / not 0 with fade != 0 */
    short state;                /* 0x1ed001c: 1 = stepping; 2 = finished (fade == 0);
                                   0 = finished (fade != 0) */
    short cut0;                 /* 0x1ed001e */
    short cut1;                 /* 0x1ed0020 */
    short cut2;                 /* 0x1ed0022 */
    char unknown_24[72];
    short hold;                 /* 0x1ed006c */
};
/* The scalars are literal addresses in the target while the drawing loops read the counters
 * through one pointer to the block; the host build maps all of them onto one object the test
 * defines. */
#ifdef __MWERKS__
#define DOTR_SINEMA_BLOCK ((struct DotrSinemaWork *)0x1ed0000)
#define DOTR_SINEMA_MODE (*(short *)0x1ed001a)
#define DOTR_SINEMA_STATE (*(short *)0x1ed001c)
#define DOTR_SINEMA_CUT0 (*(short *)0x1ed001e)
#define DOTR_SINEMA_CUT1 (*(short *)0x1ed0020)
#define DOTR_SINEMA_CUT2 (*(short *)0x1ed0022)
#define DOTR_SINEMA_HOLD (*(short *)0x1ed006c)
#else
extern struct DotrSinemaWork dotr_camp_sinema_cut_work;
#define DOTR_SINEMA_BLOCK (&dotr_camp_sinema_cut_work)
#define DOTR_SINEMA_MODE (dotr_camp_sinema_cut_work.mode)
#define DOTR_SINEMA_STATE (dotr_camp_sinema_cut_work.state)
#define DOTR_SINEMA_CUT0 (dotr_camp_sinema_cut_work.cut0)
#define DOTR_SINEMA_CUT1 (dotr_camp_sinema_cut_work.cut1)
#define DOTR_SINEMA_CUT2 (dotr_camp_sinema_cut_work.cut2)
#define DOTR_SINEMA_HOLD (dotr_camp_sinema_cut_work.hold)
#endif

/* The 38-byte drawing template of camp_tikei.h (struct DotrTikeiItem) as this function writes
 * it: eight halfword coordinates, sixteen colour bytes, a byte at +32. Assumed: the member
 * names; that w[1], w[3], w[5], w[7] are the y coordinates of the four corners and every
 * fourth colour byte is an alpha; `z` and `code` are not touched here. 0x29aec0 is one such
 * template copied to the stack at entry; tex_sinema_brack (name from the Japanese build) is
 * assumed to hold two, the top and the bottom bar. */
struct DotrSinemaQuad {
    short w[8];
    unsigned char col[16];
    signed char flag;
    char unknown_21;
    short z;
    short code;
};
extern struct DotrSinemaQuad data_0029aec0;
extern struct DotrSinemaQuad tex_sinema_brack[2];

/* MATCHING DEVICE: the cut heights are written scaled (`* 64 / 64`, `* 116 / 116`). The scaling
 * folds away but changes the order of the loads. */
#define DOTR_SINEMA_C0 (b->cut0 * 64 / 64)
#define DOTR_SINEMA_C1 (b->cut1 * 64 / 64)
#define DOTR_SINEMA_C2 (b->cut2 * 116 / 116)

/* 0x234040 Taku_Camp_SINEMA_CUT (name from the Japanese build's symbol table). Draws the two
 * bars through Taku_G4_Set(cursor, quad, 0, 0) and returns the cursor after both.
 * fade == 0: when state is 1 and hold is 0, steps the counters by 8 according to mode
 * (0: all three down to 0; 1: cut0 up to 64, the others down; 2: cut0 up to 64, then cut1 up
 * to 64, cut2 down; 3: cut0, then cut1 up to 64, then cut2 up to 116) and sets state 2 once
 * three steps report finished. Bar 0 gets w[5] = w[7] = its w[1] + cut0 + cut1 + cut2, bar 1
 * gets w[1] = w[3] = its w[1] - cut0 - cut1 - cut2; colours 0 with every fourth byte 128,
 * flag 0.
 * fade != 0: when state is 1, mode 0 steps cut0 by 32 up to 255 (then mode = 1, state = 0),
 * any other mode steps it down to 0 (then mode = 0, state = 0); each bar gets the template's
 * eight coordinates on top of the copy of 0x29aec0, with bytes 3, 7, 11, 15 = cut0.
 * Assumed: two parameters with these types and the cursor result. */
DotrTikeiWord Taku_Camp_SINEMA_CUT(DotrTikeiWord cursor, unsigned short fade);

#endif

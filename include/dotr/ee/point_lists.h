#ifndef DOTR_EE_POINT_LISTS_H
#define DOTR_EE_POINT_LISTS_H

#include "dotr/ee/spline_paths.h"

/* Routine 0x22c450, which converts the 2 x 16 halfword point lists at 0x299020 to floats and
 * hands each to 0x21bc10 with output arrays inside the block at the literal 0x1ed0000.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* One 76-byte list: up to sixteen (x, y) halfword pairs, their count at +64 and
 * a halfword at +66 that is passed on unread. The last eight bytes are not read. */
struct DotrPointList {
    short points[16][2];
    short count;
    short unknown_42;
    unsigned char unknown_44[8];
};

/* One 516-byte output record: the three addresses passed to 0x21bc10. The
 * element types are assumed (two 256-byte areas of floats); 0x21bc10 stores a
 * halfword at +512 (the sample count less one); bytes +514..+515 are not seen used. */
struct DotrPointOutput {
    float unknown_000[64];
    float unknown_100[64];
    short unknown_200;
    unsigned char unknown_202[2];
};

/* The block at 0x1ed0000 as this routine addresses it: records from +556. The
 * code loads the address as a literal, so the target uses a constant pointer;
 * the host build substitutes an object (the glide_steps.h arrangement). */
struct DotrPointBlock {
    unsigned char unknown_000[556];
    struct DotrPointOutput outputs[2][16];
};
#ifdef __MWERKS__
#define DOTR_POINT_BLOCK ((struct DotrPointBlock *)0x1ed0000)
#else
extern struct DotrPointBlock dotr_point_block_01ed0000;
#define DOTR_POINT_BLOCK (&dotr_point_block_01ed0000)
#endif

extern struct DotrPointList root_no[2][16];

/* Taku_spline2 (0x21bc10) is declared in spline_paths.h. */

void Taku_spline_init(void);

#endif

#ifndef DOTR_EE_RH2_DATA_IN_H
#define DOTR_EE_RH2_DATA_IN_H

/* Texture-sheet rectangle tables: a sheet kind and an index select an x, y, width, height
 * rectangle copied to the caller.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The rectangle written: x, y, width, height halfwords and two words this function never
 * sets (copied as whatever the local holds). Assumed: field meanings from the values. */
struct DotrRh2Rect {
    short x;
    short y;
    short w;
    short h;
    int unk8;
    int unkC;
};

/* 0x106080 RH2_DATA_IN2 (name from the Japanese build's symbols): stores the 16-byte
 * rectangle of sheet `arg1` (0x0a, 0x14, 0x1e, 0x3c, 0x3d, 0x50, 0x5a, 0x64, 0x65, 0x6e,
 * 0x82, 0x83, 0x8c..0x8f, 0x96..0x9f), entry `arg2`, at arg0. */
void RH2_DATA_IN2(void *arg0, short arg1, short arg2);

#endif

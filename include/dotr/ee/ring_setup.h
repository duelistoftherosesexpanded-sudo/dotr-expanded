#ifndef DOTR_EE_RING_SETUP_H
#define DOTR_EE_RING_SETUP_H

/* Functions that fill fixed tables of points on a circle (0x20b480 / 0x20b550 of a phase,
 * scaled) and their colour cells at start-up.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Callees, defined in small_misc with its own struct names; these are
 * this group's views of the same arguments. */
struct DotrRingVector3 {
    float x, y, z;
};
struct DotrRingTriple {
    float unknown_00[12];
};
extern void Taku_Coord_Init(struct DotrRingTriple *m);
extern float RhMath_CosI(int phase);
extern float RhMath_SinI(int phase);
extern void Taku_Vector_Set(struct DotrRingVector3 *a, struct DotrRingVector3 *b);

/* View of the storage at 0x299f60 used by 0x2374d0: the block passed to
 * 0x21bae0, a word at +0xa0, sixteen groups of four 4-float rows from +0xec
 * and sixteen 52-byte cells from +0x4ec (a 3-float head passed to 0x21bb70,
 * a word at +0x20 and four 4-byte colours from +0x24). Only these fields are
 * evidenced. */
struct DotrRingCell {
    struct DotrRingVector3 unknown_00;
    unsigned char unknown_0c[20];
    int unknown_20;
    unsigned char color[4][4];
};
struct DotrRing {
    struct DotrRingTriple head;
    unsigned char unknown_30[0x70];
    int unknown_a0;
    unsigned char unknown_a4[0x48];
    float rows[16][4][4];
    struct DotrRingCell cells[16];
};
extern struct DotrRing prim_data;
extern struct DotrRingVector3 model_data;

/* 0x2374d0. Calls 0x21bae0 on the block head and clears the word at +0xa0.
 * For each group i of 16 and row j of 4: row 2 is (0, 8.65, 0, 0); rows 0,
 * 1 and 3 are (5 * RhMath_CosI(p), 8.65, 5 * RhMath_SinI(p), 0) with the phase
 * p = (k << 16) >> 5 for k = 2i, 2i + 1 and 2i + 2. Then for each of the 16
 * cells: Taku_Vector_Set(cell, 0x298ba0), the word at +0x20 cleared and the four
 * colours set to (128, 0, 0, 128), (0, 128, 0, 0), (0, 0, 128, 0) and
 * (128, 128, 128, 128).
 * Assumed: no arguments or result; the float constants are 0x40a00000 and
 * 0x410a6666. */
void Taku_Camp_3D_Poly_Init(void);

#endif

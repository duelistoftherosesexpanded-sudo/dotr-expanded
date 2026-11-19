#ifndef DOTR_EE_CELL_TINTS_H
#define DOTR_EE_CELL_TINTS_H

/* Functions that pass a four-byte colour and a range of 7 x 7 grid cells to SzDuel_SetMapColor (first member
 * 0x260490).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

/* Callees. 0x1e3060's fourth argument is declared int in
 * state_grid.h; here it is the address of four colour bytes, hence the view. */
extern void func_001e3060_tint(int a, int b, int c, unsigned char *colour, int e);
extern int GetCap(int limit, int value);

/* Views used by 0x1e3070 of the 7 x 7 grid at 0x318e60 (state_grid.h has the
 * defining group's view; this one names the first word as an item pointer, so the
 * host cell is wider than the target's 28 bytes), of the item bytes it reads
 * and of the halfword at 0x318118. */
struct DotrTintQuad {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
};
struct DotrTintItem {
    unsigned char unknown_00[0x44];
    unsigned char team;
    unsigned char unknown_45[0x12];
    signed char unknown_57_bit_0 : 1;
    signed char unknown_57_bit_1 : 1;
};
struct DotrTintCell {
    struct DotrTintItem *unit;
    struct DotrTintQuad quads[4];
    int unknown_14;
    unsigned char TargetSelectionHighlight;
    unsigned char SelectionHighlightType;
};
extern struct DotrTintCell data_00318e60_tint[7][7];
extern unsigned short data_00318118_tint[8];


/* 0x260490. Builds four bytes from `visualType`: {0, 0, 0, 255} for visualType <= 0,
 * {255, 255, 0, 128} for 11, {255, 128, 0, 128} for 13, {128, 0, 255, 128}
 * for 15, {0, 255, 0, 128} for 16 and {255, 255, 255, 128} otherwise. With
 * mode 18 it calls SzDuel_SetMapColor(i, y, c, bytes, e) and SzDuel_SetMapColor(x, i, c, bytes,
 * e) for i = 0..6. Otherwise, with d = 2 when `reach` is 1 and 1 when not,
 * it calls SzDuel_SetMapColor(i, j, c, bytes, e) for i from GetCap(0, x - d) to
 * min(x + d, 6) and j from GetCap(0, y - d) to min(y + d, 6).
 * Assumed: seven int arguments, no result. */
void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);

/* SzDuel_SetMapColor2 (state_grid.h declares it as an int-argument extern
 * for its caller 0x1e3060). Answers 0 for a cell outside 0..6 x 0..6. Else
 * stores `c` in the cell's byte +0x18, the four colour bytes (zeros for a
 * null pointer) in quad 0 and zeros in quad 2. With `f` set and an item in
 * the cell, quad 2 becomes {lo', lo', hi' + lo', 128} when the item's bit 1
 * of byte +0x57 is set (hi = high byte of the halfword at 0x318118,
 * lo = 128 - hi as a byte, v' = v * 255 >> 7), {0, 0, 255, 128} when the
 * item's byte +0x44 is 0, else {255, 255, 255, 128}. With `e` set, quads 1
 * and 3 become copies of quads 0 and 2. Answers 1.
 * Assumed: argument types and names, int result. */
int SzDuel_SetMapColor2(int x, int y, int c, struct DotrTintQuad *colour, int e, int f);

/* SzDuel_CalcMapColor (name from the Japanese build's symbols). Answers 0 for a cell outside
 * 0..6 x 0..6. While byte +0x19 differs from byte +0x18: lowers quad 1's byte 3 by
 * min(value, 8), or, once that byte is 0, copies +0x18 to +0x19; answers 1 without
 * touching anything else. Otherwise moves each byte of quad 1 towards quad 0 and of
 * quad 3 towards quad 2 by at most 8 and answers 1.
 * Assumed: argument types, int result, byte +0x19 as a member of the cell. */
int SzDuel_CalcMapColor(int x, int y);

#endif

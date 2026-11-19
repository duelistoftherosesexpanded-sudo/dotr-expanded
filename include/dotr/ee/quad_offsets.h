#ifndef DOTR_EE_QUAD_OFFSETS_H
#define DOTR_EE_QUAD_OFFSETS_H

/* Adjacent routines 0x218970 and 0x218ab0 that offset the four corner points of one 36-byte
 * record and forward them, with its 17 bytes and last halfword, to 0x218c10.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

#include "dotr/ee/small_state.h"

#ifdef __MWERKS__
typedef int DotrDrawWord;
#else
#include <stdint.h>
typedef intptr_t DotrDrawWord;
#endif

/* The record as the two routines read it: four (x, y) halfword pairs, sixteen
 * bytes, a byte the callee reads as a halfword selector, and a halfword at +34. */
struct DotrQuadRecord {
    short points[8];
    unsigned char bytes[16];
    unsigned char mode;
    unsigned char unknown_21;
    short unknown_22;
};

/* Taku_Prim_Set_G4 (cursor_packets declares it with int parameters). Its 27 arguments are fixed by
 * both callers: the first passes through untouched, eight are sign-extended halfwords, the rest
 * go on the stack. That it returns a draw cursor word, and that the first
 * argument is one, are assumptions (its last call is to 0x207650). */
extern DotrDrawWord Taku_Prim_Set_G4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
                                  short x3, short y3, unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6, unsigned char b7,
                                  unsigned char b8, unsigned char b9, unsigned char b10, unsigned char b11, unsigned char b12, unsigned char b13, unsigned char b14, unsigned char b15,
                                  unsigned short mode, short tail);

/* dx, dy, width and height are sign-extended halfwords in both routines. */
DotrDrawWord Taku_G4_Set(DotrDrawWord cursor, struct DotrQuadRecord *quad, short dx, short dy);
DotrDrawWord Taku_G4_Set2(DotrDrawWord cursor, struct DotrQuadRecord *quad, short dx, short dy, short width, short height);

/* 0x218fb0: the record continues with a halfword at +36 that selects a 20-byte
 * row of the table at 0x2950a0 (declared in small_state.h as DotrSmallSheetRow;
 * this routine reads the row as eight halfwords and the halfword at +16, so it
 * casts at the use instead of declaring the storage again). */
struct DotrQuadIndexedRecord {
    struct DotrQuadRecord quad;
    unsigned short row;
};
struct DotrQuadSheetRow {
    short values[8];
    unsigned short unknown_10;
    short unknown_12;
};

/* Taku_Prim_Set_GT4 (taku_quads). Its 37 arguments are fixed by this one caller; the
 * return type and the parameter names are assumptions. */
extern DotrDrawWord Taku_Prim_Set_GT4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
                                  short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
                                  unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3, unsigned char b4, unsigned char b5, unsigned char b6, unsigned char b7,
                                  unsigned char b8, unsigned char b9, unsigned char b10, unsigned char b11, unsigned char b12, unsigned char b13, unsigned char b14, unsigned char b15,
                                  unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail);
DotrDrawWord Taku_GT4_Set(DotrDrawWord cursor, struct DotrQuadIndexedRecord *record, short dx, short dy, short width, short height);

/* Taku_Prim_Set_FT4 (taku_quads) and Taku_Prim_Set_FT4_ST (taku_prims). Their 25 arguments are fixed by this
 * one caller (0x218020); the return type and the parameter names are assumptions.
 * camp_tikei.h declares Taku_FT4_Set over its own view of the same 38-byte record. */
extern DotrDrawWord Taku_Prim_Set_FT4(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
                                  short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
                                  unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3,
                                  unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail);
extern DotrDrawWord Taku_Prim_Set_FT4_ST(DotrDrawWord cursor, short x0, short y0, short x1, short y1, short x2, short y2,
                                  short x3, short y3, short v0, short v1, short v2, short v3, short v4, short v5, short v6, short v7,
                                  unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3,
                                  unsigned char mode, unsigned short row_word, unsigned char unknown_21, short tail);
DotrDrawWord Taku_FT4_Set(DotrDrawWord cursor, struct DotrQuadIndexedRecord *record, short dx, short dy, unsigned short mode);

#endif

#ifndef DOTR_EE_MATRIX_ROT_H
#define DOTR_EE_MATRIX_ROT_H

/* MATRIX_* helpers that rotate vectors with SIN_COS_INRAD (0x108950 first).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

#include "dotr/ee/paint_prims.h"

/* The angle argument reaches 0x108950 as the four-float copy 0x109100 makes, and is read there
 * as 32-bit integers: this union is that second view of the same 16 bytes (assumed; a structure
 * of four ints compiles to the same bytes). */
union DotrMatrixAngles {
    struct DotrPaintVector f;
    int i[4];
};

/* The second parameter is short here: every call in 0x108950 sign-extends a 32-bit angle from 16
 * bits just before the call, which the compiler only emits for a short parameter; the
 * definition (fx_calc.h) is (float *tbl, short deg, int mode). obj_vectors.h declares it with
 * int parameters for 0x106cf0; the views are not reconciled. The first parameter is passed this
 * function's fourth argument. */
extern float SIN_COS_INRAD(int unit, short angle, int cosine);

/* 0x108950: rotates the row vector about one axis (0: x/z by angle word 1, 1: y/z by word 0,
 * 2: x/y by word 2); any other axis leaves it alone. Writes back all 16 bytes of a local whose
 * fourth float is never set. Returns 0. The name is the Japanese build's symbol. */
int MATRIX_XYZ_ININ33(struct DotrPaintVector *row, struct DotrPaintVector *v, int axis, int unit);

#endif

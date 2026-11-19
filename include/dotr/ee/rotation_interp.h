#ifndef DOTR_EE_ROTATION_INTERP_H
#define DOTR_EE_ROTATION_INTERP_H

/* RhGeom_RoationComplexInterpolate: interpolates two packed rotations, choosing the shorter of
 * the target's two equivalent Euler forms; callees are the sine/cosine and arctangent helpers.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

/* Callees, as small_misc.h (RhMath_SinI/RhMath_CosI, defined there) and fx_calc.h
 * (0x1a6c18) declare them. Assumed: the phases are 0x10000 per turn; 0x1a6c18 (library code,
 * out of scope) is a two-argument arctangent (y, x) in radians. */
extern float RhMath_SinI(int phase);
extern float RhMath_CosI(int phase);
extern float func_001a6c18(float a, float b);

/* 0x20d240 RhGeom_RoationComplexInterpolate (name from the Japanese build's symbols). Assumed:
 * three-word rotations of 0x10000 per turn (low halfword used), result = from * weight0 +
 * to * weight1. */
void RhGeom_RoationComplexInterpolate(int *out, int *from, int *to, float weight0, float weight1);

#endif

#ifndef DOTR_EE_SPLINE_PATHS_H
#define DOTR_EE_SPLINE_PATHS_H

/* Cubic spline sampling of point lists into output coordinate arrays, called from the point
 * list setup at 0x22c450.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/campaign.md. */

/* Defined in small_misc (same prototypes as small_misc.h). func (0x21c320) is
 * assumed to give the right-hand side of the spline system for the middle point
 * from three coordinates and the two interval lengths around it. */
extern float func(float a, float b, float c, float d, float e);
extern void tri_diag(float *lower, float *diagonal, float *upper, float *right, float *solution, int n);

/* Library-block routines, declared only; assumed from their use to be powf
 * (0x1a6c48), sqrtf (0x1a6c60) and a float-to-int conversion (0x1a22c0); unverified. */
extern float func_001a6c48_spline(float x, float y);
extern float func_001a6c60(float value);
extern int func_001a22c0(float value);

/* 0x21bc10. x and y receive up to 64 sampled coordinates each (the caller passes
 * two 256-byte areas); count is a halfword (stored with sh). pts holds n (x, y)
 * points and h the n - 1 interval lengths. e selects fixed 0.1 steps (e == 1) or
 * a sample count from the segment length divided by scale. Names are assumed. */
void Taku_spline2(float *x, float *y, short *count, float (*pts)[2], float *h, int n, short e, float scale);

#endif

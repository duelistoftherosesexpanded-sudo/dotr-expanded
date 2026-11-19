#ifndef DOTR_EE_LK_MATH_H
#define DOTR_EE_LK_MATH_H

/* Lk_ helpers that do vector and angle arithmetic: a random rotated vector, a screen angle
 * between two projected points, frame coordinates and yaw/pitch angles between two points.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* LkRegulateRotBorder's body (dotr_lk_regulate_rot_border), which LkMoveHoming inlines. */
#include "dotr/ee/lk_frames.h"

/* Library-block routines, declared only (assumed from their use, as in other groups):
 * func_0019a5c8 returns the next pseudo-random number, func_001a22c0 converts a float to an
 * int (truncating), func_001a6c18 is atan2f(a, b). */
extern int func_0019a5c8(void);
extern int func_001a22c0(float value);
extern float func_001a6c18(float a, float b);
/* func_001a6c60 (library block, declared only) assumed sqrtf. */
extern float func_001a6c60(float value);

/* A four-float vector. */
struct DotrLkVec {
    float x, y, z, w;
};
/* The 16-byte vector constant at 0x28c220, copied whole; its contents are not examined. */
extern struct DotrLkVec data_0028c220_lk;
/* RhGeom_GetRotXYZMatrix (0x20c630, small_misc) and RhGeom_MulMatrixVector (0x20cdd0,
 * vu0 assembly) as this caller uses them: a 4x4 float matrix from three angles (the fourth
 * word of the angle array is not set), and out = m * v. */
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
extern void RhGeom_MulMatrixVector(struct DotrLkVec *out, float *m, struct DotrLkVec *v);

/* The screen point Ex3d_RotTransPers (0x205440, ex3d_clip) writes: two unsigned halfword
 * coordinates (12.4 fixed point, as the >> 4 suggests) and a depth word. */
struct DotrLkScreen {
    unsigned short x, y;
    int z;
};
/* Ex3d_RotTransPers as this caller uses it (ex3d_clip.h has the main declaration). */
extern int func_00205440_lk(void *in, float *matrix, struct DotrLkScreen *packet, float *w);
/* The object whose world-to-screen matrix is at +48. */
struct DotrLkObject {
    char unknown_00[48];
    float matrix[16];
};

/* 0x1043f0 LkGetRandVec: copies the constant vector, sets its y to len, rotates it by two
 * random angles (0 .. 0xffff about x and y, 0 about z) and stores the result in *out;
 * returns 0. Assumed: an int result (v0 is set to 0). */
int LkGetRandVec(struct DotrLkVec *out, float len);
/* 0x1037c0 LkSetAngleW2S: projects p1 and p2 with the object's matrix and returns the screen
 * angle from p2 to p1, atan2f((ax - bx) >> 4, (ay - by) >> 4) scaled to 65536 per turn,
 * converted to int and reduced % 0x10000. MATCHING DEVICE: two unused filler locals (8 bytes
 * before the first point, 4 between the second point and w) give the original frame; their
 * real use is unknown, and they exist in the target build only. */
int LkSetAngleW2S(struct DotrLkObject *obj, void *p1, void *p2);
/* 0x105c40 LkAssignFrameST: from p[0..3], s = p0 / p2 (0 when p0 is 0) and t = p1 / p3 (0 when
 * p1 is 0); returns int(t + int(s) * (1 / p3)). MATCHING DEVICES: `0.0f == p[i]` with the zero
 * first; t and 1 / p3 held in a local float pair (separate variables swap f20/f21); the
 * truncation of s as its own statement. */
int LkAssignFrameST(float *p);
/* 0x103db0 LkChangeAxisY: d = a - b (x, y, z); out[1] = angle(atan2f(dx, dz)) and
 * out[0] = angle(atan2f(sqrtf(dx*dx + dz*dz), dy)), angle(t) = int(t / 6.2831855 * 65536)
 * % 0x10000 (C remainder: negative stays negative); out[2] = out[3] = 0; returns 0.
 * MATCHING DEVICE: dx, dy, dz declared together and assigned in x, y, z order. Assumed:
 * float and int pointers, the int result. */
int LkChangeAxisY(float *a, float *b, int *out);
/* 0x103ec0 LkMoveHoming (name from the Japanese build's symbols): target -= pos (written
 * back); six 16-bit angles (int(atan2f(a, b) / 2pi * 65536) % 0x10000, C remainder) from the
 * target, ty = (x, z), tx = (z, y), tz = (y, x), and likewise dy, dx, dz from dir; each pair
 * goes through LkRegulateRotBorder's body (inlined; ang = {(tx, dx), (ty, dy), (tz, dz), 0})
 * and is clamped to -limit .. limit; dir is rotated by the RhGeom_GetRotXYZMatrix matrix of
 * ang, pos += the result, dir = the result (xyz), dir->w = 0; returns 0. MATCHING DEVICE:
 * locals in the order m, ang, out, ty, tx, tz, dy, dx, dz. Assumed: float pointers for pos and
 * target, the vector type for dir, an int limit and the int result. */
int LkMoveHoming(float *pos, float *target, struct DotrLkVec *dir, int limit);

#endif

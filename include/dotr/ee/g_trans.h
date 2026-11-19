#ifndef DOTR_EE_G_TRANS_H
#define DOTR_EE_G_TRANS_H

/* G_ helpers near the start of the game code: perspective projection of three or four copied
 * vectors, a transition curve value and a rotation matrix from a direction vector.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* A four-float vector, copied whole. */
struct DotrGVec {
    float x, y, z, w;
};
/* The matrix at 0x30ff00 passed to the projection; contents not examined. */
extern float data_0030ff00_g[];
/* Ex3d_RotTransPersAutoCalcQ (0x2058d0, ex3d_calc) as these callers use it: a vector
 * array, a matrix and three further words passed through. Assumed: an int result, which the
 * callers return. */
extern int func_002058d0_g(struct DotrGVec *v, float *m, int a, int b, int c);
/* Library-block routines, declared only; assumed from their use to be sinf (0x1a6b70) and
 * powf (0x1a6c48), not checked. */
extern float func_001a6b70_g(float x);
extern float func_001a6c48_g(float x, float y);

/* 0x100b00 GRotTransPers_3: copies three vectors into a local array and returns the
 * projection of the array with the matrix at 0x30ff00 and the last three arguments.
 * Assumed: the int return (v0 passed through). */
int GRotTransPers_3(struct DotrGVec *v0, struct DotrGVec *v1, struct DotrGVec *v2, int a, int b, int c);
/* 0x100ba0 GRotTransPers_4: the same with four vectors. */
int GRotTransPers_4(struct DotrGVec *v0, struct DotrGVec *v1, struct DotrGVec *v2, struct DotrGVec *v3, int a, int b, int c);
/* 0x100d50 GCalc_Trans: 1 - powf(sinf(2pi * (((b - a) << 16) / (b * 5)) / 65536) /
 * sinf(1.2566179), n), the integer quotient truncated (b = 0 traps in the original). The
 * angle 1.2566179 is 2pi / 5. */
float GCalc_Trans(int a, int b, int n);

/* For GVec2RotMat_Z (0x100e40). RhGeom_UnitMatrix (0x20bbb0, an assembly routine in asm/ee/vu0,
 * declared in view_state.h with a matrix type) and RhGeom_RotXYZMatrix (0x20cb90, declared in
 * small_misc.h with its own matrix type) viewed with a float pointer; RotXYZMatrix builds a
 * rotation from the angle words and applies it to out (assumed, as in fx_calc). Library-block
 * routines declared only, assumed from their use: 0x1a6c18 atan2f(a, b), 0x1a6c60 sqrtf, 0x1a22c0
 * float to int. */
extern void func_0020bbb0_g(float *m);
extern void func_0020cb90_g(float *out, int *ang);
extern float func_001a6c18_g(float a, float b);
extern float func_001a6c60_g(float x);
extern int func_001a22c0_g(float x);
/* The angle template at 0x28c210 (the one GRot_Matrix copies, fx_calc): four words, one per
 * axis plus an unused fourth (layout assumed); its contents are not examined. */
struct DotrGAngles {
    int x, y, z, w;
};
extern struct DotrGAngles data_0028c210_g;
/* 0x100e40 GVec2RotMat_Z: out = unit matrix, then rotated about z (axis word 2) by
 * atan2f(v[1], v[0]) and about y (word 1) by atan2f(sqrtf(powf(v[0], 2) + powf(v[1], 2)), v[2]),
 * each angle converted to 65536 units per turn and masked to 16 bits. MATCHING DEVICE: the two
 * rotations are GRot_Matrix (0x100e00) inlined from a static inline copy in the source; a call
 * gives `jal 0x100e00`. Assumed: void result, float pointers. */
void GVec2RotMat_Z(float *out, float *v);

/* For GSet_RandVector (0x101080). Library-block routines (0x17b48c-0x1a6c80), declared only:
 * 0x19a5c8 returns an int and takes nothing (assumed rand; fx_calc.h declares it the same
 * way); 0x187b48 (out, in, scale) assumed to scale a four-float vector (as fx_calc.h);
 * 0x1878e0 (out, m, in) assumed to apply a 4 x 4 matrix to a four-float vector. */
extern int func_0019a5c8(void);
extern void func_00187b48(float *out, float *in, float scale);
extern void func_001878e0(float *out, float *m, float *in);
/* The 1536 bytes at 0x28bc10 (ending at the angle template 0x28c210), read as floats indexed
 * [0..3][0..31][0..2] by this function; the shape is taken from the index ranges, the
 * contents and the real object bounds are not examined. */
extern float data_0028bc10[4][32][3];
/* 0x101080 GSet_RandVector: out[0..2] each = data_0028bc10[(rand >> 23) % 4][(rand >> 23) % 32]
 * [(rand >> 23) % 3] (three draws, used in that order by the target compiler) times the sign
 * 2 * ((rand >> 23) % 2) - 1 from a fourth draw; out[3] = 1; out scaled by scale; a unit
 * matrix rotated about x, y and z (axis words 0, 1, 2) by ((rand >> 23) % 256) << 8 each,
 * then applied to out. MATCHING DEVICE: GRot_Matrix inlined as in GVec2RotMat_Z; the element
 * as a static inline helper with a sign local; the angle written `% 256) << 16 >> 8`.
 * Assumed: void result, float pointer. */
void GSet_RandVector(float *out, float scale);

#endif

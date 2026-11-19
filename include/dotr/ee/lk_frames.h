#ifndef DOTR_EE_LK_FRAMES_H
#define DOTR_EE_LK_FRAMES_H

/* Lk and S_I_P_IN frame helpers around 0x103920-0x108ef0: each builds a local
 * scale/rotation/translation matrix from a frame and multiplies it into the world-screen
 * matrix (RhView_GetWorldScreenMatrix, RhGeom_GetScaleRotXYZTransMatrix, RhGeom_MulMatrix).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* 112-byte frame as these functions use it: the three 16-byte members are handed to
 * RhGeom_GetScaleRotXYZTransMatrix as scale, rotation and position (member names follow
 * that callee's parameter order; small_misc.h views the same layout as DotrMiscFrame). */
struct DotrLkFrame {
    float scale[4];
    int rot[4];
    float pos[4];
    float matrix[16];
};
struct DotrLkMatrix {
    float m[16];
};

/* 0x20eb40, 0x20c940, RhGeom_MulMatrix (declared in view_state.h / pool_table.h with
 * other pointer views of the same 64-byte blocks). */
extern void RhView_GetWorldScreenMatrix(float *out);
extern void RhGeom_GetScaleRotXYZTransMatrix(float *outMatrix, float *scale, int *rot, float *pos);
extern void RhGeom_MulMatrix(float *m, float *other);
/* 0x205cb0: fills three floats from a screen position (view_state.h has the main declaration). */
extern void Ex3d_GetPointOnScreen(float *out, float x, float y);
/* 0x205c40: fills three rotation words; its float result is not used here. */
extern float Ex3d_GetLookVector(int *out);

/* 0x1039a0: copies the look rotation into the frame (the middle word reduced with % 65536),
 * rebuilds the frame matrix and answers the unreduced middle word. int result assumed from v0. */
int LkRotSF(struct DotrLkFrame *frame);
/* 0x103920: position from Ex3d_GetPointOnScreen(x, y) (floats passed through untouched),
 * LkRotSF, then the frame matrix again; answers 0. */
int LkChange2D(struct DotrLkFrame *frame, float x, float y);
/* 0x108ef0: world-screen matrix times the matrix of the three given members, copied to out.
 * Parameter types assumed from the callee. */
void S_I_P_IN(struct DotrLkMatrix *out, float *scale, int *rot, float *pos);


/* 0x103300: adds step and returns 1 while the result stays at or below limit; otherwise stores
 * limit and returns 0. */
int LkIncrSHORT(short *value, short limit, short step);
/* 0x1034b0: subtracts step and returns 1 while the result stays at or above limit; otherwise
 * stores limit and returns 0. */
int LkDecrSHORT(short *value, short limit, short step);


/* Library-block routines (outside the game code, declared only): two floats to float, and
 * float to int, as other groups declare them. */
extern float func_001a6c18(float a, float b);
extern int func_001a22c0(float value);
/* 0x103760: 0x1a22c0(360 / (6.2831855 / 0x1a6c18(vector[1], vector[0])) + 90). The int
 * result is an assumption: a void function discarding it compiles to the same bytes. */
int LkSetAngleXY_m90(float *vector);

/* 0x103d00: signed shortest difference of two 16-bit angles: from - to when its low 16 bits
 * are at most 0x8000, otherwise the difference taken the other way round the 0x10000 circle;
 * 0 when equal. Parameter meaning assumed from the arithmetic. */
int LkRegulateRotBorder(int from, int to);
/* The body of LkRegulateRotBorder, kept here as the single source: 0x103d00 returns it and
 * LkMoveHoming (0x103ec0, lk_math) has it inlined three times in the original. */
static inline int dotr_lk_regulate_rot_border(int from, int to)
{
    int diff;
    int result;

    if (from > to) {
        diff = from - to;
        if (diff % 0x10000 <= 0x8000) {
            result = diff;
        } else {
            result = -(0x10000 - from + to);
        }
    } else if (from < to) {
        diff = to - from;
        if (diff % 0x10000 <= 0x8000) {
            result = -diff;
        } else {
            result = 0x10000 - to + from;
        }
    } else {
        result = 0;
    }
    return result;
}

/* 0x103c40: normalizes a copy of the vector, crosses the unit Y axis with it, converts
 * 0x1a6c00(length of the cross product) from radians to a 16-bit angle and builds the
 * rotation matrix about the cross product with RhGeom_GetRotVectorMatrix; returns 0.
 * The four-float layout is assumed; the RhGeom callees are declared in view_state.h with
 * its own vector type (same layout). 0x1a6c00 is a library-block routine taken as float to
 * float. */
struct DotrLkVector {
    float x, y, z, w;
};
extern void RhGeom_NormalizeVector(struct DotrLkVector *v);
extern void RhGeom_OuterProduct(struct DotrLkVector *out, struct DotrLkVector *a, struct DotrLkVector *b);
extern float RhGeom_GetVectorScalor(struct DotrLkVector *vector);
extern void RhGeom_GetRotVectorMatrix(float *m, struct DotrLkVector *axis, int phase);
extern float func_001a6c00(float value);
int LkVec2Mat(struct DotrLkVector *v, float *m);

/* 0x103a30 LkVec2Rot, 0x103b50 LkVec2RotXY (names from the Japanese build): turn a direction
 * vector into three 16-bit angles (a full turn is 0x10000) with the two-argument angle
 * helper 0x1a6c18; LkVec2RotXY takes the first angle against the length in the x/z plane
 * (0x1a6c60, a square root by its use) and clears the third. Both clear rot[3] and return 0.
 * Assumed: float and int element views of the two arguments, int result. Both are
 * compiled warm. */
extern float func_001a6c60(float value);
int LkVec2Rot(float *v, int *rot);
int LkVec2RotXY(float *v, int *rot);

#endif

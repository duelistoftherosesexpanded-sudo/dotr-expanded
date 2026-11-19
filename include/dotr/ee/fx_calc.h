#ifndef DOTR_EE_FX_CALC_H
#define DOTR_EE_FX_CALC_H

/* GCalc_* ratio helpers at 0x100ff0-0x101020 and ShFx_* effect helpers at 0x1018e0-0x1031c0
 * (names from the Japanese build).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Two frame numbers read as unsigned halfwords at +0 and +2. */
struct DotrFxBlackSpan {
    unsigned short darken_frame;
    unsigned short lighten_frame;
};

/* Also declared in block_table.h and index_fanout.h. */
extern float SzView_GetScreenDistance(int index);
extern void SzEffect_SetBlackOutForBattle(int blackOutValue);

float GCalc_Decrease(float value, float base, float limit);
float GCalc_Increase(float value, float limit);
float ShFx_GetDrawSize(float size, float depth);
void ShFx_BlackBG(struct DotrFxBlackSpan *span, int frame);

/* Library-block routines (outside the game code, declared only):
 * 0x19a5c8 returns an int and takes nothing; 0x187b00, 0x187b18 and 0x187b48 take an
 * output and two inputs / an input and a float, assumed add, subtract and scale of
 * four-float vectors from how GGet_TriangleCenter combines them. */
extern int func_0019a5c8(void);
extern void func_00187b00(float *out, float *a, float *b);
extern void func_00187b18(float *out, float *a, float *b);
extern void func_00187b48(float *out, float *in, float scale);

/* Two 14-bit fields in halfwords; the top two bits of each halfword are kept by GSet_WindowUv. */
struct DotrWindowUv {
    unsigned short u : 14;
    unsigned short u_high : 2;
    unsigned short v : 14;
    unsigned short v_high : 2;
};
/* Converts a 12.4 screen position (origin 1728, 1824) plus an offset to window texels, clamps
 * to 1-639 and 1-447 and stores both as 12.4 values. Types assumed from the code. */
void GSet_WindowUv(struct DotrWindowUv *out, unsigned short *xy, int dx, int dy);
/* ShFx_GetAlpha reads only the byte at +5; ShFx_SetTexFT4 / ShFx_SetTexGT4 also read the four
 * words at +8 as a texel rectangle (the words ShFx_SetUV writes). The rest is unknown here. */
struct DotrFxAlphaSource {
    unsigned char unknown_00[5];
    unsigned char mode;
    int left, right, top, bottom;
};
/* Writes four bytes chosen by mode 1-4; other modes leave out untouched. */
void ShFx_GetAlpha(struct DotrFxAlphaSource *source, unsigned char *out);
/* Texture size in texels for kinds 0-9 (jump table at 0x2b9010); other kinds give 0, 0.
 * The unsigned kind and the float outputs are assumed from the code. */
void ShFx_GetTexWidth(unsigned int kind, float *width, float *height);
/* Frame number masked to the kind's frame count (jump table at 0x2b9040); 0 for kinds 6 and up. */
int ShFx_GetAnmID(unsigned int kind, int frame);
/* Fills count bytes with func_0019a5c8() >> 4. */
void ShFx_MakeRnd(unsigned char *out, int count);
/* out = (a + b) / 2 + (c - (a + b) / 2) / 3, through two local vectors. */
void GGet_TriangleCenter(float *out, float *a, float *b, float *c);

/* 0x101050: value clamped to [low, high]; low is tested first. */
int GWithin_LimitsI(int value, int low, int high);
/* 0x1f5f90, declared in packed_item.h; here the output is seen as four floats. */
extern int SzEffect_GetModelPos2(int model, float *pos);
/* 0x100d00: -1 when the model position's third float is negative, otherwise 1. */
int GGet_ModelPosZSign(int model);

/* 0x100e00: copies the 16-byte angle template at 0x28c210, replaces the word picked by axis with
 * the low 16 bits of angle and builds the matrix with RhGeom_RotXYZMatrix (0x20cb90, declared in
 * small_misc.h with its own matrix type). The four-word layout, the int angle and the float pointer
 * are assumptions; the template's contents are not asserted here. */
struct DotrFxAngles {
    int x, y, z, w;
};
extern struct DotrFxAngles data_0028c210;
extern void RhGeom_RotXYZMatrix(float *out, int *ang);
void GRot_Matrix(float *out, int angle, int axis);
/* 0x1020b0: cell rectangle of frame `index` in a sheet whose cell size and grid depend on kind
 * (0-9); writes left, right, top, bottom to out[2..5], all zero when the index is past the grid.
 * A kind above 9 leaves the sizes unset in the original. out is assumed to be an int block. */
void ShFx_SetUV(int *out, int index, unsigned int kind);
/* 0x101600 / 0x101770: 1 when pos[0] and pos[1] (unsigned 16-bit, compared as floats) lie inside
 * 22528..43008 by 25600..39936, resp. 27648..37888 by 29184..36352; pointer type assumed. */
int ShFx_isClip(unsigned short *pos);
int ShFx_isClipSC(unsigned short *pos);
/* 0x101430: builds three points (x, y from the unsigned 16-bit pairs, z = 0, w = 1), asks
 * Ex3d_GetSurfaceNormal (0x205e70, view_state) for their normal and returns 1 when its z is
 * negative, else 0. Pointer types assumed from the halfword loads. */
int GJudge_FrontBack2(unsigned short *a, unsigned short *b, unsigned short *c);
/* 0x102390: second-derivative table z[0..n-1] of the natural cubic spline through (x[i], y[i]);
 * two eight-float work arrays on the stack, so n is at most 8 (not checked). Parameter order
 * and float pointers assumed from the code. */
void ShFx_Spline_MakeTable(int n, float *x, float *y, float *z);
/* 0x102550 ShFx_Spline (Japanese build name). Assumed from the code: the block holds the point
 * count, the x, y and z of up to eight control points, and 28 four-float output points (x, y,
 * z written). */
struct DotrFxSplinePoint {
    float x;
    float y;
    float z;
    float w;
};
struct DotrFxSpline {
    int count;
    float x[8];
    float y[8];
    float z[8];
    struct DotrFxSplinePoint out[28];
};
int ShFx_Spline(void *arg0);

/* 0x108030: a table of 91 floats (read as the sine of 0..90 degrees), a signed 16-bit angle in
 * degrees and a selector: 0 reads it as sine, anything else as cosine. Types assumed from the code; the callers
 * in obj_vectors and matrix_rot declare the first two parameters as integers. */
float SIN_COS_INRAD(float *tbl, short deg, int mode);

/* ShFx_Spin2P (0x102cb0). Assumed from the code: the argument block holds a point count, a
 * radius, a start angle and an angle step in degrees, two three-float points and the address
 * of count + 1 three-float output points. The locals are a 16-float matrix, two four-float
 * vectors and an angle block of which three words are written. Library-block callees
 * (declared only) are read from their use: 0x1a6c60 square root, 0x1a6c18 two-argument
 * arctangent, 0x1a6898 cosine, 0x1a6a88 sine, 0x1a22c0 float to int. RhGeom_GetVectorScalor,
 * RhGeom_MulMatrixVector and func_001a6c60 are declared in view_state.h, which the source
 * includes. */
struct DotrFxPoint {
    float x, y, z;
};
struct DotrFxSpin {
    int count;
    float radius;
    float angle;
    float turn;
    struct DotrFxPoint start;
    struct DotrFxPoint end;
    struct DotrFxPoint *out;
};
extern float func_001a6c18(float a, float b);
extern int func_001a22c0(float value);
extern float func_001a6898(float value);
extern float func_001a6a88(float value);
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
void ShFx_Spin2P(struct DotrFxSpin *p);
/* ShFx_Hsin2P (0x102f20): the same argument block, locals and callees as ShFx_Spin2P (assumed
 * likewise). Point 0 is start; points 1..count lie on the circle at angle + i * turn degrees
 * with radius radius * sin(i * 180 / count degrees), at height i * length / count, turned by
 * the direction matrix and moved to start; the end point is not copied. */
void ShFx_Hsin2P(struct DotrFxSpin *p);
/* ShFx_Vec2Rot (0x101940) / ShFx_Vec2Mat (0x101a00): the two 16-bit angles of a direction
 * (pitch from atan2(sqrt(x*x + z*z), y), yaw from atan2(x, z), third word 0), written to rot
 * or, through a local angle block, turned into a matrix by RhGeom_GetRotXYZMatrix. Assumed:
 * the direction is three floats, rot three ints, the matrix a float pointer. */
void ShFx_Vec2Rot(struct DotrFxPoint *v, int *rot);
void ShFx_Vec2Mat(struct DotrFxPoint *v, float *m);

/* ShFx_SetTexFT4 (0x101d20) / ShFx_SetTexGT4 (0x101ee0): set bit 0x800 of the halfword at +2,
 * store the rectangle's corners as 12.4 texel pairs in the four vertex slots (stride 16 from
 * +24 for FT4, stride 24 from +24 for GT4, top two bits of each halfword kept), then assign a
 * region clamp (modes 2, 2, right and bottom minus one), the texture with a third argument 1
 * and the alpha value built from the four ShFx_GetAlpha bytes; GT4 skips the alpha when the
 * mode byte is 0. The packet layouts are assumed from the code; the other
 * callees are declared in draw_state.h, which the sources include, and ExAssignTexture
 * (0x208700, ex_texture) is declared here with the same packet type. */
struct DotrFxFT4Packet {
    unsigned short unknown_00;
    unsigned short flags;
    int unknown_04[5];
    struct {
        struct DotrWindowUv uv;
        int unknown_04[3];
    } vertex[4];
};
struct DotrFxGT4Packet {
    unsigned short unknown_00;
    unsigned short flags;
    int unknown_04[5];
    struct {
        struct DotrWindowUv uv;
        int unknown_04[5];
    } vertex[4];
};
struct DotrDrawPacket;
extern int ExAssignTexture(struct DotrDrawPacket *packet, int *texture, int a);
void ShFx_SetTexFT4(struct DotrFxFT4Packet *packet, struct DotrFxAlphaSource *source, int *texture);
void ShFx_SetTexGT4(struct DotrFxGT4Packet *packet, struct DotrFxAlphaSource *source, int *texture);

/* ShFx_Vib2P (0x102a50): fills count + 1 points from start to end. The ends
 * are copied; each point between is start + step * i with, per coordinate, a random offset
 * ((func_0019a5c8() >> 4) % range - range / 2) / 100, where range is the float-to-int of
 * 200 * amplitude * the step's length (converted again for every coordinate, as the original
 * does). Assumed from the code: the argument block holds a count, an amplitude, two
 * three-float points and the address of the output points; the step is a three-float local
 * handed to RhGeom_GetVectorScalor, which is declared in view_state.h with a four-float
 * vector (cast at the call). */
struct DotrFxVib {
    int count;
    float amplitude;
    struct DotrFxPoint start;
    struct DotrFxPoint end;
    struct DotrFxPoint *out;
};
void ShFx_Vib2P(struct DotrFxVib *p);

#endif

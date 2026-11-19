#ifndef DOTR_EE_OBJ_VECTORS_H
#define DOTR_EE_OBJ_VECTORS_H

/* OBJ_, PV_IN_, V0IN_ and PAINT_VECTOR_ vector fillers at 0x106ab0-0x108d20 (names from the
 * Japanese build).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Four floats per element; the fourth is never written by these functions. */
struct DotrObjVector {
    float x;
    float y;
    float z;
    float w;
};

/* Library-block float-to-int conversion (out of scope). */
extern int func_001a22c0(float value);

/* PV_IN_ fills halfwords 0-3 when second is zero and 4-7 otherwise. */
void PV_IN_(short *target, float *source, int second);
/* The pointer is declared after the floats: with it first the compiler copies
 * a0 before the float moves, which the target does not. */
void OBJ_ROT(float x, float y, float z, int *target);
void OBJ_I_V_IN(int *target, float *source);
void OBJ_VE_SCL_KAKE_4(struct DotrObjVector *vectors, struct DotrObjVector *scale);
/* 0x106fa0: adds delta's x, y, z to each of four vectors (w untouched); types as in OBJ_VE_SCL_KAKE_4. */
void POS_POS_4(struct DotrObjVector *vectors, struct DotrObjVector *delta);
int V0IN_V1_V2_XZ(struct DotrObjVector *pair, struct DotrObjVector *to, struct DotrObjVector *from);
int PAINT_VECTOR_4TG_SET_IN(struct DotrObjVector *corners, float half_width, float half_height);
int PAINT_VECTOR_4TG_SET_IN2(struct DotrObjVector *corners, float half_width, float half_depth);

/* SDK-block routine at 0x195108 (out of scope, declared only); the call shape is
 * (pointer, int). */
extern void func_00195108(void *a, int b);

/* Three-component add, multiply, set and copy; w is untouched. */
void POS_POS(struct DotrObjVector *target, struct DotrObjVector *delta);
void OBJ_VE_SCL_KAKE(struct DotrObjVector *target, struct DotrObjVector *scale);
void OBJ_SCL2(struct DotrObjVector *target, float x, float y, float z);
/* reverse == 0 copies vector into point, otherwise point into vector. */
int POINT_IN_VECTOR_(struct DotrObjVector *point, struct DotrObjVector *vector, int reverse);
/* The size is sign-extended from 16 bits before the call; returns target. */
void *OBJ_ADDRESS_IN(void *target, short size);
/* Byte colour stores: three bytes from arguments, four bytes copied. */
int OBJ_RGB_IN(unsigned char *target, unsigned char r, unsigned char g, unsigned char b);
int OBJ_GsRGB_RGB_IN(unsigned char *target, unsigned char *source);
/* Stores three floats at 0x449c of an otherwise unknown object. */
int PSYS_RR2H_IN(void *system, float x, float y, float z);

/* SIN_COS_INRAD (0x108030, fx_calc): this view takes three int-register arguments and returns
 * a float, as XC_ZS_IN calls it; its parameter names follow XC_ZS_IN's arguments. The
 * definition (fx_calc.h) is (float *tbl, short deg, int mode), and matrix_rot.h has a third
 * view (int, short, int). XC_ZS_IN passes its `angle` as the table and its `unit` as the
 * angle, so those two parameter names of XC_ZS_IN are probably swapped. */
extern float SIN_COS_INRAD(int angle, int unit, int cosine);
/* x = radius * SIN_COS_INRAD(angle, unit, 1), z = radius * SIN_COS_INRAD(angle, unit, 0). */
int XC_ZS_IN(struct DotrObjVector *target, int unit, int radius, int angle);
/* Halfwords at 0xa8/0xaa from two converted floats, the third float at 0xa0. */
/* The pointer is declared after the floats for the same reason as OBJ_ROT. */
int OBJ_RR2H_IN(float x, float y, float z, void *object);


/* 0x106b90: subtracts amount from the bytes at 0xaf and 0xb3; returns 1 when the byte at 0xaf is
 * then below floor. */
int OBJECT_RGB_A_OFF(unsigned char *object, int amount, unsigned char floor);

/* 0x106bc0: adds amount to the bytes at 0xaf and 0xb3; returns 1 when the byte at 0xaf is then
 * above ceiling. */
int OBJECT_RGB_A_ON(unsigned char *object, int amount, unsigned char ceiling);
/* 0x106bf0: adds amount to three bytes when off is zero, subtracts otherwise; the third argument is
 * not read. */
int OBJECT_RGB_INOFF(unsigned char *rgb, int amount, int unused, int off);
/* 0x106d80: adds amount to *angle and reduces it modulo 360 when it leaves (-360, 360). */
int RAD_IN_360_(int amount, short *angle);
/* 0x107070: adds delta to x (axis 0), y (low 16 bits of axis equal 1) or z of four vectors.
 * The zero test uses the whole register and the second test its low 16 bits, hence long. */
int XYZ_POS_4(struct DotrObjVector *vectors, long axis, float delta);
/* Library-block routines (out of scope, declared only): float in, float out is what
 * the callers show. */
extern float func_001a6c60(float value);
extern float func_001a6a88(float value);
/* 0x107b90: *dx = f(d * d) with d = point->x - (pair[0].x + pair[1].x), likewise z; f is 0x1a6c60. */
void CARD_X_Z_IN_(float *dx, float *dz, struct DotrObjVector *pair, struct DotrObjVector *point);
/* 0x107c10: out[i] = 0x1a6a88(i * 3.1415927f / 180.0f) for i = 0..90. */
void SIN_IN_PSYS(float *out);
/* 0x107cb0 SIN_IN_PSYS2 (name from the Japanese build's symbols; parameters, the block layout
 * and its member names are assumptions): for i < count writes 0x1a6a88 and 0x1a6898 of
 * i * step degrees in radians, and of (i + 1) * step degrees, to four float blocks 80 bytes
 * apart. 0x1a6898 is a library-block routine like 0x1a6a88. */
extern float func_001a6898(float value);
struct DotrSineBlocks {
    float sin_at[20];
    float cos_at[20];
    float sin_next[20];
    float cos_next[20];
};
void SIN_IN_PSYS2(struct DotrSineBlocks *out, int step, int count);
/* Defined in packed_item; the scale is seen here as floats. */
extern int SzEffect_IsModelStandby2(int model);
extern int SzEffect_SetModelScale2(int model, float *scale);
/* 0x107310: when the model is on standby and on is non-zero, sets its scale to (1, 1, 1); the
 * second argument is not read. */
/* Defined in packed_item with this prototype (its vector type is left incomplete here);
 * the callers below pass a float array. */
struct DotrObjectVector4;
extern int SzEffect_GetModelScale2(int packed, struct DotrObjectVector4 *v);
/* Scale steps for a standing-by model: MODEL_OFF lowers each component by 0.18 (floor 0) when
 * its fourth argument is set, MODEL_SCL_ON_1 raises by step (ceiling 1), MODEL_SCL_OFF_1 lowers
 * by step (floor 0). All return 0; the unused parameters are assumed int. */
int MODEL_OFF(int model, int unused_1, int unused_2, int on);
int MODEL_SCL_ON_1(int model, float step);
int MODEL_SCL_OFF_1(int model, float step);
int MODEL_ON(int model, int unused, int on);

/* 0x106c50: off == 0 adds amount to the byte at 3 while it is below limit, otherwise subtracts
 * while it is above limit; returns 1 when the limit was already reached, else 0. */
int OBJECT_RGBA_INOFF(unsigned char *rgba, int amount, unsigned char limit, int off);

/* 0x107740: four colour bytes followed by 1.0f, copied out as two floats (the byte word is
 * moved through a float register, not converted). Both layouts are assumptions. */
struct DotrObjRgbaOne {
    unsigned char rgba[4];
    float one;
};
struct DotrObjFloatPair {
    float unknown_0;
    float unknown_4;
};
void OBJ_GsRGB_RGB_IN2(struct DotrObjFloatPair *out, unsigned char *rgba);

/* 0x107280: fades a standing-by card's fourth colour byte by 19 (to 0 at 20 or below) and
 * returns 1 once it is 0. The callees are declared in packed_item.h with a four-byte
 * structure pointer; a byte pointer is used here (same object, assumed). */
/* 0x107650: assumed (card, alpha step); alpha saturates at 128. */
int CARD_ON(int card, int step);
extern int SzEffect_IsCardStandby2(int card);
extern int SzEffect_GetCardRgba2(int card, unsigned char *rgba);
extern int SzEffect_SetCardRgba2(int card, unsigned char *rgba);
int CARD_OFF(int card);
/* 0x108be0: rotates *in about one axis (0: y, 1: x, 2: z) by the sine/cosine pair given and writes
 * the result to *out; the w member and, for any other axis, the whole result are copied from an
 * unset local. Assumed: explicit out pointer, int axis compared as short for 1 and 2. */
void RAD_XYZ_IN(struct DotrObjVector *out, struct DotrObjVector *in, int axis, float s, float c);
/* 0x1098e0: foot of the perpendicular from (0, point->y, 0) onto the line through from and to
 * (both shifted by offset's x and z); writes x and z, y = 0. A zero-length line writes x = 0 only.
 * Parameter roles assumed from the code. */
void ATARI3(struct DotrObjVector *out, struct DotrObjVector *offset, struct DotrObjVector *point,
            struct DotrObjVector *from, struct DotrObjVector *to);
/* 0x107ec0: writes one of ten fixed colours (kind 0-9, any other kind white) to rgb[0..2] and
 * returns 0; the second argument is not read. Assumed: byte pointer, short kind. */
int OBJ_GsRGB_RGB_IN_DATA(unsigned char *rgb, int unused, short kind);
/* 0x108d60: for mode 0-3 writes x to v[0] and v[8], y to v[1] and v[5] and zero to
 * v[2], v[4], v[6], v[9], v[10], v[12..14], where (x, y) is (-w, h), (w, h), (-w, -h), (w, -h);
 * any other mode writes nothing. Returns 0. Assumed: float block, short parameters. */
int PAINT_VECTOR_4TG_SET_INMAP4(float *v, short w, short h, short mode);
/* 0x107790 / 0x107990: write the colour bytes and 1.0f (the body of OBJ_GsRGB_RGB_IN2, which the
 * original expands in place eight times with a separate stack temporary each) to four pairs of
 * p for mode 0, the first two for mode 1, the last two for mode 2; other modes write nothing.
 * Pairs 2, 5, 8, 11 (GT4) and 2, 4, 6, 8 (G4). Returns 0. Assumed: p as an array of float
 * pairs, int mode; the inline helper stands for the same-file inlining of 0x107740. */
static inline void OBJ_GsRGB_RGB_IN2_inline(struct DotrObjFloatPair *out, unsigned char *rgba)
{
    struct DotrObjRgbaOne tmp;

    tmp.rgba[0] = rgba[0];
    tmp.rgba[1] = rgba[1];
    tmp.rgba[2] = rgba[2];
    tmp.rgba[3] = rgba[3];
    tmp.one = 1.0f;
    *out = *(struct DotrObjFloatPair *)&tmp;
}
int GT4_IN_RGBA(struct DotrObjFloatPair *p, unsigned char *rgba, int mode);
int G4_IN_RGBA(struct DotrObjFloatPair *p, unsigned char *rgba, int mode);

/* 0x105d00: four 16-bit values (named u, v, w, h here from the values, an assumption) in a
 * 16-byte local whose last eight bytes are never written; the whole local is copied out. */
struct DotrUvRect {
    short u;
    short v;
    short w;
    short h;
    int pad[2];
};
/* Selects a fixed rectangle by kind 0-6: kind 1 takes v = sub << 4, kinds 2 and 4 also pick by
 * sub; kind 4 with sub outside 0-3, or any other kind, gives 0,0,96,96. */
void RH2_DATA_IN(struct DotrObjVector *out, short kind, short sub);

#endif

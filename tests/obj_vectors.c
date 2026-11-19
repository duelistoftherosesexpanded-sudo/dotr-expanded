/* Synthetic host tests for the obj_vectors group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/obj_vectors.h"

int func_001a22c0(float value)
{
    return (int)value;
}

static int trig_args[2][3];
static int trig_calls;

float SIN_COS_INRAD(int angle, int unit, int cosine)
{
    trig_args[trig_calls][0] = angle;
    trig_args[trig_calls][1] = unit;
    trig_args[trig_calls][2] = cosine;
    trig_calls++;
    return cosine ? 0.5f : 0.25f;
}

static void check_polar(void)
{
    struct DotrObjVector v = { 9.0f, 9.0f, 9.0f, 9.0f };
    union { short h[0xb0 / 2]; float f[0xb0 / 4]; } object;

    assert(XC_ZS_IN(&v, 5, 10, 7) == 0);
    assert(trig_calls == 2 && trig_args[0][0] == 7 && trig_args[0][1] == 5 && trig_args[0][2] == 1);
    assert(trig_args[1][0] == 7 && trig_args[1][1] == 5 && trig_args[1][2] == 0);
    assert(v.x == 5.0f && v.y == 9.0f && v.z == 2.5f && v.w == 9.0f);

    memset(&object, 0x55, sizeof(object));
    assert(OBJ_RR2H_IN(1.5f, -2.5f, 7.0f, &object) == 0);
    assert(object.h[0xa8 / 2] == 1 && object.h[0xaa / 2] == -2 && object.f[0xa0 / 4] == 7.0f);
    assert(object.h[0xa6 / 2] == 0x5555 && object.h[0xac / 2] == 0x5555);
}

static void *clear_target;
static int clear_size;

void func_00195108(void *a, int b)
{
    clear_target = a;
    clear_size = b;
}

static void check_small(void)
{
    struct DotrObjVector a = { 1.0f, 2.0f, 3.0f, 9.0f };
    struct DotrObjVector b = { 10.0f, 20.0f, 30.0f, 40.0f };
    static float system[0x44b0 / 4];
    unsigned char rgb[5] = { 0x55, 0x55, 0x55, 0x55, 0x55 };
    unsigned char src[4] = { 1, 2, 3, 4 };

    POS_POS(&a, &b);
    assert(a.x == 11.0f && a.y == 22.0f && a.z == 33.0f && a.w == 9.0f);
    OBJ_VE_SCL_KAKE(&a, &b);
    assert(a.x == 110.0f && a.y == 440.0f && a.z == 990.0f && a.w == 9.0f);
    OBJ_SCL2(&a, 5.0f, 6.0f, 7.0f);
    assert(a.x == 5.0f && a.y == 6.0f && a.z == 7.0f && a.w == 9.0f);

    assert(POINT_IN_VECTOR_(&a, &b, 0) == 0);
    assert(a.x == 10.0f && a.y == 20.0f && a.z == 30.0f && a.w == 9.0f && b.x == 10.0f);
    OBJ_SCL2(&a, 5.0f, 6.0f, 7.0f);
    assert(POINT_IN_VECTOR_(&a, &b, 1) == 0);
    assert(b.x == 5.0f && b.y == 6.0f && b.z == 7.0f && b.w == 40.0f && a.x == 5.0f);

    assert(OBJ_ADDRESS_IN(&a, 0x40) == &a);
    assert(clear_target == &a && clear_size == 0x40);
    assert(OBJ_ADDRESS_IN(&b, -2) == &b);
    assert(clear_target == &b && clear_size == -2);

    assert(OBJ_RGB_IN(rgb, 7, 8, 9) == 0);
    assert(rgb[0] == 7 && rgb[1] == 8 && rgb[2] == 9 && rgb[3] == 0x55);
    assert(OBJ_GsRGB_RGB_IN(rgb, src) == 0);
    assert(rgb[0] == 1 && rgb[1] == 2 && rgb[2] == 3 && rgb[3] == 4 && rgb[4] == 0x55);

    system[0x4498 / 4] = 77.0f;
    system[0x44a8 / 4] = 77.0f;
    assert(PSYS_RR2H_IN(system, 1.0f, 2.0f, 3.0f) == 0);
    assert(system[0x449c / 4] == 1.0f && system[0x44a0 / 4] == 2.0f && system[0x44a4 / 4] == 3.0f);
    assert(system[0x4498 / 4] == 77.0f && system[0x44a8 / 4] == 77.0f);
}

static void check_convert(void)
{
    short halves[9];
    float source[4] = { 1.5f, -2.5f, 300.0f, 4.0f };
    int words[4];
    int i;

    for (i = 0; i < 9; i++)
        halves[i] = 0x5555;
    PV_IN_(halves, source, 0);
    assert(halves[0] == 1 && halves[1] == -2 && halves[2] == 300 && halves[3] == 4);
    assert(halves[4] == 0x5555 && halves[7] == 0x5555);
    for (i = 0; i < 9; i++)
        halves[i] = 0x5555;
    PV_IN_(halves, source, 1);
    assert(halves[4] == 1 && halves[5] == -2 && halves[6] == 300 && halves[7] == 4);
    assert(halves[0] == 0x5555 && halves[3] == 0x5555 && halves[8] == 0x5555);

    words[3] = 0x55;
    OBJ_ROT(7.0f, -8.0f, 70000.0f, words);
    assert(words[0] == 7 && words[1] == -8 && words[2] == 70000 && words[3] == 0x55);
    OBJ_I_V_IN(words, source);
    assert(words[0] == 1 && words[1] == -2 && words[2] == 300 && words[3] == 0x55);
}

float func_001a6c60(float value)
{
    return value + 1.0f;
}

float func_001a6a88(float value)
{
    return value * 2.0f;
}

static int standby_result, standby_arg, scale_calls, scale_arg;
static float scale_seen[3];

int SzEffect_IsModelStandby2(int model)
{
    standby_arg = model;
    return standby_result;
}

static float scale_now[4];
static int scale_get_arg, scale_get_calls;

int SzEffect_GetModelScale2(int packed, struct DotrObjectVector4 *v)
{
    scale_get_arg = packed;
    scale_get_calls++;
    memcpy(v, scale_now, sizeof(scale_now));
    return 0;
}

static void set_scale(float x, float y, float z)
{
    scale_now[0] = x, scale_now[1] = y, scale_now[2] = z, scale_now[3] = 7.0f;
    scale_calls = 0;
    scale_get_calls = 0;
}

static void check_model_scale_steps(void)
{
    /* Not standing by: nothing is read or written. */
    standby_result = 0;
    set_scale(0.5f, 0.5f, 0.5f);
    assert(MODEL_SCL_ON_1(31, 0.25f) == 0 && standby_arg == 31);
    assert(MODEL_SCL_OFF_1(32, 0.25f) == 0 && standby_arg == 32);
    assert(MODEL_OFF(33, 98, 99, 1) == 0 && standby_arg == 33);
    assert(scale_calls == 0 && scale_get_calls == 0);

    standby_result = 1;
    /* x stays below 1, y lands exactly on 1 (kept), z passes 1 and is clamped. */
    set_scale(0.5f, 0.75f, 0.875f);
    assert(MODEL_SCL_ON_1(34, 0.25f) == 0);
    assert(scale_get_arg == 34 && scale_arg == 34 && scale_calls == 1 && scale_get_calls == 1);
    assert(scale_seen[0] == 0.75f && scale_seen[1] == 1.0f && scale_seen[2] == 1.0f);

    /* x stays above 0, y lands exactly on 0 (kept), z passes 0 and is clamped. */
    set_scale(0.5f, 0.25f, 0.125f);
    assert(MODEL_SCL_OFF_1(35, 0.25f) == 0);
    assert(scale_get_arg == 35 && scale_arg == 35 && scale_calls == 1 && scale_get_calls == 1);
    assert(scale_seen[0] == 0.25f && scale_seen[1] == 0.0f && scale_seen[2] == 0.0f);

    /* MODEL_OFF needs its fourth argument; then it steps by 0.18 with a floor of 0. */
    set_scale(1.0f, 0.18f, 0.125f);
    assert(MODEL_OFF(36, 98, 99, 0) == 0 && standby_arg == 36);
    assert(scale_calls == 0 && scale_get_calls == 0);
    assert(MODEL_OFF(37, 98, 99, 1) == 0);
    assert(scale_get_arg == 37 && scale_arg == 37 && scale_calls == 1 && scale_get_calls == 1);
    assert(scale_seen[0] == 1.0f - 0.18f && scale_seen[1] == 0.0f && scale_seen[2] == 0.0f);
}

int SzEffect_SetModelScale2(int model, float *scale)
{
    scale_arg = model;
    memcpy(scale_seen, scale, sizeof(scale_seen));
    scale_calls++;
    return 0;
}

/* Branch coverage: one assertion per branch outcome and boundary. */
static void check_object_bytes(void)
{
    unsigned char object[0xb4];
    unsigned char rgb[4] = { 10, 20, 30, 40 };
    short angle;
    struct DotrObjVector v[4];
    int axis, i;

    memset(object, 0, sizeof(object));
    object[0xaf] = 50;
    object[0xb3] = 60;
    assert(OBJECT_RGB_A_OFF(object, 10, 41) == 1 && object[0xaf] == 40 && object[0xb3] == 50);
    assert(OBJECT_RGB_A_OFF(object, 10, 30) == 0 && object[0xaf] == 30 && object[0xb3] == 40);
    assert(OBJECT_RGB_A_OFF(object, 10, 20) == 0 && object[0xaf] == 20);
    object[0xb3] = 30;
    assert(OBJECT_RGB_A_ON(object, 10, 29) == 1 && object[0xaf] == 30 && object[0xb3] == 40);
    assert(OBJECT_RGB_A_ON(object, 10, 40) == 0 && object[0xaf] == 40 && object[0xb3] == 50);
    assert(OBJECT_RGB_A_ON(object, 10, 60) == 0 && object[0xaf] == 50);

    assert(OBJECT_RGB_INOFF(rgb, 5, 99, 0) == 0);
    assert(rgb[0] == 15 && rgb[1] == 25 && rgb[2] == 35 && rgb[3] == 40);
    assert(OBJECT_RGB_INOFF(rgb, 7, 99, 1) == 0);
    assert(rgb[0] == 8 && rgb[1] == 18 && rgb[2] == 28 && rgb[3] == 40);

    angle = 100;
    assert(RAD_IN_360_(259, &angle) == 0 && angle == 359);
    assert(RAD_IN_360_(1, &angle) == 0 && angle == 0);
    angle = 300;
    assert(RAD_IN_360_(425, &angle) == 0 && angle == 5);
    angle = -300;
    assert(RAD_IN_360_(-59, &angle) == 0 && angle == -359);
    assert(RAD_IN_360_(-1, &angle) == 0 && angle == 0);
    angle = -300;
    assert(RAD_IN_360_(-70, &angle) == 0 && angle == -10);

    for (axis = 0; axis < 4; axis++) {
        long arg = axis == 3 ? 0x10001 : axis;
        int moved = axis == 3 ? 1 : axis;

        for (i = 0; i < 4; i++) {
            v[i].x = 1.0f;
            v[i].y = 2.0f;
            v[i].z = 3.0f;
            v[i].w = 4.0f;
        }
        assert(XYZ_POS_4(v, arg, 0.5f) == 0);
        for (i = 0; i < 4; i++) {
            assert(v[i].x == (moved == 0 ? 1.5f : 1.0f));
            assert(v[i].y == (moved == 1 ? 2.5f : 2.0f));
            assert(v[i].z == (moved == 2 ? 3.5f : 3.0f));
            assert(v[i].w == 4.0f);
        }
    }

    for (i = 0; i < 3; i++) {
        standby_result = i != 0;
        scale_calls = 0;
        assert(MODEL_ON(21 + i, 99, i == 2) == 0 && standby_arg == 21 + i);
        assert(scale_calls == (i == 2));
    }
    assert(scale_arg == 23 && scale_seen[0] == 1.0f && scale_seen[1] == 1.0f && scale_seen[2] == 1.0f);
}

/* Straight-line coverage for CARD_X_Z_IN_; loop bounds (first, last, one past) for SIN_IN_PSYS. */
static void check_card_and_sines(void)
{
    struct DotrObjVector pair[2] = { { 1.0f, 9.0f, 2.0f, 9.0f }, { 3.0f, 9.0f, 4.0f, 9.0f } };
    struct DotrObjVector point = { 6.0f, 9.0f, 9.0f, 9.0f };
    float dx = 0.0f, dz = 0.0f;
    float sines[92];

    CARD_X_Z_IN_(&dx, &dz, pair, &point);
    assert(dx == 5.0f && dz == 10.0f);

    sines[91] = -7.0f;
    SIN_IN_PSYS(sines);
    assert(sines[0] == 0.0f);
    assert(sines[1] == func_001a6a88(1.0f * 3.1415927f / 180.0f));
    assert(sines[90] == func_001a6a88(90.0f * 3.1415927f / 180.0f));
    assert(sines[91] == -7.0f);
}

static int card_standby;
static int card_arg;
static int card_set_calls;
static unsigned char card_rgba[4];

int SzEffect_IsCardStandby2(int card)
{
    card_arg = card;
    return card_standby;
}

int SzEffect_GetCardRgba2(int card, unsigned char *rgba)
{
    assert(card == card_arg);
    memcpy(rgba, card_rgba, 4);
    return 0;
}

int SzEffect_SetCardRgba2(int card, unsigned char *rgba)
{
    assert(card == card_arg);
    memcpy(card_rgba, rgba, 4);
    card_set_calls++;
    return 0;
}

/* Branch coverage for OBJECT_RGBA_INOFF and CARD_OFF; OBJ_GsRGB_RGB_IN2 is straight-line. */
static void check_rgba_and_card(void)
{
    unsigned char rgba[4] = { 1, 2, 3, 40 };
    unsigned char bytes[4] = { 0x11, 0x22, 0x33, 0x44 };
    struct DotrObjFloatPair pair;

    assert(OBJECT_RGBA_INOFF(rgba, 5, 41, 0) == 0 && rgba[3] == 45);
    assert(OBJECT_RGBA_INOFF(rgba, 5, 45, 0) == 1 && rgba[3] == 45);
    assert(OBJECT_RGBA_INOFF(rgba, 5, 44, 1) == 0 && rgba[3] == 40);
    assert(OBJECT_RGBA_INOFF(rgba, 5, 40, 1) == 1 && rgba[3] == 40);
    assert(rgba[0] == 1 && rgba[1] == 2 && rgba[2] == 3);

    OBJ_GsRGB_RGB_IN2(&pair, bytes);
    assert(memcmp(&pair.unknown_0, bytes, 4) == 0 && pair.unknown_4 == 1.0f);

    card_standby = 0;
    card_set_calls = 0;
    assert(CARD_OFF(7) == 0 && card_arg == 7 && card_set_calls == 0);
    card_standby = 1;
    card_rgba[3] = 21;
    assert(CARD_OFF(8) == 0 && card_rgba[3] == 2 && card_set_calls == 1 && card_arg == 8);
    card_rgba[3] = 20;
    assert(CARD_OFF(9) == 1 && card_rgba[3] == 0 && card_set_calls == 2);
    card_rgba[0] = 5;
    card_rgba[3] = 0;
    assert(CARD_OFF(9) == 1 && card_rgba[0] == 5 && card_rgba[3] == 0);

    /* CARD_ON (branch coverage): not standing by, below, at and above the 128 ceiling */
    card_standby = 0;
    card_set_calls = 0;
    card_rgba[3] = 10;
    assert(CARD_ON(7, 5) == 0 && card_arg == 7 && card_set_calls == 0 && card_rgba[3] == 10);
    card_standby = 1;
    assert(CARD_ON(8, 117) == 0 && card_rgba[3] == 127 && card_set_calls == 1 && card_arg == 8);
    assert(CARD_ON(8, 1) == 1 && card_rgba[3] == 128 && card_set_calls == 2);
    card_rgba[3] = 120;
    assert(CARD_ON(9, 20) == 1 && card_rgba[3] == 128 && card_rgba[0] == 5);
}

float func_001a6898(float value)
{
    return value * 3.0f + 1.0f;
}

/* SIN_IN_PSYS2 (branch coverage): count 0 writes nothing; count 2 writes entries 0 and 1 of
 * each block and leaves entry 2. */
static void check_sines2(void)
{
    struct DotrSineBlocks b;
    float r1 = 30.0f * 3.1415927f / 180.0f, r2 = 60.0f * 3.1415927f / 180.0f;
    int i;

    for (i = 0; i < 20; i++) {
        b.sin_at[i] = b.cos_at[i] = b.sin_next[i] = b.cos_next[i] = -7.0f;
    }
    SIN_IN_PSYS2(&b, 30, 0);
    assert(b.sin_at[0] == -7.0f && b.cos_at[0] == -7.0f && b.sin_next[0] == -7.0f && b.cos_next[0] == -7.0f);
    SIN_IN_PSYS2(&b, 30, 2);
    assert(b.sin_at[0] == 0.0f && b.cos_at[0] == 1.0f);
    assert(b.sin_next[0] == func_001a6a88(r1) && b.cos_next[0] == func_001a6898(r1));
    assert(b.sin_at[1] == func_001a6a88(r1) && b.cos_at[1] == func_001a6898(r1));
    assert(b.sin_next[1] == func_001a6a88(r2) && b.cos_next[1] == func_001a6898(r2));
    assert(b.sin_at[2] == -7.0f && b.cos_at[2] == -7.0f && b.sin_next[2] == -7.0f && b.cos_next[2] == -7.0f);
}

static void check_colour_table(void)
{
    static const unsigned char want[11][3] = {
        {255, 255, 255}, {255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {255, 255, 0}, {255, 0, 255},
        {0, 255, 255}, {255, 128, 255}, {128, 0, 255}, {255, 128, 0}, {255, 255, 255},
    };
    unsigned char rgb[4];
    int k;

    for (k = 0; k < 11; k++) {
        rgb[0] = rgb[1] = rgb[2] = 7;
        rgb[3] = 9;
        assert(OBJ_GsRGB_RGB_IN_DATA(rgb, 77, (short)k) == 0);
        assert(rgb[0] == want[k][0] && rgb[1] == want[k][1] && rgb[2] == want[k][2] && rgb[3] == 9);
    }
    assert(OBJ_GsRGB_RGB_IN_DATA(rgb, 0, -1) == 0);
    assert(rgb[0] == 255 && rgb[1] == 255 && rgb[2] == 255);
}

static void check_rotate_and_foot(void)
{
    struct DotrObjVector in = {1.0f, 2.0f, 3.0f, 9.0f}, out, off, p, a, b;

    RAD_XYZ_IN(&out, &in, 0, 0.5f, 2.0f);
    assert(out.x == 0.5f * 1.0f + -2.0f * 3.0f && out.y == 2.0f && out.z == 2.0f * 1.0f + 0.5f * 3.0f);
    RAD_XYZ_IN(&out, &in, 1, 0.5f, 2.0f);
    assert(out.x == 1.0f && out.y == 2.0f * 2.0f + 0.5f * 3.0f && out.z == -0.5f * 2.0f + 2.0f * 3.0f);
    RAD_XYZ_IN(&out, &in, 2, 0.5f, 2.0f);
    assert(out.x == 0.5f * 1.0f + 2.0f * 2.0f && out.y == -2.0f * 1.0f + 0.5f * 2.0f && out.z == 3.0f);
    /* Only the low 16 bits select axes 1 and 2. */
    RAD_XYZ_IN(&out, &in, 0x10002, 0.5f, 2.0f);
    assert(out.x == 4.5f && out.y == -1.0f && out.z == 3.0f);

    off.x = 1.0f; off.y = 50.0f; off.z = 2.0f; off.w = 0.0f;
    p.x = 70.0f; p.y = 3.0f; p.z = 80.0f; p.w = 0.0f;
    a.x = -1.0f; a.y = 1.0f; a.z = -2.0f; a.w = 0.0f;
    b.x = 3.0f; b.y = 1.0f; b.z = -2.0f; b.w = 0.0f;
    out.x = out.y = out.z = -7.0f;
    ATARI3(&out, &off, &p, &a, &b);
    /* Line from (0, 1, 0) to (4, 1, 0): the foot of (0, 3, 0) is the origin. */
    assert(out.x == 0.0f && out.y == 0.0f && out.z == 0.0f);
    a.x = 1.0f; a.z = 0.0f; b.x = 1.0f; b.z = 2.0f;
    ATARI3(&out, &off, &p, &a, &b);
    /* Line from (2, 1, 2) to (2, 1, 4): t = -1, foot (2, 0, 0). */
    assert(out.x == 2.0f && out.y == 0.0f && out.z == 0.0f);
    out.x = -7.0f;
    ATARI3(&out, &off, &p, &a, &a);
    assert(out.x == 0.0f);
}

static void check_inmap4(void)
{
    static const float want[4][2] = {{-3.0f, 5.0f}, {3.0f, 5.0f}, {-3.0f, -5.0f}, {3.0f, -5.0f}};
    float v[16];
    int mode, i;

    for (mode = 0; mode < 5; mode++) {
        for (i = 0; i < 16; i++) {
            v[i] = 7.0f;
        }
        assert(PAINT_VECTOR_4TG_SET_INMAP4(v, 3, 5, (short)mode) == 0);
        assert(v[3] == 7.0f && v[7] == 7.0f && v[11] == 7.0f && v[15] == 7.0f);
        if (mode == 4) {
            for (i = 0; i < 16; i++) {
                assert(v[i] == 7.0f);
            }
            continue;
        }
        assert(v[0] == want[mode][0] && v[8] == want[mode][0]);
        assert(v[1] == want[mode][1] && v[5] == want[mode][1]);
        assert(v[2] == 0.0f && v[4] == 0.0f && v[6] == 0.0f && v[9] == 0.0f && v[10] == 0.0f);
        assert(v[12] == 0.0f && v[13] == 0.0f && v[14] == 0.0f);
    }
}

static void check_in_rgba(void)
{
    static const int idx[2][4] = {{2, 5, 8, 11}, {2, 4, 6, 8}};
    static const int from[4] = {0, 0, 2, 4}, to[4] = {4, 2, 4, 4};
    unsigned char rgba[4] = {1, 2, 3, 4};
    struct DotrObjFloatPair p[12], want[12];
    struct DotrObjRgbaOne one;
    int f, mode, i;

    memcpy(one.rgba, rgba, 4);
    one.one = 1.0f;
    for (f = 0; f < 2; f++) {
        for (mode = 0; mode < 4; mode++) {
            memset(p, 0x55, sizeof(p));
            memset(want, 0x55, sizeof(want));
            for (i = from[mode]; i < to[mode]; i++) {
                memcpy(&want[idx[f][i]], &one, sizeof(one));
            }
            assert((f == 0 ? GT4_IN_RGBA : G4_IN_RGBA)(p, rgba, mode) == 0);
            assert(memcmp(p, want, sizeof(p)) == 0);
        }
    }
}

static void check_rh2_data_in(void)
{
    /* One row per branch outcome: kind, sub, then the four values written. */
    static const short rows[][6] = {
        {0, 9, 64, 0, 63, 63},    {1, 3, 240, 48, 16, 16},  {2, 0, 143, 0, 31, 64},
        {2, 1, 143, 63, 31, 64},  {3, 9, 128, 128, 127, 63}, {4, 0, 128, 128, 63, 32},
        {4, 1, 128, 160, 63, 32}, {4, 2, 192, 128, 63, 32}, {4, 3, 192, 160, 63, 32},
        {4, 4, 0, 0, 96, 96},     {5, 9, 143, 0, 31, 127},  {6, 9, 176, 0, 59, 31},
        {7, 0, 0, 0, 96, 96},     {-1, 0, 0, 0, 96, 96},
    };
    struct DotrObjVector out[2];
    unsigned i;

    for (i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
        memset(out, 0x55, sizeof(out));
        RH2_DATA_IN(&out[0], rows[i][0], rows[i][1]);
        /* Only the first eight bytes are defined; the rest of the copy is the unwritten local. */
        assert(memcmp(&out[0], &rows[i][2], 8) == 0);
        assert(out[1].x == out[1].w);
    }
}

int main(void)
{
    struct DotrObjVector v[4];
    struct DotrObjVector to = { 10.0f, 20.0f, 30.0f, 40.0f };
    struct DotrObjVector from = { 4.0f, 5.0f, 6.0f, 7.0f };
    struct DotrObjVector scale = { 2.0f, 4.0f, 8.0f, 16.0f };
    int i;

    assert(sizeof(struct DotrObjVector) == 16);
    check_convert();
    check_small();
    check_polar();

    for (i = 0; i < 4; i++) {
        v[i].x = (float)(i + 1);
        v[i].y = (float)(i + 2);
        v[i].z = (float)(i + 3);
        v[i].w = 9.0f;
    }
    OBJ_VE_SCL_KAKE_4(v, &scale);
    for (i = 0; i < 4; i++) {
        assert(v[i].x == (float)(i + 1) * 2.0f && v[i].y == (float)(i + 2) * 4.0f);
        assert(v[i].z == (float)(i + 3) * 8.0f && v[i].w == 9.0f);
    }

    /* POS_POS_4 (straight-line coverage): every stored component, w untouched */
    for (i = 0; i < 4; i++) {
        v[i].x = (float)(i + 1);
        v[i].y = (float)(i + 2);
        v[i].z = (float)(i + 3);
        v[i].w = 9.0f;
    }
    POS_POS_4(v, &scale);
    for (i = 0; i < 4; i++) {
        assert(v[i].x == (float)(i + 1) + 2.0f && v[i].y == (float)(i + 2) + 4.0f);
        assert(v[i].z == (float)(i + 3) + 8.0f && v[i].w == 9.0f);
    }

    for (i = 0; i < 4; i++) {
        v[i].x = 1.0f;
        v[i].y = 2.0f;
        v[i].z = 3.0f;
        v[i].w = 9.0f;
    }
    assert(V0IN_V1_V2_XZ(v, &to, &from) == 0);
    for (i = 0; i < 2; i++)
        assert(v[i].x == 7.0f && v[i].y == 2.0f && v[i].z == 27.0f && v[i].w == 9.0f);
    assert(v[2].x == 1.0f && v[2].z == 3.0f);

    for (i = 0; i < 4; i++)
        v[i].w = 9.0f;
    assert(PAINT_VECTOR_4TG_SET_IN(v, 3.0f, 5.0f) == 0);
    assert(v[0].x == -3.0f && v[0].y == 5.0f && v[0].z == 0.0f && v[0].w == 9.0f);
    assert(v[1].x == 3.0f && v[1].y == 5.0f && v[1].z == 0.0f && v[1].w == 9.0f);
    assert(v[2].x == -3.0f && v[2].y == -5.0f && v[2].z == 0.0f && v[2].w == 9.0f);
    assert(v[3].x == 3.0f && v[3].y == -5.0f && v[3].z == 0.0f && v[3].w == 9.0f);
    assert(PAINT_VECTOR_4TG_SET_IN2(v, 3.0f, 5.0f) == 0);
    assert(v[0].x == -3.0f && v[0].y == 0.0f && v[0].z == 5.0f && v[0].w == 9.0f);
    assert(v[1].x == 3.0f && v[1].y == 0.0f && v[1].z == 5.0f && v[1].w == 9.0f);
    assert(v[2].x == -3.0f && v[2].y == 0.0f && v[2].z == -5.0f && v[2].w == 9.0f);
    assert(v[3].x == 3.0f && v[3].y == 0.0f && v[3].z == -5.0f && v[3].w == 9.0f);

    check_object_bytes();
    check_card_and_sines();
    check_rgba_and_card();
    check_model_scale_steps();
    puts("obj_vectors: ok");
    check_sines2();
    check_rotate_and_foot();
    check_colour_table();
    check_inmap4();
    check_in_rgba();
    check_rh2_data_in();
    return 0;
}

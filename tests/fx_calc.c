/* Synthetic host tests for the fx_calc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/draw_state.h"
#include "dotr/ee/fx_calc.h"
#include "dotr/ee/view_state.h"

static int distance_index;
static int black_calls;
static int black_values[4];

float SzView_GetScreenDistance(int index)
{
    distance_index = index;
    return 1600.0f;
}

void SzEffect_SetBlackOutForBattle(int value)
{
    black_values[black_calls++ & 3] = value;
}

static void check_black(void)
{
    struct DotrFxBlackSpan span = { 10, 100 };

    black_calls = 0;
    ShFx_BlackBG(&span, 9); /* step 0 for the first, far out of range for the second */
    assert(black_calls == 0);
    ShFx_BlackBG(&span, 10);
    assert(black_calls == 1 && black_values[0] == 0x70);
    ShFx_BlackBG(&span, 14); /* step 5: last one inside */
    assert(black_calls == 2 && black_values[1] == 0x30);
    ShFx_BlackBG(&span, 15); /* step 6 */
    assert(black_calls == 2);
    ShFx_BlackBG(&span, 8); /* step 0xffff */
    assert(black_calls == 2);
    black_calls = 0;
    ShFx_BlackBG(&span, 99);
    assert(black_calls == 0);
    ShFx_BlackBG(&span, 100);
    assert(black_calls == 1 && black_values[0] == 0x40);
    ShFx_BlackBG(&span, 104);
    assert(black_calls == 2 && black_values[1] == 0x80);
    ShFx_BlackBG(&span, 105);
    assert(black_calls == 2);
    span.lighten_frame = 12; /* both ranges at once: darken first, then lighten */
    black_calls = 0;
    ShFx_BlackBG(&span, 12);
    assert(black_calls == 2 && black_values[0] == 0x50 && black_values[1] == 0x40);
}

static int random_next;

int func_0019a5c8(void)
{
    random_next += 0x111;
    return random_next;
}

void func_00187b00(float *out, float *a, float *b)
{
    int i;
    for (i = 0; i < 4; i++)
        out[i] = a[i] + b[i];
}

void func_00187b18(float *out, float *a, float *b)
{
    int i;
    for (i = 0; i < 4; i++)
        out[i] = a[i] - b[i];
}

void func_00187b48(float *out, float *in, float scale)
{
    int i;
    for (i = 0; i < 4; i++)
        out[i] = in[i] * scale;
}

static void check_random_and_center(void)
{
    unsigned char bytes[4] = { 0x55, 0x55, 0x55, 0x55 };
    float a[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float b[4] = { 8.0f, 0.0f, 4.0f, 0.0f };
    float c[4] = { 1.0f, 12.0f, 2.0f, 0.0f };
    float out[4];

    random_next = 0;
    ShFx_MakeRnd(bytes, 0); /* zero count: nothing drawn, nothing stored */
    assert(random_next == 0 && bytes[0] == 0x55);
    ShFx_MakeRnd(bytes, 3);
    assert(bytes[0] == 0x11 && bytes[1] == 0x22 && bytes[2] == 0x33 && bytes[3] == 0x55);
    assert(random_next == 0x333);

    /* middle = (4, 0, 2); third = (c - middle) * 0.33333334f; out = third + middle. */
    GGet_TriangleCenter(out, a, b, c);
    assert(out[0] == (1.0f - 4.0f) * 0.33333334f + 4.0f && out[1] == 12.0f * 0.33333334f + 0.0f);
    assert(out[2] == (2.0f - 2.0f) * 0.33333334f + 2.0f && out[3] == 0.0f);
}

static float model_z;
static int model_arg;

int SzEffect_GetModelPos2(int model, float *pos)
{
    model_arg = model;
    pos[0] = -1.0f;
    pos[1] = -1.0f;
    pos[2] = model_z;
    return 0;
}

/* Branch coverage: each clamp outcome and both boundaries; each sign outcome and zero. */
static void check_limits(void)
{
    assert(GWithin_LimitsI(1, 2, 8) == 2);
    assert(GWithin_LimitsI(2, 2, 8) == 2);
    assert(GWithin_LimitsI(5, 2, 8) == 5);
    assert(GWithin_LimitsI(8, 2, 8) == 8);
    assert(GWithin_LimitsI(9, 2, 8) == 8);
    model_z = -0.5f;
    assert(GGet_ModelPosZSign(11) == -1 && model_arg == 11);
    model_z = 0.0f;
    assert(GGet_ModelPosZSign(12) == 1 && model_arg == 12);
    model_z = 2.0f;
    assert(GGet_ModelPosZSign(13) == 1);
}

struct DotrFxAngles data_0028c210 = { 1, 2, 3, 4 };
static float *rot_out;
static int rot_words[4], rot_calls;

void RhGeom_RotXYZMatrix(float *out, int *ang)
{
    rot_out = out;
    memcpy(rot_words, ang, sizeof(rot_words));
    rot_calls++;
}

/* Straight-line coverage: the forwarded pointer, the copied words and the masked angle. */
static void check_rot_matrix(void)
{
    float m[16];

    GRot_Matrix(m, 0x12345, 1);
    assert(rot_calls == 1 && rot_out == m);
    assert(rot_words[0] == 1 && rot_words[1] == 0x2345 && rot_words[2] == 3 && rot_words[3] == 4);
    GRot_Matrix(m, -1, 3);
    assert(rot_calls == 2 && rot_words[1] == 2 && rot_words[3] == 0xffff);
    assert(data_0028c210.x == 1 && data_0028c210.y == 2 && data_0028c210.z == 3 && data_0028c210.w == 4);
}

static void check_window_uv(void)
{
    static const int in_x[5] = { 1728, 1727, 1729, 1728 + 639, 1728 + 640 };
    static const int want_x[5] = { 1, 1, 1, 639, 639 };
    static const int in_y[5] = { 1824, 1823, 1825, 1824 + 447, 1824 + 448 };
    static const int want_y[5] = { 1, 1, 1, 447, 447 };
    unsigned short out[3];
    unsigned short xy[2];
    int i;

    assert(sizeof(struct DotrWindowUv) == 4);
    for (i = 0; i < 5; i++) {
        out[0] = 0xffff, out[1] = 0x4000, out[2] = 0x5555;
        xy[0] = (unsigned short)(in_x[i] << 4 | 9);
        xy[1] = (unsigned short)(in_y[i] << 4 | 9);
        GSet_WindowUv((struct DotrWindowUv *)out, xy, 0, 0);
        assert(out[0] == (0xc000 | want_x[i] << 4) && out[1] == (0x4000 | want_y[i] << 4));
        assert(out[2] == 0x5555);
    }
    /* Offsets are added before the clamp; a mid-range value passes through. */
    xy[0] = 1728 << 4, xy[1] = 1824 << 4;
    out[0] = out[1] = 0;
    GSet_WindowUv((struct DotrWindowUv *)out, xy, 300, 200);
    assert(out[0] == 300 << 4 && out[1] == 200 << 4);
    GSet_WindowUv((struct DotrWindowUv *)out, xy, 700, -5);
    assert(out[0] == 639 << 4 && out[1] == 1 << 4);
}

static void check_kind_tables(void)
{
    static const unsigned char alpha[6][4] = {
        { 9, 9, 9, 9 }, { 0, 1, 0, 1 }, { 0, 2, 0, 1 }, { 1, 0, 0, 1 }, { 2, 0, 0, 1 }, { 9, 9, 9, 9 }
    };
    static const float size[11][2] = {
        { 128.0f, 64.0f }, { 64.0f, 128.0f }, { 128.0f, 128.0f }, { 256.0f, 64.0f }, { 128.0f, 64.0f },
        { 128.0f, 128.0f }, { 32.0f, 128.0f }, { 128.0f, 64.0f }, { 128.0f, 128.0f }, { 64.0f, 64.0f },
        { 0.0f, 0.0f }
    };
    static const int mask[11] = { 7, 7, 15, 7, 1, 3, 0, 0, 0, 0, 0 };
    struct DotrFxAlphaSource source;
    unsigned char out[5];
    float width, height;
    int i;

    assert(offsetof(struct DotrFxAlphaSource, mode) == 5);
    for (i = 0; i < 6; i++) {
        memset(out, 9, sizeof(out));
        source.mode = (unsigned char)i;
        ShFx_GetAlpha(&source, out);
        assert(memcmp(out, alpha[i], 4) == 0 && out[4] == 9);
    }
    for (i = 0; i < 11; i++) {
        width = height = -1.0f;
        ShFx_GetTexWidth((unsigned int)i, &width, &height);
        assert(width == size[i][0] && height == size[i][1]);
        assert(ShFx_GetAnmID((unsigned int)i, 0x7fffffff) == mask[i]);
    }
    width = height = -1.0f;
    ShFx_GetTexWidth(0xffffffffu, &width, &height);
    assert(width == 0.0f && height == 0.0f);
    assert(ShFx_GetAnmID(0xffffffffu, -1) == 0);
    assert(ShFx_GetAnmID(2, 0x35) == 5);
}

static void check_uv_and_clip(void)
{
    static const int dims[10][4] = {
        {32, 32, 4, 2}, {32, 32, 2, 4}, {32, 32, 4, 4}, {32, 64, 8, 1}, {64, 64, 2, 1},
        {64, 64, 2, 2}, {32, 128, 1, 1}, {128, 64, 1, 1}, {128, 128, 1, 1}, {64, 64, 1, 1},
    };
    static const unsigned short lim[2][4] = {{22528, 43008, 25600, 39936}, {27648, 37888, 29184, 36352}};
    int out[6];
    unsigned short pos[2];
    int k, n, last;

    for (k = 0; k < 10; k++) {
        last = dims[k][2] * dims[k][3] - 1;
        out[0] = out[1] = -7;
        ShFx_SetUV(out, last, (unsigned int)k);
        assert(out[0] == -7 && out[1] == -7);
        assert(out[2] == (last % dims[k][2]) * dims[k][0] && out[3] == out[2] + dims[k][0]);
        assert(out[4] == (last / dims[k][2]) * dims[k][1] && out[5] == out[4] + dims[k][1]);
        ShFx_SetUV(out, last + 1, (unsigned int)k);
        assert(out[2] == 0 && out[3] == 0 && out[4] == 0 && out[5] == 0);
    }
    ShFx_SetUV(out, 0, 0);
    assert(out[2] == 0 && out[3] == 32 && out[4] == 0 && out[5] == 32);

    for (n = 0; n < 2; n++) {
        int (*clip)(unsigned short *) = n == 0 ? ShFx_isClip : ShFx_isClipSC;

        pos[0] = lim[n][0]; pos[1] = lim[n][2];
        assert(clip(pos) == 1);
        pos[0] = lim[n][1]; pos[1] = lim[n][3];
        assert(clip(pos) == 1);
        pos[0] = (unsigned short)(lim[n][0] - 1);
        assert(clip(pos) == 0);
        pos[0] = (unsigned short)(lim[n][1] + 1);
        assert(clip(pos) == 0);
        pos[0] = lim[n][0]; pos[1] = (unsigned short)(lim[n][2] - 1);
        assert(clip(pos) == 0);
        pos[1] = (unsigned short)(lim[n][3] + 1);
        assert(clip(pos) == 0);
    }
}

static int normal_n;
static struct DotrViewVector normal_pts[3];
static float normal_z;

int Ex3d_GetSurfaceNormal(int n, struct DotrViewVector *pts, struct DotrViewVector *out)
{
    normal_n = n;
    memcpy(normal_pts, pts, sizeof(normal_pts));
    out->z = normal_z;
    return 0;
}

static void check_front_back_and_spline(void)
{
    unsigned short a[2] = {1, 2}, b[2] = {40000, 4}, c[2] = {5, 65535};
    float x[4] = {0.0f, 1.0f, 2.0f, 3.0f};
    float y[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float z[4] = {9.0f, 9.0f, 9.0f, 9.0f};
    float z2[3] = {9.0f, 9.0f, 9.0f};
    float z1[2] = {9.0f, 9.0f};

    normal_z = -1.0f;
    assert(GJudge_FrontBack2(a, b, c) == 1);
    assert(normal_n == 3);
    assert(normal_pts[0].x == 1.0f && normal_pts[0].y == 2.0f && normal_pts[0].z == 0.0f && normal_pts[0].unknown_w == 1.0f);
    assert(normal_pts[1].x == 40000.0f && normal_pts[1].y == 4.0f && normal_pts[1].z == 0.0f && normal_pts[1].unknown_w == 1.0f);
    assert(normal_pts[2].x == 5.0f && normal_pts[2].y == 65535.0f && normal_pts[2].z == 0.0f && normal_pts[2].unknown_w == 1.0f);
    normal_z = 0.0f;
    assert(GJudge_FrontBack2(a, b, c) == 0);
    normal_z = 2.0f;
    assert(GJudge_FrontBack2(a, b, c) == 0);

    /* n = 4: both elimination and back-substitution loops run; 4*z1 + z2 = -2, z1 + 4*z2 = 2. */
    ShFx_Spline_MakeTable(4, x, y, z);
    assert(z[0] == 0.0f && z[3] == 0.0f);
    assert(z[1] > -0.667f && z[1] < -0.666f);
    assert(z[2] > 0.666f && z[2] < 0.667f);
    /* n = 3: the middle loop does not run; 4*z1 = -2. */
    ShFx_Spline_MakeTable(3, x, y, z2);
    assert(z2[0] == 0.0f && z2[1] == -0.5f && z2[2] == 0.0f);
    (void)z1;
}

/* SIN_COS_INRAD (branch coverage): both selectors, each quadrant at its lower and upper boundary, and the
 * three angle normalisations (>= 360, < -359, negative). The table holds its own index so the entry
 * is visible. */
static void check_sin_cos_inrad(void)
{
    static const struct { short deg; float sine, cosine; } rows[] = {
        { 0, 0.0f, 90.0f },      { 89, 89.0f, 1.0f },     { 90, 90.0f, -0.0f },    { 179, 1.0f, -89.0f },
        { 180, -0.0f, -90.0f },  { 269, -89.0f, -1.0f },  { 270, -90.0f, 0.0f },   { 359, -1.0f, 89.0f },
        { 360, 0.0f, 90.0f },    { 450, 90.0f, -0.0f },   { -1, -1.0f, 89.0f },    { -359, 1.0f, 89.0f },
        { -360, 0.0f, 90.0f },   { -450, -90.0f, 0.0f },  { 725, 5.0f, 85.0f },
    };
    float tbl[91];
    unsigned i;

    for (i = 0; i < 91; i++) {
        tbl[i] = (float)i;
    }
    for (i = 0; i < sizeof(rows) / sizeof(rows[0]); i++) {
        assert(SIN_COS_INRAD(tbl, rows[i].deg, 0) == rows[i].sine);
        assert(SIN_COS_INRAD(tbl, rows[i].deg, 1) == rows[i].cosine);
        assert(SIN_COS_INRAD(tbl, rows[i].deg, -5) == rows[i].cosine);
    }
}

/* ShFx_Spin2P callees: canned results, every argument recorded. */
static struct DotrViewVector spin_dir;
static float spin_scalar, spin_root_arg, spin_atan_args[2][2], spin_int_args[2];
static float spin_cos_args[8], spin_sin_args[8];
static struct DotrViewVector spin_in[8];
static void *spin_out[8];
static struct DotrViewMatrix *spin_matrix;
static int spin_atan_calls, spin_int_calls, spin_cos_calls, spin_sin_calls, spin_mul_calls;
static int spin_angles[3], spin_rot_calls;

static struct DotrFxPoint vib_step;
static int vib_mode;

float RhGeom_GetVectorScalor(struct DotrViewVector *v)
{
    /* ShFx_Vib2P hands over a three-float step. */
    if (vib_mode) {
        vib_step = *(struct DotrFxPoint *)v;
        return spin_scalar;
    }
    spin_dir = *v;
    return spin_scalar;
}
float func_001a6c60(float value)
{
    spin_root_arg = value;
    return 2.0f;
}
float func_001a6c18(float a, float b)
{
    spin_atan_args[spin_atan_calls][0] = a;
    spin_atan_args[spin_atan_calls][1] = b;
    return 6.2831855f * (spin_atan_calls++ == 0 ? 0.25f : 0.5f);
}
int func_001a22c0(float value)
{
    if (spin_int_calls < 2) {
        spin_int_args[spin_int_calls] = value;
    }
    spin_int_calls++;
    return (int)value + 0x10000;
}
void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    spin_matrix = (struct DotrViewMatrix *)m;
    memcpy(spin_angles, ang, sizeof(spin_angles));
    ++spin_rot_calls;
}
float func_001a6898(float value)
{
    spin_cos_args[spin_cos_calls++] = value;
    return value + 1.0f;
}
float func_001a6a88(float value)
{
    spin_sin_args[spin_sin_calls++] = value;
    return value + 2.0f;
}
void RhGeom_MulMatrixVector(struct DotrViewVector *out, struct DotrViewMatrix *m, struct DotrViewVector *v)
{
    assert(m == spin_matrix);
    spin_in[spin_mul_calls] = *v;
    spin_out[spin_mul_calls] = out;
    ++spin_mul_calls;
    out->x = 100.0f * spin_mul_calls;
    out->y = 200.0f * spin_mul_calls;
    out->z = 300.0f * spin_mul_calls;
}

/* ShFx_Spin2P: one run with three segments (two loop passes) and one with a single segment
 * (the loop bound: no pass). */
static void check_spin(void)
{
    struct DotrFxPoint out[5];
    struct DotrFxSpin spin;
    int i;

    assert(offsetof(struct DotrFxSpin, start) == 16);
    assert(offsetof(struct DotrFxSpin, end) == 28);
    assert(sizeof(struct DotrFxPoint) == 12);
    memset(out, 0, sizeof(out));
    spin.count = 3;
    spin.radius = 2.0f;
    spin.angle = 30.0f;
    spin.turn = 45.0f;
    spin.start.x = 1.0f;
    spin.start.y = 2.0f;
    spin.start.z = 3.0f;
    spin.end.x = 5.0f;
    spin.end.y = 10.0f;
    spin.end.z = 7.0f;
    spin.out = out;
    spin_scalar = 12.0f;
    ShFx_Spin2P(&spin);
    assert(spin_dir.x == 4.0f && spin_dir.y == 8.0f && spin_dir.z == 4.0f);
    assert(spin_root_arg == 32.0f);
    assert(spin_atan_calls == 2 && spin_atan_args[0][0] == 2.0f && spin_atan_args[0][1] == 8.0f);
    assert(spin_atan_args[1][0] == 4.0f && spin_atan_args[1][1] == 4.0f);
    assert(spin_int_calls == 2 && spin_int_args[0] == 16384.0f && spin_int_args[1] == 32768.0f);
    assert(spin_rot_calls == 1);
    assert(spin_angles[0] == 16384 && spin_angles[1] == 32768 && spin_angles[2] == 0);
    assert(spin_cos_calls == 2 && spin_sin_calls == 2 && spin_mul_calls == 2);
    assert(out[0].x == 1.0f && out[0].y == 2.0f && out[0].z == 3.0f);
    assert(out[3].x == 5.0f && out[3].y == 10.0f && out[3].z == 7.0f);
    assert(out[4].x == 0.0f);
    for (i = 1; i < 3; i++) {
        float rad = 3.1415927f * (30.0f + 45.0f * i) / 180.0f;
        assert(spin_cos_args[i - 1] == rad && spin_sin_args[i - 1] == rad);
        assert(spin_in[i - 1].x == 2.0f * (rad + 1.0f));
        assert(spin_in[i - 1].y == 4.0f * i);
        assert(spin_in[i - 1].z == 2.0f * (rad + 2.0f));
        assert(spin_out[i - 1] == (void *)&out[i]);
        assert(out[i].x == 100.0f * i + 1.0f);
        assert(out[i].y == 200.0f * i + 2.0f);
        assert(out[i].z == 300.0f * i + 3.0f);
    }

    memset(out, 0, sizeof(out));
    spin.count = 1;
    spin_atan_calls = spin_int_calls = spin_cos_calls = spin_sin_calls = spin_mul_calls = 0;
    ShFx_Spin2P(&spin);
    assert(spin_mul_calls == 0 && spin_cos_calls == 0 && spin_rot_calls == 2);
    assert(out[0].y == 2.0f && out[1].y == 10.0f && out[2].y == 0.0f);
}

/* ShFx_Hsin2P: three segments (three loop passes, the last one computes the end point) and
 * one segment (one pass); the end point is never copied. Callee stubs as for ShFx_Spin2P. */
static void check_hsin(void)
{
    struct DotrFxPoint out[5];
    struct DotrFxSpin spin;
    int i;

    memset(out, 0, sizeof(out));
    spin.count = 3;
    spin.radius = 2.0f;
    spin.angle = 30.0f;
    spin.turn = 45.0f;
    spin.start.x = 1.0f;
    spin.start.y = 2.0f;
    spin.start.z = 3.0f;
    spin.end.x = 5.0f;
    spin.end.y = 10.0f;
    spin.end.z = 7.0f;
    spin.out = out;
    spin_scalar = 12.0f;
    spin_atan_calls = spin_int_calls = spin_cos_calls = spin_sin_calls = spin_mul_calls = spin_rot_calls = 0;
    ShFx_Hsin2P(&spin);
    assert(spin_dir.x == 4.0f && spin_dir.y == 8.0f && spin_dir.z == 4.0f);
    assert(spin_root_arg == 32.0f);
    assert(spin_atan_calls == 2 && spin_atan_args[0][0] == 2.0f && spin_atan_args[0][1] == 8.0f);
    assert(spin_atan_args[1][0] == 4.0f && spin_atan_args[1][1] == 4.0f);
    assert(spin_int_calls == 2 && spin_int_args[0] == 16384.0f && spin_int_args[1] == 32768.0f);
    assert(spin_rot_calls == 1);
    assert(spin_angles[0] == 16384 && spin_angles[1] == 32768 && spin_angles[2] == 0);
    assert(spin_cos_calls == 3 && spin_sin_calls == 6 && spin_mul_calls == 3);
    assert(out[0].x == 1.0f && out[0].y == 2.0f && out[0].z == 3.0f);
    assert(out[4].x == 0.0f && out[4].y == 0.0f);
    for (i = 1; i <= 3; i++) {
        float wave = 180.0f / 3;
        float envelope = 3.1415927f * (wave * i) / 180.0f;
        float rad = 3.1415927f * (30.0f + i * 45.0f) / 180.0f;
        float r = 2.0f * (envelope + 2.0f);

        /* Per pass: sine of the envelope angle, then cosine and sine of the circle angle. */
        assert(spin_sin_args[2 * (i - 1)] == envelope);
        assert(spin_cos_args[i - 1] == rad && spin_sin_args[2 * (i - 1) + 1] == rad);
        assert(spin_in[i - 1].x == r * (rad + 1.0f));
        assert(spin_in[i - 1].y == 4.0f * i);
        assert(spin_in[i - 1].z == r * (rad + 2.0f));
        assert(spin_out[i - 1] == (void *)&out[i]);
        assert(out[i].x == 100.0f * i + 1.0f);
        assert(out[i].y == 200.0f * i + 2.0f);
        assert(out[i].z == 300.0f * i + 3.0f);
    }

    memset(out, 0, sizeof(out));
    spin.count = 1;
    spin_atan_calls = spin_int_calls = spin_cos_calls = spin_sin_calls = spin_mul_calls = 0;
    ShFx_Hsin2P(&spin);
    assert(spin_mul_calls == 1 && spin_cos_calls == 1 && spin_sin_calls == 2 && spin_rot_calls == 2);
    assert(spin_sin_args[0] == 3.1415927f * 180.0f / 180.0f);
    assert(out[0].y == 2.0f && out[1].y == 202.0f && out[2].y == 0.0f);
}

/* ShFx_Vec2Rot / ShFx_Vec2Mat: straight-line; one call each checking every forwarded
 * argument and every stored word (the callee stubs are those of ShFx_Spin2P). */
static void check_vec2(void)
{
    struct DotrFxPoint dir = { 3.0f, 5.0f, 4.0f };
    int rot[4] = { -1, -1, -1, 77 };
    float m[16];

    spin_atan_calls = spin_int_calls = spin_rot_calls = 0;
    ShFx_Vec2Rot(&dir, rot);
    assert(spin_root_arg == 25.0f);
    assert(spin_atan_calls == 2 && spin_atan_args[0][0] == 2.0f && spin_atan_args[0][1] == 5.0f);
    assert(spin_atan_args[1][0] == 3.0f && spin_atan_args[1][1] == 4.0f);
    assert(spin_int_calls == 2 && spin_int_args[0] == 16384.0f && spin_int_args[1] == 32768.0f);
    assert(rot[0] == 16384 && rot[1] == 32768 && rot[2] == 0 && rot[3] == 77);
    assert(spin_rot_calls == 0);

    dir.x = 6.0f;
    dir.y = 7.0f;
    dir.z = 8.0f;
    spin_atan_calls = spin_int_calls = 0;
    spin_angles[0] = spin_angles[1] = spin_angles[2] = -1;
    ShFx_Vec2Mat(&dir, m);
    assert(spin_root_arg == 100.0f);
    assert(spin_atan_calls == 2 && spin_atan_args[0][0] == 2.0f && spin_atan_args[0][1] == 7.0f);
    assert(spin_atan_args[1][0] == 6.0f && spin_atan_args[1][1] == 8.0f);
    assert(spin_int_calls == 2 && spin_rot_calls == 1);
    assert((float *)spin_matrix == m);
    assert(spin_angles[0] == 16384 && spin_angles[1] == 32768 && spin_angles[2] == 0);
}

/* ShFx_SetTexFT4 / ShFx_SetTexGT4 callees: every argument recorded. */
static void *tex_packets[3];
static long tex_clamp, tex_alpha;
static int *tex_texture, tex_flag, tex_alpha_calls;

int ExAssignClamp(struct DotrDrawPacket *packet, long value)
{
    tex_packets[0] = packet;
    tex_clamp = value;
    return 0;
}

int ExAssignTexture(struct DotrDrawPacket *packet, int *texture, int a)
{
    tex_packets[1] = packet;
    tex_texture = texture;
    tex_flag = a;
    return 0;
}

int ExAssignAlpha(struct DotrDrawPacket *packet, long value)
{
    tex_packets[2] = packet;
    tex_alpha = value;
    ++tex_alpha_calls;
    return 0;
}

/* ShFx_SetTexFT4: straight-line, one call checking every stored field and forwarded argument.
 * ShFx_SetTexGT4: one branch (mode 0 skips the alpha); both outcomes. */
static void check_set_tex(void)
{
    struct DotrFxFT4Packet ft;
    struct DotrFxGT4Packet gt;
    struct DotrFxAlphaSource source;
    int texture;
    long clamp = 0xaL | (3L << 4) | (9L << 14) | (5L << 24) | (19L << 34);
    int i;

    memset(&source, 0, sizeof(source));
    source.mode = 2;
    source.left = 3;
    source.right = 10;
    source.top = 5;
    source.bottom = 20;
    memset(&ft, 0xff, sizeof(ft));
    ft.flags = 0x0001;
    ShFx_SetTexFT4(&ft, &source, &texture);
    assert(ft.flags == 0x0801 && ft.unknown_00 == 0xffff);
    for (i = 0; i < 4; ++i) {
        assert(ft.vertex[i].uv.u == ((i & 1) ? 160 : 48) && ft.vertex[i].uv.v == ((i & 2) ? 320 : 80));
        assert(ft.vertex[i].uv.u_high == 3 && ft.vertex[i].uv.v_high == 3);
        assert(ft.vertex[i].unknown_04[0] == -1 && ft.vertex[i].unknown_04[2] == -1);
    }
    assert(tex_packets[0] == &ft && tex_packets[1] == &ft && tex_packets[2] == &ft);
    assert(tex_clamp == clamp && tex_texture == &texture && tex_flag == 1);
    assert(tex_alpha_calls == 1 && tex_alpha == (0L | (2L << 2) | (0L << 4) | (1L << 6)));

    memset(&gt, 0xff, sizeof(gt));
    gt.flags = 0x0002;
    source.mode = 3;
    tex_flag = tex_alpha_calls = 0;
    ShFx_SetTexGT4(&gt, &source, &texture);
    assert(gt.flags == 0x0802);
    for (i = 0; i < 4; ++i) {
        assert(gt.vertex[i].uv.u == ((i & 1) ? 160 : 48) && gt.vertex[i].uv.v == ((i & 2) ? 320 : 80));
        assert(gt.vertex[i].uv.u_high == 3 && gt.vertex[i].uv.v_high == 3);
        assert(gt.vertex[i].unknown_04[0] == -1 && gt.vertex[i].unknown_04[4] == -1);
    }
    assert(tex_packets[0] == &gt && tex_packets[1] == &gt && tex_packets[2] == &gt);
    assert(tex_clamp == clamp && tex_texture == &texture && tex_flag == 1);
    assert(tex_alpha_calls == 1 && tex_alpha == (1L | (0L << 2) | (0L << 4) | (1L << 6)));

    source.mode = 0;
    tex_packets[2] = NULL;
    tex_flag = tex_alpha_calls = 0;
    ShFx_SetTexGT4(&gt, &source, &texture);
    assert(tex_packets[1] == &gt && tex_flag == 1);
    assert(tex_alpha_calls == 0 && tex_packets[2] == NULL);
}

/* ShFx_Vib2P (0x102a50). Loop coverage: count 1 (ends only, one range conversion) and count 3
 * (two points between, three random offsets each, seven conversions). The stubs give a step
 * length of 2, so the range is int(200 * 0.5 * 2) as the conversion stub returns it. */
static int vib_close(float a, float b)
{
    float d = a - b;
    return d < 0.001f && d > -0.001f;
}
static void check_vib(void)
{
    struct DotrFxVib p;
    struct DotrFxPoint out[4];
    int i, r, range, half;

    p.amplitude = 0.5f;
    p.start.x = 1.0f; p.start.y = 2.0f; p.start.z = 3.0f;
    p.end.x = 4.0f; p.end.y = 8.0f; p.end.z = 12.0f;
    p.out = out;
    vib_mode = 1;
    spin_scalar = 2.0f;

    p.count = 1;
    memset(out, 0, sizeof(out));
    spin_int_calls = 0;
    ShFx_Vib2P(&p);
    assert(vib_step.x == 3.0f && vib_step.y == 6.0f && vib_step.z == 9.0f);
    assert(memcmp(&out[0], &p.start, sizeof(p.start)) == 0 && memcmp(&out[1], &p.end, sizeof(p.end)) == 0);
    assert(spin_int_calls == 1 && spin_int_args[0] == 200.0f && out[2].x == 0.0f);

    p.count = 3;
    memset(out, 0, sizeof(out));
    spin_int_calls = 0;
    random_next = 0;
    ShFx_Vib2P(&p);
    assert(vib_step.x == 1.0f && vib_step.y == 2.0f && vib_step.z == 3.0f && spin_int_calls == 7);
    assert(memcmp(&out[0], &p.start, sizeof(p.start)) == 0 && memcmp(&out[3], &p.end, sizeof(p.end)) == 0);
    range = 200 + 0x10000;
    half = range >> 1;
    r = 0;
    for (i = 1; i < 3; i++) {
        r += 0x111;
        assert(vib_close(out[i].x, 1.0f + 1.0f * i + (float)((r >> 4) % range - half) / 100.0f));
        r += 0x111;
        assert(vib_close(out[i].y, 2.0f + 2.0f * i + (float)((r >> 4) % range - half) / 100.0f));
        r += 0x111;
        assert(vib_close(out[i].z, 3.0f + 3.0f * i + (float)((r >> 4) % range - half) / 100.0f));
    }
    vib_mode = 0;
    spin_int_calls = 0;
}

/* ShFx_Spline: the count bounds (2 and 9 rejected, 3 and 8 accepted), a straight line sampled
 * exactly (the natural spline of collinear points is the line), and a curve that passes through
 * its knots. This group's func_001a6c60 stub answers 2.0 for every chord, so the parameter is
 * uniform: knot i at i / (count - 1). */
static int near_spline(float a, float b)
{
    float d = a - b;

    return d < 1e-4f && d > -1e-4f;
}
static void check_shfx_spline(void)
{
    static struct DotrFxSpline sp;
    int i;

    memset(&sp, 0, sizeof(sp));
    sp.out[0].x = 77.0f;
    sp.count = 2;
    assert(ShFx_Spline(&sp) == 0 && sp.out[0].x == 77.0f);
    sp.count = 9;
    assert(ShFx_Spline(&sp) == 0 && sp.out[0].x == 77.0f);

    /* three collinear points: x = 2u, y = 4u, z = 5; eight samples at u = k / 8 */
    memset(&sp, 0, sizeof(sp));
    sp.count = 3;
    for (i = 0; i < 3; i++) {
        sp.x[i] = (float)i;
        sp.y[i] = 2.0f * (float)i;
        sp.z[i] = 5.0f;
    }
    sp.out[8].x = 55.0f;
    sp.out[0].w = 66.0f;
    spin_root_arg = 0.0f;
    assert(ShFx_Spline(&sp) == 1);
    assert(spin_root_arg == 5.0f);       /* the last chord: 1 + 4 + 0 */
    for (i = 0; i < 8; i++) {
        float u = (float)i / 8.0f;

        assert(near_spline(sp.out[i].x, 2.0f * u) && near_spline(sp.out[i].y, 4.0f * u));
        assert(near_spline(sp.out[i].z, 5.0f));
    }
    assert(sp.out[0].x == 0.0f && sp.out[8].x == 55.0f && sp.out[0].w == 66.0f);

    /* eight points on a curve: 28 samples, every fourth one on a knot */
    memset(&sp, 0, sizeof(sp));
    sp.count = 8;
    for (i = 0; i < 8; i++) {
        sp.x[i] = (float)i;
        sp.y[i] = (float)((i * i) % 5);
        sp.z[i] = (float)(i & 1);
    }
    assert(ShFx_Spline(&sp) == 1);
    for (i = 0; i < 7; i++) {
        assert(near_spline(sp.out[i * 4].x, (float)i) && near_spline(sp.out[i * 4].y, sp.y[i]));
        assert(near_spline(sp.out[i * 4].z, sp.z[i]));
    }
    /* between knots 1 and 2 the curve is not the chord (y 1 -> 4, z 1 -> 0) */
    assert(!near_spline(sp.out[6].y, 2.5f) && near_spline(sp.out[6].x, 1.5f));
}

int main(void)
{
    check_sin_cos_inrad();
    check_random_and_center();
    assert(sizeof(struct DotrFxBlackSpan) == 4);

    assert(GCalc_Decrease(6.0f, 2.0f, 4.0f) == 2.0f);
    assert(GCalc_Decrease(4.0f, 2.0f, 4.0f) == 1.0f); /* boundary: value == limit */
    assert(GCalc_Decrease(3.0f, 2.0f, 4.0f) == 1.0f);
    assert(GCalc_Increase(1.0f, 4.0f) == 0.25f);
    assert(GCalc_Increase(4.0f, 4.0f) == 1.0f); /* boundary: value == limit */
    assert(GCalc_Increase(5.0f, 4.0f) == 1.0f);

    distance_index = -1;
    assert(ShFx_GetDrawSize(64.0f, 512.0f) == 64.0f);
    assert(distance_index == 0);

    check_black();
    check_limits();
    check_rot_matrix();
    check_kind_tables();
    check_window_uv();
    check_uv_and_clip();
    check_front_back_and_spline();
    check_shfx_spline();
    check_spin();
    check_hsin();
    check_vec2();
    check_set_tex();
    puts("fx_calc: ok");
    check_vib();
    return 0;
}

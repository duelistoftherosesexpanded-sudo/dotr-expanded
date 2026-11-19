/* Synthetic host tests for the lk_math group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/lk_math.h"

struct DotrLkVec data_0028c220_lk = {1.5f, 2.5f, 3.5f, 1.0f};

static char order[16];
static int calls;

static void said(char c)
{
    assert(calls < (int)sizeof order - 1);
    order[calls++] = c;
    order[calls] = 0;
}
static void reset(void)
{
    calls = 0;
    order[0] = 0;
}

static int rands[2], rand_next;
int func_0019a5c8(void)
{
    said('R');
    return rands[rand_next++];
}

static float conv_in[4];
static int convs;
/* LkMoveHoming: the six converted angles come from this queue instead of the float. */
static int conv_queue[6], conv_queued;
int func_001a22c0(float value)
{
    said('I');
    if (conv_queued) {
        assert(convs < 6);
        return conv_queue[convs++];
    }
    conv_in[convs++] = value;
    return (int)value;
}

static float atan_a, atan_b, atan_result, atan_log[2][2], atan_queue[2];
static int atans, atan_queued;
static float atan_long_log[6][2];
static int atan_long;
float func_001a6c18(float a, float b)
{
    said('A');
    atan_a = a;
    atan_b = b;
    if (atans < 2) {
        atan_log[atans][0] = a;
        atan_log[atans][1] = b;
    }
    if (atan_long < 6) {
        atan_long_log[atan_long][0] = a;
        atan_long_log[atan_long][1] = b;
        atan_long++;
    }
    if (atan_queued) {
        assert(atans < 2);
        return atan_queue[atans++];
    }
    atans++;
    return atan_result;
}

static float sqrt_in;
float func_001a6c60(float value)
{
    said('S');
    sqrt_in = value;
    return value * 0.25f;
}

static int angles[4];
static float *rot_matrix;
static int want_fourth;
void RhGeom_GetRotXYZMatrix(float *m, int *ang)
{
    said('M');
    memcpy(angles, ang, 3 * sizeof *ang);
    if (want_fourth) {
        angles[3] = ang[3]; /* LkMoveHoming sets it; LkGetRandVec does not */
    }
    rot_matrix = m;
    m[0] = 42.0f;
}

static struct DotrLkVec mul_in;
void RhGeom_MulMatrixVector(struct DotrLkVec *out, float *m, struct DotrLkVec *v)
{
    said('V');
    assert(m == rot_matrix && m[0] == 42.0f);
    mul_in = *v;
    out->x = v->x * 2.0f;
    out->y = v->y * 2.0f;
    out->z = v->z * 2.0f;
    out->w = v->w;
}

static void check_rand_vec(void)
{
    struct DotrLkVec out;

    reset();
    rand_next = 0;
    rands[0] = 0x23456;
    rands[1] = 70000;
    assert(LkGetRandVec(&out, 8.0f) == 0);
    assert(strcmp(order, "RRMV") == 0);
    assert(angles[0] == 0x3456 && angles[1] == 70000 - 65536 && angles[2] == 0);
    assert(mul_in.x == 1.5f && mul_in.y == 8.0f && mul_in.z == 3.5f && mul_in.w == 1.0f);
    assert(out.x == 3.0f && out.y == 16.0f && out.z == 7.0f && out.w == 1.0f);
    /* The constant itself is not changed. */
    assert(data_0028c220_lk.y == 2.5f);
}

static char point1, point2;
static float *proj_matrix[2];
static int projs;
int func_00205440_lk(void *in, float *matrix, struct DotrLkScreen *packet, float *w)
{
    said('P');
    assert(w != NULL);
    proj_matrix[projs++] = matrix;
    if (in == &point1) {
        packet->x = 0x200;
        packet->y = 0x100;
    } else {
        assert(in == &point2);
        packet->x = 0x100;
        packet->y = 0x300;
    }
    packet->z = 0;
    *w = 1.0f;
    return 0;
}

static void check_angle(void)
{
    struct DotrLkObject obj;

    memset(&obj, 0, sizeof obj);
    reset();
    projs = convs = 0;
    atan_result = 3.14159265f / 2.0f;   /* a quarter turn */
    assert(LkSetAngleW2S(&obj, &point1, &point2) == 16384);
    assert(strcmp(order, "PPAI") == 0);
    assert(proj_matrix[0] == obj.matrix && proj_matrix[1] == obj.matrix);
    assert(atan_a == 16.0f && atan_b == -32.0f);

    /* Whole turns are removed by % 0x10000; negative angles stay negative. */
    reset();
    projs = convs = 0;
    atan_result = 6.2831855f * 2.0f;    /* two whole turns */
    assert(LkSetAngleW2S(&obj, &point1, &point2) == 0 && conv_in[0] == 131072.0f);
    reset();
    projs = convs = 0;
    atan_result = 6.2831855f * 1.5f;    /* about one and a half turns */
    assert(atan_result / 6.2831855f * 65536.0f > 65536.0f);
    assert(LkSetAngleW2S(&obj, &point1, &point2) == (int)(atan_result / 6.2831855f * 65536.0f) - 65536);
    reset();
    projs = convs = 0;
    atan_result = -3.14159265f / 2.0f;
    assert(LkSetAngleW2S(&obj, &point1, &point2) == -16384);
}

static int frame(float p0, float p1, float p2, float p3)
{
    float p[4];

    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
    p[3] = p3;
    reset();
    convs = 0;
    return LkAssignFrameST(p);
}

static void check_frame(void)
{
    assert(frame(9.0f, 6.0f, 2.0f, 4.0f) == 2);
    assert(strcmp(order, "II") == 0 && conv_in[0] == 4.5f && conv_in[1] == 2.5f);
    assert(frame(0.0f, 6.0f, 2.0f, 4.0f) == 1 && conv_in[0] == 0.0f && conv_in[1] == 1.5f);
    assert(frame(9.0f, 0.0f, 2.0f, 4.0f) == 1 && conv_in[1] == 1.0f);
    assert(frame(0.0f, 0.0f, 2.0f, 4.0f) == 0 && conv_in[1] == 0.0f);
    assert(frame(-9.0f, 6.0f, 2.0f, 4.0f) == 0 && conv_in[0] == -4.5f && conv_in[1] == 0.5f);
}

static void axis(float ax, float ay, float az, float bx, float by, float bz, float yaw, float pitch,
                 int out1, int out0)
{
    float a[3], b[3];
    int out[4] = {7, 7, 7, 7};
    float dx = ax - bx, dy = ay - by, dz = az - bz;

    a[0] = ax; a[1] = ay; a[2] = az;
    b[0] = bx; b[1] = by; b[2] = bz;
    reset();
    convs = atans = 0;
    atan_queued = 1;
    atan_queue[0] = yaw;
    atan_queue[1] = pitch;
    assert(LkChangeAxisY(a, b, out) == 0);
    atan_queued = 0;
    assert(strcmp(order, "AISAI") == 0);
    assert(atan_log[0][0] == dx && atan_log[0][1] == dz);
    assert(sqrt_in == dx * dx + dz * dz);
    assert(atan_log[1][0] == sqrt_in * 0.25f && atan_log[1][1] == dy);
    assert(conv_in[0] == yaw / 6.2831855f * 65536.0f && conv_in[1] == pitch / 6.2831855f * 65536.0f);
    assert(out[1] == out1 && out[0] == out0 && out[2] == 0 && out[3] == 0);
}

static void check_change_axis(void)
{
    /* Positive differences: a quarter turn of yaw, half a turn of pitch. */
    axis(5.0f, 4.0f, 3.0f, 1.0f, 1.0f, 1.0f, 1.5707964f, 3.1415927f, 16384, 32768);
    /* Negative yaw stays negative; a whole turn reduces to 0, one and a quarter to 16384. */
    axis(-3.0f, 0.0f, 2.0f, 1.0f, 2.0f, -1.0f, -1.5707964f, 6.2831855f, -16384, 0);
    axis(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -6.2831855f * 1.25f, 6.2831855f * 1.25f, -16384, 16384);
    /* a == b: all differences zero. */
    axis(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0, 0);
}

/* LkMoveHoming with the six raw converted angles ty, tx, tz, dy, dx, dz (before % 0x10000). */
static void homing(int ty, int tx, int tz, int dy, int dx, int dz, int limit, int a0, int a1, int a2)
{
    float pos[3] = {1.0f, 2.0f, 3.0f};
    float target[3] = {11.0f, 22.0f, 33.0f};
    struct DotrLkVec dir = {0.5f, 1.0f, 1.5f, 9.0f};

    reset();
    convs = atan_long = 0;
    atan_result = 0.25f;
    conv_queued = want_fourth = 1;
    conv_queue[0] = ty;
    conv_queue[1] = tx;
    conv_queue[2] = tz;
    conv_queue[3] = dy;
    conv_queue[4] = dx;
    conv_queue[5] = dz;
    angles[3] = 99;
    assert(LkMoveHoming(pos, target, &dir, limit) == 0);
    conv_queued = want_fourth = 0;
    assert(strcmp(order, "AIAIAIAIAIAIMV") == 0);
    /* target -= pos, written back; the angle pairs (x, z), (z, y), (y, x) of target then dir */
    assert(target[0] == 10.0f && target[1] == 20.0f && target[2] == 30.0f);
    assert(atan_long_log[0][0] == 10.0f && atan_long_log[0][1] == 30.0f);
    assert(atan_long_log[1][0] == 30.0f && atan_long_log[1][1] == 20.0f);
    assert(atan_long_log[2][0] == 20.0f && atan_long_log[2][1] == 10.0f);
    assert(atan_long_log[3][0] == 0.5f && atan_long_log[3][1] == 1.5f);
    assert(atan_long_log[4][0] == 1.5f && atan_long_log[4][1] == 1.0f);
    assert(atan_long_log[5][0] == 1.0f && atan_long_log[5][1] == 0.5f);
    assert(angles[0] == a0 && angles[1] == a1 && angles[2] == a2 && angles[3] == 0);
    /* dir rotated (the stub doubles it): pos += it, dir = it, w cleared */
    assert(mul_in.x == 0.5f && mul_in.y == 1.0f && mul_in.z == 1.5f && mul_in.w == 9.0f);
    assert(pos[0] == 2.0f && pos[1] == 4.0f && pos[2] == 6.0f);
    assert(dir.x == 1.0f && dir.y == 2.0f && dir.z == 3.0f && dir.w == 0.0f);
}

static void check_homing(void)
{
    /* Equal pair; from > to near; from < to near after a negative remainder (-70000 % 0x10000). */
    homing(0x10100, 300, -70000, 100, 300, -4000, 1000, 0, 156, -464);
    /* Clamps: at or above limit, at or below -limit; the bounds themselves are kept. */
    homing(0x100, 300, -70000, 100, 300, -4000, 156, 0, 156, -156);
    homing(0x100, 300, -70000, 100, 300, -4000, 464, 0, 156, -464);
    /* Wrap-around both ways: from > to beyond 0x8000 (-0x7100), from < to beyond 0x8000
     * (0x7100), and exactly 0x8000 (taken directly). */
    homing(0x8000, 0x9000, 0x100, 0, 0x100, 0x9000, 0x10000, -0x7100, 0x8000, 0x7100);
    homing(0x8000, 0x9000, 0x100, 0, 0x100, 0x9000, 0x1000, -0x1000, 0x1000, 0x1000);
    /* from < to by exactly 0x8000 gives -0x8000; a difference of 0x18000 reduces to 0x8000 and is
     * returned whole. */
    homing(0, 0xc000, 0, 0x8000, -0xc000, 0, 0x20000, 0x18000, -0x8000, 0);
}

int main(void)
{
    check_rand_vec();
    check_angle();
    check_frame();
    check_change_axis();
    check_homing();
    puts("lk_math: ok");
    return 0;
}

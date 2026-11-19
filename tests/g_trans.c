/* Synthetic host tests for the g_trans group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/g_trans.h"

float data_0030ff00_g[16];

static struct DotrGVec seen[4];
static float *seen_m;
static int seen_a, seen_b, seen_c, projs;
/* How many vectors the caller's array holds: 3 for GRotTransPers_3, 4 for GRotTransPers_4. */
static int seen_n;

int func_002058d0_g(struct DotrGVec *v, float *m, int a, int b, int c)
{
    projs++;
    memcpy(seen, v, seen_n * sizeof *v);
    seen_m = m;
    seen_a = a;
    seen_b = b;
    seen_c = c;
    /* The copies are the caller's own array. */
    v[0].x = -1.0f;
    return 1000 + a;
}

static float sin_in[2];
static int sins;
float func_001a6b70_g(float x)
{
    assert(sins < 2);
    sin_in[sins++] = x;
    return x * 2.0f;
}
static float pow_x, pow_y, pow_log[4][2];
static int pows;
float func_001a6c48_g(float x, float y)
{
    if (pows < 4) {
        pow_log[pows][0] = x;
        pow_log[pows][1] = y;
    }
    pows++;
    pow_x = x;
    pow_y = y;
    return x + y;
}

static void set(struct DotrGVec *v, float base)
{
    v->x = base;
    v->y = base + 1.0f;
    v->z = base + 2.0f;
    v->w = base + 3.0f;
}

static void check_rot_trans(void)
{
    struct DotrGVec v[4];
    int i;

    for (i = 0; i < 4; i++) {
        set(&v[i], 10.0f * (i + 1));
    }
    memset(seen, 0, sizeof seen);
    projs = 0;
    seen_n = 3;
    assert(GRotTransPers_3(&v[0], &v[1], &v[2], 5, 6, 7) == 1005 && projs == 1);
    assert(seen_m == data_0030ff00_g && seen_a == 5 && seen_b == 6 && seen_c == 7);
    for (i = 0; i < 3; i++) {
        assert(memcmp(&seen[i], &v[i], sizeof v[i]) == 0);
    }
    assert(v[0].x == 10.0f);

    seen_n = 4;
    assert(GRotTransPers_4(&v[0], &v[1], &v[2], &v[3], -2, 8, 9) == 998 && projs == 2);
    assert(seen_m == data_0030ff00_g && seen_a == -2 && seen_b == 8 && seen_c == 9);
    for (i = 0; i < 4; i++) {
        assert(memcmp(&seen[i], &v[i], sizeof v[i]) == 0);
    }
    assert(v[0].x == 10.0f);
}

static float trans(int a, int b, int n, int quotient)
{
    float r, angle;

    sins = 0;
    r = GCalc_Trans(a, b, n);
    angle = 6.2831855f * ((float)quotient / 65536.0f);
    assert(sins == 2 && sin_in[0] == angle && sin_in[1] == 1.2566179f);
    assert(pow_x == (angle * 2.0f) / (1.2566179f * 2.0f) && pow_y == (float)n);
    assert(r == 1.0f - (pow_x + pow_y));
    return r;
}

static void check_calc_trans(void)
{
    /* (b - a) << 16 divided by b * 5, truncated toward zero. */
    trans(0, 1, 2, 13107);
    trans(3, 1, 1, -26214);
    trans(2, 2, 0, 0);
    assert(trans(0, 2, 3, 13107) == 1.0f - (pow_x + 3.0f));
    trans(1, 4, -1, (3 << 16) / 20);
}

/* GVec2RotMat_Z fixtures and stubs. */
struct DotrGAngles data_0028c210_g = {11, 22, 33, 44};
static char order[32];
static int ncalls;
static void said(char c)
{
    assert(ncalls < (int)sizeof order - 1);
    order[ncalls++] = c;
    order[ncalls] = 0;
}
static float *unit_m;
void func_0020bbb0_g(float *m)
{
    int i;
    said('U');
    unit_m = m;
    for (i = 0; i < 16; i++) m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
}
static int rot_ang[3][4], rots;
static float *rot_out[3];
void func_0020cb90_g(float *out, int *ang)
{
    said('R');
    assert(rots < 3);
    memcpy(rot_ang[rots], ang, sizeof rot_ang[rots]);
    rot_out[rots++] = out;
}
static float atan_args[2][2], atan_ret[2];
static int atans;
float func_001a6c18_g(float a, float b)
{
    said('A');
    assert(atans < 2);
    atan_args[atans][0] = a;
    atan_args[atans][1] = b;
    return atan_ret[atans++];
}
static float sqrt_in;
float func_001a6c60_g(float x)
{
    said('S');
    sqrt_in = x;
    return x * 0.5f;
}
static float conv_in[2];
static int convs;
int func_001a22c0_g(float x)
{
    said('I');
    assert(convs < 2);
    conv_in[convs++] = x;
    return (int)x;
}

static void rot_mat(float x, float y, float z, float a0, float a1, int z_angle, int y_angle)
{
    float out[16];
    float v[3];

    v[0] = x; v[1] = y; v[2] = z;
    ncalls = 0; order[0] = 0; rots = 0; atans = 0; convs = 0; pows = 0;
    atan_ret[0] = a0;
    atan_ret[1] = a1;
    memset(out, 0, sizeof out);
    GVec2RotMat_Z(out, v);
    /* Unit matrix, then each rotation from its own fresh copy of the template. */
    assert(strcmp(order, "UAIRSAIR") == 0 && pows == 2);
    assert(unit_m == out && rot_out[0] == out && rot_out[1] == out && out[0] == 1.0f && out[5] == 1.0f);
    assert(atan_args[0][0] == y && atan_args[0][1] == x);
    assert(conv_in[0] == 65536.0f * (a0 / 6.2831855f));
    assert(rot_ang[0][0] == 11 && rot_ang[0][1] == 22 && rot_ang[0][2] == z_angle && rot_ang[0][3] == 44);
    assert(pow_log[0][0] == x && pow_log[0][1] == 2.0f && pow_log[1][0] == y && pow_log[1][1] == 2.0f);
    /* The stub "power" is x + 2, so the root sees (x + 2) + (y + 2). */
    assert(sqrt_in == (x + 2.0f) + (y + 2.0f));
    assert(atan_args[1][0] == sqrt_in * 0.5f && atan_args[1][1] == z);
    assert(conv_in[1] == 65536.0f * (a1 / 6.2831855f));
    assert(rot_ang[1][0] == 11 && rot_ang[1][1] == y_angle && rot_ang[1][2] == 33 && rot_ang[1][3] == 44);
    assert(data_0028c210_g.x == 11 && data_0028c210_g.y == 22 && data_0028c210_g.z == 33);
}

static void check_vec2rotmat(void)
{
    /* Quarter and half turns: 16384 and 32768. */
    rot_mat(1.0f, 2.0f, 3.0f, 1.5707964f, 3.1415927f, 16384, 32768);
    /* Negative angles wrap through the 16-bit mask: -16384 -> 49152, -1 -> 0xffff. */
    rot_mat(-1.0f, -2.0f, 0.5f, -1.5707964f, -0.0001f, 49152, 0xffff);
    /* Zero vector, zero angles. */
    rot_mat(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0);
}

/* GSet_RandVector fixtures and stubs. The table holds f(i, j, k) = 1000 i + 10 j + k + 0.5
 * so a read names its indices. */
float data_0028bc10[4][32][3];
static int rand_seq[16], rands, rand_n;
int func_0019a5c8(void)
{
    said('r');
    assert(rands < rand_n);
    return rand_seq[rands++];
}
/* A draw whose (r >> 23) is v; the low bits are noise that the shift drops. */
static int draw(int v)
{
    return (int)((unsigned int)v << 23) | 0x5a5a5;
}
static float scale_seen[4], scale_by, *scale_out, *scale_in;
void func_00187b48(float *out, float *in, float scale)
{
    said('S');
    memcpy(scale_seen, in, sizeof scale_seen);
    scale_out = out;
    scale_in = in;
    scale_by = scale;
    out[0] = in[0] * scale;
    out[1] = in[1] * scale;
    out[2] = in[2] * scale;
    out[3] = in[3] * scale;
}
static float applied_in[4], *apply_out, *apply_m, *apply_in;
void func_001878e0(float *out, float *m, float *in)
{
    said('M');
    memcpy(applied_in, in, sizeof applied_in);
    apply_out = out;
    apply_m = m;
    apply_in = in;
    out[0] = -7.0f;
}
static float cell(int i, int j, int k)
{
    return 1000.0f * i + 10.0f * j + k + 0.5f;
}

/* One call with the index draw values idx[e] (the three draws of element e all equal, so
 * the result does not depend on the host compiler's order of evaluation inside the one
 * expression), sign draw values sg[e] and angle draw values ang[0..2]. */
static void rand_vector(const int idx[3], const int sg[3], const int ang[3], float scale)
{
    float out[4];
    int e, n = 0;

    for (e = 0; e < 3; e++) {
        rand_seq[n++] = draw(idx[e]);
        rand_seq[n++] = draw(idx[e]);
        rand_seq[n++] = draw(idx[e]);
        rand_seq[n++] = draw(sg[e]);
    }
    for (e = 0; e < 3; e++) {
        rand_seq[n++] = draw(ang[e]);
    }
    rand_n = n;
    rands = 0;
    ncalls = 0; order[0] = 0; rots = 0;
    memset(out, 0, sizeof out);
    GSet_RandVector(out, scale);
    assert(strcmp(order, "rrrrrrrrrrrrSUrRrRrRM") == 0 && rands == 15);
    assert(scale_out == out && scale_in == out && scale_by == scale);
    for (e = 0; e < 3; e++) {
        int i = idx[e] % 4, j = idx[e] % 32, k = idx[e] % 3;
        float s = 2.0f * (float)(sg[e] % 2) - 1.0f;

        assert(scale_seen[e] == s * cell(i, j, k));
        assert(applied_in[e] == scale_seen[e] * scale);
    }
    assert(scale_seen[3] == 1.0f && applied_in[3] == scale);
    /* Rotations of the local matrix about x, y, z, each from a fresh template copy. */
    assert(unit_m != out && rot_out[0] == unit_m && rot_out[1] == unit_m && rot_out[2] == unit_m);
    assert(apply_m == unit_m && apply_out == out && apply_in == out && out[0] == -7.0f);
    assert(rot_ang[0][0] == ((ang[0] % 256) << 8) && rot_ang[0][1] == 22 && rot_ang[0][2] == 33 && rot_ang[0][3] == 44);
    assert(rot_ang[1][0] == 11 && rot_ang[1][1] == ((ang[1] % 256) << 8) && rot_ang[1][2] == 33 && rot_ang[1][3] == 44);
    assert(rot_ang[2][0] == 11 && rot_ang[2][1] == 22 && rot_ang[2][2] == ((ang[2] % 256) << 8) && rot_ang[2][3] == 44);
}

static void check_rand_vector(void)
{
    static const int idx_a[3] = {0, 37, 255}, sg_a[3] = {1, 0, 255}, ang_a[3] = {0, 1, 255};
    /* -96 is 0 modulo 4, 32 and 3; a sign draw of -1 gives r % 2 = -1, so the factor -3. */
    static const int idx_b[3] = {-96, 95, 128}, sg_b[3] = {-1, -2, 3}, ang_b[3] = {128, 64, 200};
    int i, j, k, n, e;
    float out[4];

    for (i = 0; i < 4; i++)
        for (j = 0; j < 32; j++)
            for (k = 0; k < 3; k++)
                data_0028bc10[i][j][k] = cell(i, j, k);
    rand_vector(idx_a, sg_a, ang_a, 2.0f);
    rand_vector(idx_b, sg_b, ang_b, -0.5f);
    assert(scale_seen[0] == -3.0f * cell(0, 0, 0) && scale_seen[1] == -1.0f * cell(3, 31, 2));

    /* Three different index draws: the element is one of the six orders of them (the target
     * compiler uses the first for % 4, the second for % 32 and the third for % 3; the host
     * order is unspecified). */
    n = 0;
    for (e = 0; e < 3; e++) {
        rand_seq[n++] = draw(5);
        rand_seq[n++] = draw(70);
        rand_seq[n++] = draw(11);
        rand_seq[n++] = draw(1);
    }
    for (e = 0; e < 3; e++) {
        rand_seq[n++] = draw(0);
    }
    rand_n = n;
    rands = 0;
    ncalls = 0; order[0] = 0; rots = 0;
    GSet_RandVector(out, 1.0f);
    {
        static const int v[6][3] = {{5, 70, 11}, {5, 11, 70}, {70, 5, 11}, {70, 11, 5}, {11, 5, 70}, {11, 70, 5}};
        int hit = 0;

        for (i = 0; i < 6; i++) {
            if (scale_seen[0] == cell(v[i][0] % 4, v[i][1] % 32, v[i][2] % 3)) {
                hit = 1;
            }
        }
        assert(hit && scale_seen[0] == scale_seen[1] && scale_seen[1] == scale_seen[2]);
    }
}

int main(void)
{
    check_rot_trans();
    check_calc_trans();
    check_vec2rotmat();
    check_rand_vector();
    puts("g_trans: ok");
    return 0;
}

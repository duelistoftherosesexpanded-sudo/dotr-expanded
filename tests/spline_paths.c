/* Synthetic host tests for the spline_paths group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/spline_paths.h"

/* func (0x21c320): records its arguments and returns a value derived from them. */
static int rhs_calls;
static float rhs_args[8][5];

float func(float a, float b, float c, float d, float e)
{
    rhs_args[rhs_calls][0] = a;
    rhs_args[rhs_calls][1] = b;
    rhs_args[rhs_calls][2] = c;
    rhs_args[rhs_calls][3] = d;
    rhs_args[rhs_calls][4] = e;
    rhs_calls++;
    return c - a + 0.5f;
}

/* tri_diag: copies the system it was given and writes solution[i] = right[i] / 4 + i. */
static int tri_calls;
static int tri_n[2];
static float tri_lower[2][64], tri_diag_in[2][64], tri_upper[2][64], tri_right[2][64];
static float tri_sol[2][64];

void tri_diag(float *lower, float *diagonal, float *upper, float *right, float *solution, int n)
{
    int i;

    assert(tri_calls < 2);
    tri_n[tri_calls] = n;
    for (i = 0; i < n; i++) {
        tri_lower[tri_calls][i] = lower[i];
        tri_diag_in[tri_calls][i] = diagonal[i];
        tri_upper[tri_calls][i] = upper[i];
        tri_right[tri_calls][i] = right[i];
        solution[i] = right[i] * 0.25f + (float)i;
        tri_sol[tri_calls][i] = solution[i];
    }
    tri_calls++;
}

/* powf stand-in for the integer powers used (2 and 3). */
static int pow_calls;

float func_001a6c48_spline(float x, float y)
{
    pow_calls++;
    assert(y == 2.0f || y == 3.0f);
    return y == 2.0f ? x * x : x * x * x;
}

/* sqrtf stand-in: records its argument and returns the next scripted value. */
static int sqrt_calls;
static float sqrt_args[8];
static float sqrt_results[8];

float func_001a6c60(float value)
{
    sqrt_args[sqrt_calls] = value;
    return sqrt_results[sqrt_calls++];
}

/* float-to-int stand-in: truncation, recorded. */
static int ftoi_calls;
static float ftoi_args[8];

int func_001a22c0(float value)
{
    ftoi_args[ftoi_calls++] = value;
    return (int)value;
}

static void reset(void)
{
    rhs_calls = tri_calls = pow_calls = sqrt_calls = ftoi_calls = 0;
    memset(tri_n, 0, sizeof(tri_n));
}

static int close_to(float a, float b)
{
    float d = a - b;

    return d < 1e-4f && d > -1e-4f;
}

/* Reference value of one coordinate on segment i at parameter t, from the solution that the
 * stubbed tri_diag returned. */
static float expect_at(float (*pts)[2], float *h, int i, int k, float t)
{
    float s0 = tri_sol[k][i];
    float s1 = tri_sol[k][i + 1];
    float a = (s0 + s1) / (h[i] * h[i]) + 2.0f * (pts[i][k] - pts[i + 1][k]) / (h[i] * h[i] * h[i]);
    float b = 3.0f * (pts[i + 1][k] - pts[i][k]) / (h[i] * h[i]) - (2.0f * s0 + s1) / h[i];

    return pts[i][k] + t * (s0 + t * (b + a * t));
}

static void check_sample(float *x, float *y, int m, float (*pts)[2], float *h, int i, float t)
{
    assert(close_to(x[m], expect_at(pts, h, i, 0, t)));
    assert(close_to(y[m], expect_at(pts, h, i, 1, t)));
}

/* n = 2, e == 1: the single segment is the last one, sampled t = 0, 0.1, 0.2, 0.3 <= h. The
 * end rows of the system and the right-hand sides at both ends; no middle row call. */
static void check_two_points_fixed_step(void)
{
    float pts[2][2] = {{1.0f, 2.0f}, {3.0f, -1.0f}};
    float h[1] = {0.35f};
    float x[64], y[64];
    short count = 99;
    int k;

    reset();
    Taku_spline2(x, y, &count, pts, h, 2, 1, 4.0f);
    assert(tri_calls == 2 && rhs_calls == 0 && sqrt_calls == 0 && ftoi_calls == 0);
    for (k = 0; k < 2; k++) {
        assert(tri_n[k] == 2);
        assert(tri_diag_in[k][0] == 2.0f * h[0] && tri_upper[k][0] == h[0]);
        assert(tri_lower[k][1] == h[0] && tri_diag_in[k][1] == 2.0f * h[0]);
        assert(tri_right[k][0] == 3.0f * (pts[1][k] - pts[0][k]));
        assert(tri_right[k][1] == 3.0f * (pts[1][k] - pts[0][k]));
    }
    assert(pow_calls == 2); /* one cube per component */
    assert(count == 3);
    check_sample(x, y, 0, pts, h, 0, 0.0f);
    check_sample(x, y, 1, pts, h, 0, 0.1f);
    check_sample(x, y, 2, pts, h, 0, 0.1f + 0.1f);
    check_sample(x, y, 3, pts, h, 0, 0.1f + 0.1f + 0.1f);
}

/* n = 3, e == 1: the inner segment stops at h - 0.1 (t = 0, 0.1 for h = 0.25; 0.2 > 0.15),
 * the last one at h (t = 0, 0.1 for h = 0.15). Middle row and its right-hand side. */
static void check_three_points_fixed_step(void)
{
    float pts[3][2] = {{0.0f, 0.0f}, {2.0f, 1.0f}, {5.0f, -3.0f}};
    float h[2] = {0.25f, 0.15f};
    float x[64], y[64];
    short count = 99;
    int k;

    reset();
    Taku_spline2(x, y, &count, pts, h, 3, 1, 4.0f);
    assert(tri_calls == 2 && rhs_calls == 2 && sqrt_calls == 0);
    for (k = 0; k < 2; k++) {
        assert(tri_n[k] == 3);
        assert(tri_diag_in[k][0] == 2.0f * h[0] && tri_upper[k][0] == h[0]);
        assert(tri_lower[k][1] == h[1] && tri_diag_in[k][1] == 2.0f * (h[0] + h[1]) && tri_upper[k][1] == h[0]);
        assert(tri_lower[k][2] == h[1] && tri_diag_in[k][2] == 2.0f * h[1]);
        assert(tri_right[k][0] == 3.0f * (pts[1][k] - pts[0][k]));
        assert(tri_right[k][2] == 3.0f * (pts[2][k] - pts[1][k]));
        assert(tri_right[k][1] == pts[2][k] - pts[0][k] + 0.5f);
        assert(rhs_args[k][0] == pts[0][k] && rhs_args[k][1] == pts[1][k] && rhs_args[k][2] == pts[2][k]);
        assert(rhs_args[k][3] == h[0] && rhs_args[k][4] == h[1]);
    }
    assert(count == 3);
    check_sample(x, y, 0, pts, h, 0, 0.0f);
    check_sample(x, y, 1, pts, h, 0, 0.1f);
    check_sample(x, y, 2, pts, h, 1, 0.0f);
    check_sample(x, y, 3, pts, h, 1, 0.1f);
}

/* e != 1: num = ftoi(sqrt(dx^2 + dy^2) / scale) per segment; inner segments take k < num
 * samples at t = k * h / num, the last one k <= num. */
static void check_counted_samples(void)
{
    float pts[3][2] = {{0.0f, 0.0f}, {3.0f, 4.0f}, {4.0f, 2.0f}};
    float h[2] = {1.5f, 0.5f};
    float x[64], y[64];
    short count = 99;

    reset();
    sqrt_results[0] = 7.9f; /* / 2 = 3.95 -> 3 */
    sqrt_results[1] = 2.5f; /* / 2 = 1.25 -> 1 */
    Taku_spline2(x, y, &count, pts, h, 3, 0, 2.0f);
    assert(sqrt_calls == 2 && ftoi_calls == 2);
    assert(sqrt_args[0] == 25.0f && sqrt_args[1] == 5.0f);
    assert(ftoi_args[0] == 7.9f / 2.0f && ftoi_args[1] == 2.5f / 2.0f);
    assert(pow_calls == 2 * 2 + 2 * 2); /* two cubes and two squares per segment */
    assert(count == 4);
    check_sample(x, y, 0, pts, h, 0, 0.0f);
    check_sample(x, y, 1, pts, h, 0, 1.0f * h[0] / 3.0f);
    check_sample(x, y, 2, pts, h, 0, 2.0f * h[0] / 3.0f);
    check_sample(x, y, 3, pts, h, 1, 0.0f);
    check_sample(x, y, 4, pts, h, 1, h[1]);
}

/* num = 0: an inner segment writes nothing; the last still writes its k = 0 sample, whose
 * t is 0 / 0 (not a number on the host). A larger e behaves like 0. */
static void check_zero_count(void)
{
    float pts[3][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {2.0f, 0.0f}};
    float h[2] = {1.0f, 1.0f};
    float x[64], y[64];
    short count = 99;

    reset();
    x[0] = y[0] = 123.0f;
    x[1] = 456.0f;
    sqrt_results[0] = 0.5f;
    sqrt_results[1] = 0.5f;
    Taku_spline2(x, y, &count, pts, h, 3, 2, 1.0f);
    assert(sqrt_calls == 2);
    assert(count == 0);
    assert(x[0] != x[0] && y[0] != y[0]);
    assert(x[1] == 456.0f);
}

int main(void)
{
    check_two_points_fixed_step();
    check_three_points_fixed_step();
    check_counted_samples();
    check_zero_count();
    puts("spline_paths: ok");
    return 0;
}

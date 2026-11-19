/* Synthetic host tests for the key_tracks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/key_tracks.h"

/* Five records and a sixth that the bisection reads but never blends. */
static struct DotrKeyTriple triple[6] = {
    {0, 8.0f, 80.0f, 800.0f},   {10, 16.0f, 160.0f, 1600.0f}, {20, 32.0f, 320.0f, 3200.0f},
    {30, 64.0f, 640.0f, 6400.0f}, {40, 128.0f, 1280.0f, 12800.0f}, {50, -1.0f, -1.0f, -1.0f},
};
static struct DotrKeySingle single[6] = {
    {0, 8.0f}, {10, 16.0f}, {20, 32.0f}, {30, 64.0f}, {40, 128.0f}, {50, -1.0f},
};

static void check(int n, float t, float want)
{
    struct DotrKeyOut out = {-7.0f, -7.0f, -7.0f};

    lSzView_CalcInterpCameraMotion(&out, triple, n, t);
    assert(out.x == want && out.y == want * 10.0f && out.z == want * 100.0f);
    assert(lSzView_CalcInterpCameraMotion1(single, n, t) == want);
}

int main(void)
{
    struct DotrKeyTriple wide3[2] = {{0x80000000u, 8.0f, 16.0f, 32.0f}, {0xc0000000u, 16.0f, 32.0f, 64.0f}};
    struct DotrKeySingle wide1[2] = {{0x80000000u, 8.0f}, {0xc0000000u, 16.0f}};
    struct DotrKeyOut out;

    /* Fewer than three records: no bisection. One record is copied, two are blended. */
    check(1, 5.0f, 8.0f);
    check(2, 2.5f, 10.0f);
    check(2, 0.0f, 8.0f);
    /* Bisection: the upper bound moves down, then the bracket is found. */
    check(5, 12.5f, 20.0f);
    /* The lower bound moves up, then the bracket is found. */
    check(5, 35.0f, 96.0f);
    /* The upper bound moves down until the midpoint meets the lower bound. */
    check(5, 5.0f, 12.0f);
    /* The lower bound reaches the last record (the sixth time is read): copied. */
    check(5, 60.0f, 128.0f);
    /* The last record brackets against the sixth time: copied, not blended. */
    check(5, 45.0f, 128.0f);
    /* A time exactly on a record. */
    check(5, 20.0f, 32.0f);
    check(3, 5.0f, 12.0f);

    /* Times with the top bit set are converted as unsigned. */
    lSzView_CalcInterpCameraMotion(&out, wide3, 2, 2415919104.0f);
    assert(out.x == 10.0f && out.y == 20.0f && out.z == 40.0f);
    assert(lSzView_CalcInterpCameraMotion1(wide1, 2, 2415919104.0f) == 10.0f);
    return 0;
}

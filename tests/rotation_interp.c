/* Synthetic host tests for the rotation_interp group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/rotation_interp.h"

/* Assumed semantics, exact on the quarter turns only (the tests use no other phase). */
static int trig_calls;
float RhMath_SinI(int phase)
{
    trig_calls++;
    switch (phase & 0xffff) {
    case 0: return 0.0f;
    case 0x4000: return 1.0f;
    case 0x8000: return 0.0f;
    case 0xc000: return -1.0f;
    }
    assert(0);
    return 0.0f;
}
float RhMath_CosI(int phase)
{
    trig_calls++;
    switch (phase & 0xffff) {
    case 0: return 1.0f;
    case 0x4000: return 0.0f;
    case 0x8000: return -1.0f;
    case 0xc000: return 0.0f;
    }
    assert(0);
    return 0.0f;
}
/* Arctangent (y, x) on the axes only. */
static int atan_calls;
float func_001a6c18(float a, float b)
{
    atan_calls++;
    if (a == 0.0f && b > 0.0f) {
        return 0.0f;
    }
    if (a == 0.0f && b < 0.0f) {
        return 3.1415927f;
    }
    if (b == 0.0f && a > 0.0f) {
        return 1.5707964f;
    }
    if (b == 0.0f && a < 0.0f) {
        return -1.5707964f;
    }
    assert(0);
    return 0.0f;
}

static int run(int fx, int fy, int fz, int tx, int ty, int tz, float w0, float w1, int ox, int oy, int oz)
{
    int from[3];
    int to[3];
    int out[4];

    from[0] = fx;
    from[1] = fy;
    from[2] = fz;
    to[0] = tx;
    to[1] = ty;
    to[2] = tz;
    out[3] = 0x5a5a;
    trig_calls = atan_calls = 0;
    RhGeom_RoationComplexInterpolate(out, from, to, w0, w1);
    assert(trig_calls == 6 && atan_calls == 2 && out[3] == 0x5a5a);
    return out[0] == ox && out[1] == oy && out[2] == oz;
}

/* One case per branch outcome: the direct form chosen (strictly smaller sum; a tie is not
 * built with axis phases), the equivalent form chosen; each component more than half a turn
 * above and below the start, a negative and a non-negative difference; the low
 * halfword of the input words; the rounding back to 16 bits. */
static void check_interpolate(void)
{
    /* identity: target equals the start, direct form */
    assert(run(0, 0, 0, 0, 0, 0, 0.5f, 0.5f, 0, 0, 0));
    /* half turn on x: the direct form (sum 0x80000) beats the equivalent (0x100000) */
    assert(run(0, 0, 0, 0x8000, 0, 0, 0.5f, 0.5f, 0x4000, 0, 0));
    /* half turns on x and z: the equivalent form (0, half turn, 0; sum 0x80000) wins */
    assert(run(0, 0, 0, 0x8000, 0, 0x8000, 0.0f, 1.0f, 0, 0x8000, 0));
    /* three quarters above the start: moved down a turn, wraps back to 0xc000 */
    assert(run(0, 0, 0, 0xc000, 0, 0, 0.0f, 1.0f, 0xc000, 0, 0));
    /* three quarters below the start: moved up a turn; the equivalent x is a quarter below
     * (negative difference) */
    assert(run(0xc000, 0, 0, 0, 0, 0, 0.5f, 0.5f, 0xe000, 0, 0));
    /* only the low halfword is used, and the result rounds to nearest at 1/16 */
    assert(run(0x10001, 0, 0, 0, 0, 0, 1.0f, 0.0f, 1, 0, 0));
    assert(run(0x10001, 0, 0, 0, 0, 0, 0.5f, 0.0f, 1, 0, 0));
    assert(run(0x10001, 0, 0, 0, 0, 0, 0.25f, 0.0f, 0, 0, 0));
}

int main(void)
{
    check_interpolate();
    return 0;
}

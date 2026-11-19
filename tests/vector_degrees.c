/* Synthetic host tests for the vector_degrees group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/vector_degrees.h"

static int abs_calls, abs_args[2], abs_bias;

/* Host stand-in for the library abs. abs_bias forces a zero sum to reach the second guard. */
int func_00195070(int v)
{
    abs_args[abs_calls++ & 1] = v;
    if (abs_bias) {
        return 0;
    }
    return v < 0 ? -v : v;
}

static int angle(long x, long y)
{
    abs_calls = 0;
    return tanD_360_(x, y);
}

int main(void)
{
    /* Zero vector: no abs call. */
    assert(angle(0, 0) == 0 && abs_calls == 0);
    /* Non-zero vector whose magnitudes sum to zero: the second guard. */
    abs_bias = 1;
    assert(angle(3, 4) == 0 && abs_calls == 2 && abs_args[0] == 3 && abs_args[1] == 4);
    abs_bias = 0;

    /* Axes: x > 0 belongs to the first branch; x == 0 to the "else" side. */
    assert(angle(5, 0) == 0);
    assert(angle(0, 5) == 90);
    assert(angle(-5, 0) == 180);
    assert(angle(0, -5) == 270);

    /* y >= 0, x > 0. */
    assert(angle(7, 7) == 45);
    assert(angle(10, 5) == 22);            /* 5 * 45 / 10 = 22 */
    assert(angle(5, 10) == 90 - 22);
    /* y >= 0, x <= 0. */
    assert(angle(-7, 7) == 135);
    assert(angle(-10, 5) == 180 - 22);
    assert(angle(-5, 10) == 22 + 90);
    /* y < 0, x > 0. */
    assert(angle(7, -7) == 315);
    assert(angle(10, -5) == 360 - 22);
    assert(angle(5, -10) == 22 + 270);
    assert(angle(100, -1) == 360);         /* 45 / 100 truncates to 0 */
    /* y < 0, x <= 0. */
    assert(angle(-7, -7) == 225);
    assert(angle(-10, -5) == 22 + 180);
    assert(angle(-5, -10) == 270 - 22);

    /* Truncating 64-bit division: 1 * 45 / 2 = 22, 2 * 45 / 3 = 30. */
    assert(angle(2, 1) == 22 && angle(3, 2) == 30);
    return 0;
}

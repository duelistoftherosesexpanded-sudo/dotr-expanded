/* Synthetic host tests for the matrix_rot group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/matrix_rot.h"

static int trig_calls;
static int trig_args[8][3];

float SIN_COS_INRAD(int unit, short angle, int cosine)
{
    assert(trig_calls < 8);
    trig_args[trig_calls][0] = unit;
    trig_args[trig_calls][1] = angle;
    trig_args[trig_calls][2] = cosine;
    trig_calls++;
    return cosine ? 0.5f : 0.25f;
}

static struct DotrPaintVector row;

static int rotate(int axis)
{
    static const int words[4] = { 0x12345, 0x18000, -2, 77 };
    struct DotrPaintVector v;

    memcpy(&v, words, sizeof v);
    row.x = 2.0f, row.y = 3.0f, row.z = 5.0f, row.w = 9.0f;
    trig_calls = 0;
    return MATRIX_XYZ_ININ33(&row, &v, axis, 44);
}

/* Four calls with the unit and this angle, two for the sine and two for the cosine. The order
 * inside one C expression is the compiler's choice, so the host test does not fix it. */
static int called(int angle)
{
    int n, cosines = 0;

    for (n = 0; n < trig_calls; n++) {
        if (trig_args[n][0] != 44 || trig_args[n][1] != angle) {
            return 0;
        }
        cosines += trig_args[n][2] == 1;
    }
    return trig_calls == 4 && cosines == 2;
}

/* MATRIX_XYZ_ININ33: branch coverage (one case per axis value and one that matches none).
 * The fourth float written back is an unset local and is not checked. */
static void check_rotate(void)
{
    /* axis 0: x and z from angle word 1, cut to 16 bits */
    assert(rotate(0) == 0 && called(-32768));
    assert(row.x == -0.25f && row.y == 3.0f && row.z == 3.0f);
    /* axis 1: y and z from angle word 0 */
    assert(rotate(1) == 0 && called(0x2345));
    assert(row.x == 2.0f && row.y == 2.75f && row.z == 1.75f);
    /* axis 2: x and y from angle word 2 */
    assert(rotate(2) == 0 && called(-2));
    assert(row.x == 1.75f && row.y == 1.0f && row.z == 5.0f);
    /* any other axis: no call, the row is left alone */
    assert(rotate(3) == 0 && trig_calls == 0 && row.x == 2.0f && row.y == 3.0f && row.z == 5.0f && row.w == 9.0f);
    assert(rotate(-1) == 0 && trig_calls == 0 && row.w == 9.0f);
}

int main(void)
{
    check_rotate();
    return 0;
}

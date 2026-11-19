/* Synthetic host tests for the rh_invert_matrix group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/rh_invert_matrix.h"

static int near(float a, float b)
{
    float d = a - b;

    return d < 1e-4f && d > -1e-4f;
}

/* Checks a * b == identity. */
static void check_inverse(const struct DotrRhInvMatrix *a, const struct DotrRhInvMatrix *b)
{
    int i, j, k;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            float s = 0.0f;

            for (k = 0; k < 4; k++) {
                s += a->m[i][k] * b->m[k][j];
            }
            assert(near(s, i == j ? 1.0f : 0.0f));
        }
    }
}

int main(void)
{
    static const struct DotrRhInvMatrix identity = { { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } } };
    /* Determinant 1 (unimodular), every entry of the inverse an integer. */
    static const struct DotrRhInvMatrix general = { { { 2, 1, 0, 0 }, { 1, 1, 0, 0 }, { 0, 3, 1, 0 }, { 5, 0, 2, 1 } } };
    /* A rotation-and-translation matrix, the usual use. */
    static const struct DotrRhInvMatrix affine = { { { 0, 1, 0, 0 }, { -1, 0, 0, 0 }, { 0, 0, 1, 0 }, { 10, 20, 30, 1 } } };
    static const struct DotrRhInvMatrix singular = { { { 1, 2, 3, 4 }, { 2, 4, 6, 8 }, { 0, 1, 0, 1 }, { 1, 0, 1, 0 } } };
    struct DotrRhInvMatrix out, scaled;
    int i, j;

    RhGeom_InvertMatrix(&out, (void *)&identity);
    /* Compared by value: off-diagonal cofactors multiplied by -1.0 give -0.0. */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            assert(out.m[i][j] == identity.m[i][j]);
        }
    }

    RhGeom_InvertMatrix(&out, (void *)&general);
    check_inverse(&general, &out);
    /* Inverse of the leading 2x2 block {{2,1},{1,1}} is {{1,-1},{-1,2}}. */
    assert(out.m[0][0] == 1.0f && out.m[0][1] == -1.0f && out.m[1][0] == -1.0f && out.m[1][1] == 2.0f);
    assert(out.m[3][3] == 1.0f && out.m[0][2] == 0.0f);

    RhGeom_InvertMatrix(&out, (void *)&affine);
    check_inverse(&affine, &out);
    assert(near(out.m[3][0], -20.0f) && near(out.m[3][1], 10.0f) && near(out.m[3][2], -30.0f));

    memset(&scaled, 0, sizeof(scaled));
    for (i = 0; i < 4; i++) {
        scaled.m[i][i] = 4.0f;
    }
    RhGeom_InvertMatrix(&out, &scaled);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            assert(out.m[i][j] == (i == j ? 0.25f : 0.0f));
        }
    }

    /* Determinant 0: dst is left as it was. */
    memset(&out, 0x5a, sizeof(out));
    scaled = out;
    RhGeom_InvertMatrix(&out, (void *)&singular);
    assert(memcmp(&out, &scaled, sizeof(out)) == 0);
    return 0;
}

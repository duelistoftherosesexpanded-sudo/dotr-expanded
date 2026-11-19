/* Synthetic host tests for the ring_setup group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/ring_setup.h"

/* Every callee is a stub. The two phase functions answer exact floats made
 * from the phase (k * 2048), so the products are exact. */
struct DotrRing prim_data;
struct DotrRingVector3 model_data;
static int head_calls, cos_calls, sin_calls, copy_calls;

void Taku_Coord_Init(struct DotrRingTriple *m)
{
    assert(m == &prim_data.head && prim_data.unknown_a0 == 0x55555555);
    head_calls++;
}
float RhMath_CosI(int phase)
{
    assert(phase % 2048 == 0 && phase >= 0 && phase <= 32 * 2048);
    cos_calls++;
    return (float)(phase / 2048);
}
float RhMath_SinI(int phase)
{
    assert(phase % 2048 == 0 && sin_calls + 1 == cos_calls);
    sin_calls++;
    return (float)(phase / 2048) + 0.5f;
}
void Taku_Vector_Set(struct DotrRingVector3 *a, struct DotrRingVector3 *b)
{
    assert(a == &prim_data.cells[copy_calls].unknown_00 && b == &model_data);
    /* the circle rows are complete before the first cell */
    assert(cos_calls == 48 && sin_calls == 48);
    copy_calls++;
}

static void expect_row(const float *row, float x, float z)
{
    assert(row[0] == x && row[1] == 8.65f && row[2] == z && row[3] == 0.0f);
}

/* Taku_Camp_3D_Poly_Init (loop and branch coverage: every group, each of the four row
 * kinds, every cell; the bounds 16 and 4 through the untouched neighbours). */
static void check_ring(void)
{
    static const unsigned char colors[4][4] = { { 128, 0, 0, 128 }, { 0, 128, 0, 0 }, { 0, 0, 128, 0 }, { 128, 128, 128, 128 } };
    int i, k;

    assert(offsetof(struct DotrRing, rows) == 0xec && offsetof(struct DotrRing, cells) == 0x4ec);
    assert(sizeof(struct DotrRingCell) == 52 && offsetof(struct DotrRing, unknown_a0) == 0xa0);
    memset(&prim_data, 0x55, sizeof(prim_data));
    Taku_Camp_3D_Poly_Init();
    assert(head_calls == 1 && cos_calls == 48 && sin_calls == 48 && copy_calls == 16);
    assert(prim_data.unknown_a0 == 0);
    /* bytes on both sides of the word and before the rows are untouched */
    assert(prim_data.unknown_30[0x6f] == 0x55 && prim_data.unknown_a4[0] == 0x55 && prim_data.unknown_a4[0x47] == 0x55);
    for (i = 0; i < 16; i++) {
        k = 2 * i;
        expect_row(prim_data.rows[i][0], 5.0f * (float)k, 5.0f * (float)k + 2.5f);
        expect_row(prim_data.rows[i][1], 5.0f * (float)(k + 1), 5.0f * (float)(k + 1) + 2.5f);
        expect_row(prim_data.rows[i][2], 0.0f, 0.0f);
        expect_row(prim_data.rows[i][3], 5.0f * (float)(k + 2), 5.0f * (float)(k + 2) + 2.5f);
        assert(prim_data.cells[i].unknown_20 == 0);
        assert(memcmp(prim_data.cells[i].color, colors, 16) == 0);
        /* the cell head and the bytes after it are left to 0x21bb70 / untouched */
        assert(prim_data.cells[i].unknown_0c[0] == 0x55 && prim_data.cells[i].unknown_0c[19] == 0x55);
    }
}

int main(void)
{
    check_ring();
    return 0;
}

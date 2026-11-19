#include "dotr/ee/pool_table.h"

/* The body of SzView_GetLookVector without the test of the output pointer, as in
 * SzView_GetLookMatrix. */
static inline int block_angles(int index, int *out)
{
    float *block;
    float value;

    if (index < 0 || index > 0)
        return 0;
    block = (float *)gSzView_hView[index];
    if (block == 0)
        return 0;
    value = block[2];
    out[0] = -func_001a22c0(32768.0f * value / 3.1415927f);
    value = block[1];
    out[1] = 16384 - func_001a22c0(32768.0f * value / 3.1415927f);
    out[3] = 0;
    out[2] = 0;
    return 1;
}

/* The block's word +64, or 0 without a block: inlined in the target. */
static inline int block_word(int index)
{
    int *block;

    if (index < 0 || index > 0)
        return 0;
    block = gSzView_hView[index];
    if (block == 0)
        return 0;
    return block[16];
}

int SzView_GetLookMatrixWithRoll(int index, float *m)
{
    struct DotrPoolAngles extra = data_00293060;
    int angles[4];
    float turn[16];

    extra.v[2] = -block_word(index);
    if (m == 0)
        return 0;
    if (!block_angles(index, angles))
        return 0;
    RhGeom_GetRotXYZMatrix(m, angles);
    RhGeom_GetRotXYZMatrix(turn, extra.v);
    RhGeom_MulMatrix(m, turn);
    return 1;
}

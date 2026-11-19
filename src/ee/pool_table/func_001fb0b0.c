#include "dotr/ee/pool_table.h"

/* The body of SzView_GetLookVector as the target has it inlined here: without the test
 * of the output pointer, which is a local array in the only use. */
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

int SzView_GetLookMatrix(int index, float *m)
{
    int angles[4];

    if (m == 0)
        return 0;
    if (!block_angles(index, angles))
        return 0;
    RhGeom_GetRotXYZMatrix(m, angles);
    return 1;
}

#include "dotr/ee/lk_math.h"

int LkChangeAxisY(float *a, float *b, int *out)
{
    /* MATCHING DEVICE: declared together, assigned in x, y, z order. */
    float dx, dy, dz;
    dx = a[0] - b[0];
    dy = a[1] - b[1];
    dz = a[2] - b[2];

    out[1] = func_001a22c0(func_001a6c18(dx, dz) / 6.2831855f * 65536.0f) % 0x10000;
    out[0] = func_001a22c0(func_001a6c18(func_001a6c60(dx * dx + dz * dz), dy) / 6.2831855f * 65536.0f) % 0x10000;
    out[3] = 0;
    out[2] = 0;
    return 0;
}

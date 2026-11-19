#include "dotr/ee/pool_table.h"

int SzView_GetLookVector(int index, int *out)
{
    float *block;
    float value;

    if (index < 0 || index > 0)
        return 0;
    block = (float *)gSzView_hView[index];
    if (block == 0)
        return 0;
    if (out == 0)
        return 0;
    value = block[2];
    out[0] = -func_001a22c0(32768.0f * value / 3.1415927f);
    value = block[1];
    out[1] = 16384 - func_001a22c0(32768.0f * value / 3.1415927f);
    out[3] = 0;
    out[2] = 0;
    return 1;
}

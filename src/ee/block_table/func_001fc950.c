#include "dotr/ee/block_table.h"

void SzView_IV2V(int *in, float *out)
{
    out[0] = in[0];
    out[1] = 3.1415927f * in[1] / 32768.0f;
    out[2] = 3.1415927f * in[2] / 32768.0f;
    out[3] = in[3];
}

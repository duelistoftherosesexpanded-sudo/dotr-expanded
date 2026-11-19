#include "dotr/ee/obj_vectors.h"

void SIN_IN_PSYS2(struct DotrSineBlocks *out, int step, int count)
{
    short i;
    float r;

    for (i = 0; i < count; i++) {
        r = (float)(i * step) * 3.1415927f / 180.0f;
        out->sin_at[i] = func_001a6a88(r);
        out->cos_at[i] = func_001a6898(r);
        r = (float)(step + i * step) * 3.1415927f / 180.0f;
        out->sin_next[i] = func_001a6a88(r);
        out->cos_next[i] = func_001a6898(r);
    }
}

#include "dotr/ee/obj_vectors.h"

void SIN_IN_PSYS(float *out)
{
    short i;

    for (i = 0; i < 91; i++) {
        *out++ = func_001a6a88(((float)i * 3.1415927f) / 180.0f);
    }
}

#include "dotr/ee/obj_vectors.h"

int MODEL_ON(int model, int unused, int on)
{
    float scale[4];
    (void)unused;

    if (SzEffect_IsModelStandby2(model) != 0 && on != 0) {
        scale[0] = 1.0f;
        scale[1] = 1.0f;
        scale[2] = 1.0f;
        SzEffect_SetModelScale2(model, scale);
    }
    return 0;
}

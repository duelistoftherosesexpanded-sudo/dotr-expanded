#include "dotr/ee/obj_vectors.h"

int MODEL_OFF(int model, int unused_1, int unused_2, int on)
{
    float scale[4];
    (void)unused_1;
    (void)unused_2;

    if (SzEffect_IsModelStandby2(model) != 0 && on != 0) {
        SzEffect_GetModelScale2(model, (struct DotrObjectVector4 *)scale);
        scale[0] -= 0.18f;
        if (scale[0] < 0.0f) {
            scale[0] = 0.0f;
        }
        scale[1] -= 0.18f;
        if (scale[1] < 0.0f) {
            scale[1] = 0.0f;
        }
        scale[2] -= 0.18f;
        if (scale[2] < 0.0f) {
            scale[2] = 0.0f;
        }
        SzEffect_SetModelScale2(model, scale);
    }
    return 0;
}

#include "dotr/ee/obj_vectors.h"

int MODEL_SCL_ON_1(int model, float step)
{
    float scale[4];

    if (SzEffect_IsModelStandby2(model) != 0) {
        SzEffect_GetModelScale2(model, (struct DotrObjectVector4 *)scale);
        scale[0] += step;
        if (scale[0] > 1.0f) {
            scale[0] = 1.0f;
        }
        scale[1] += step;
        if (scale[1] > 1.0f) {
            scale[1] = 1.0f;
        }
        scale[2] += step;
        if (scale[2] > 1.0f) {
            scale[2] = 1.0f;
        }
        SzEffect_SetModelScale2(model, scale);
    }
    return 0;
}

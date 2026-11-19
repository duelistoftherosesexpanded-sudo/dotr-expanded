#include "dotr/ee/fx_calc.h"

int GGet_ModelPosZSign(int model)
{
    float pos[4];
    int sign = 1;

    SzEffect_GetModelPos2(model, pos);
    if (pos[2] < 0.0f) {
        sign = -1;
    }
    return sign;
}

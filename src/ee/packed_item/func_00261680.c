#include "dotr/ee/packed_item.h"

void TaEffect_SetOuchMotion(int packed)
{
    float v;

    if (SzEffect_GetAnim2(packed) != 4) {
        SzEffect_SetAnimIntr2(packed, 4);
        return;
    }
    v = SzEffect_GetAnimFrame2(packed);
    if (!(v < 16.0f)) {
        SzEffect_SetAnimFrame2(packed, 11.0f);
    } else if (!(v <= 11.0f)) {
        if (func_0019a5c8() % 3 == 0) {
            SzEffect_SetAnimFrame2(packed, v - 2.0f);
        }
    }
}

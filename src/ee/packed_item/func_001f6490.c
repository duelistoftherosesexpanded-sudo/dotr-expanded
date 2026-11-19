#include "dotr/ee/packed_item.h"

int SzEffect_GetModelRgba2(int packed, struct DotrObjectQuad *q)
{
    return SzEffect_GetModelRgbaX(packed, q) >= 0 ? 1 : 0;
}

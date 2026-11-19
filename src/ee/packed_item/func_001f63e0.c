#include "dotr/ee/packed_item.h"

int SzEffect_SetModelRgba2(int packed, struct DotrObjectQuad *q)
{
    return SzEffect_SetModelRgbaX(packed, -1, q);
}

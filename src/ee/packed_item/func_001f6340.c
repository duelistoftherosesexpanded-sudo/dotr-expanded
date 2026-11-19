#include "dotr/ee/packed_item.h"

int SzEffect_GetModelScale2(int packed, struct DotrObjectVector4 *v)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    v->z = 0.0f;
    v->y = 0.0f;
    v->x = 0.0f;
    v->unknown_w = 1.0f;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    packed = SzDuel_GetUnitModelID(row, column);
    if (packed < 0) {
        return 0;
    }
    return SzModel_GetScale(packed, v);
}

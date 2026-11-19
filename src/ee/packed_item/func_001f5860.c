#include "dotr/ee/packed_item.h"

float SzEffect_GetRadius2(int packed)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0.0f;
    }
    packed = SzDuel_GetUnitModelID(row, column);
    if (packed < 0) {
        return 0.0f;
    }
    return SzModel_GetRadius(packed);
}

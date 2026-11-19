#include "dotr/ee/packed_item.h"

int SzEffect_GetCamDist2(int packed)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return -1;
    }
    packed = SzDuel_GetUnitModelID(row, column);
    if (packed < 0) {
        return -1;
    }
    return SzDuel_GetCamDist(SzDuel_Xax2New(SzModel_GetModelNo(packed)));
}

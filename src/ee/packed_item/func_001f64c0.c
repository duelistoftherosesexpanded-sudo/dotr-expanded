#include "dotr/ee/packed_item.h"

int SzEffect_GetModelRgbaX(int packed, struct DotrObjectQuad *q)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    q->unknown_3 = 128;
    q->unknown_0 = 128;
    q->unknown_1 = 128;
    q->unknown_2 = 128;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return -1;
    }
    packed = SzDuel_GetUnitModelID(row, column);
    if (packed < 0) {
        return -1;
    }
    return SzModel_GetRgba(packed, q);
}

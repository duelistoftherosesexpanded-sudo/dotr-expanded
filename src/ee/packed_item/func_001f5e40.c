#include "dotr/ee/packed_item.h"

int SzEffect_GetCardRgba2(int packed, struct DotrStateCellQuad *v)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    v->unknown_3 = 128;
    v->unknown_0 = 128;
    v->unknown_1 = 128;
    v->unknown_2 = 128;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    return SzDuel_GetCardRgba(row, column, v);
}

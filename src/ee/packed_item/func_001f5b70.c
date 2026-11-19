#include "dotr/ee/packed_item.h"

int SzEffect_SetCardPos2(int packed, struct DotrStateVector *v)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    return SzDuel_SetCardPos(row, column, v);
}

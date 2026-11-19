#include "dotr/ee/packed_item.h"

int SzEffect_SetRefID2(int packed, int value, int unknown)
{
    struct DotrPackedTable *t = &gSzEffect_el;
    int row = packed >> 8;
    int column = packed & 0xff;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return -1;
    }
    packed = SzDuel_GetUnitModelID(row, column);
    if (packed < 0) {
        return -1;
    }
    if (value >= 0) {
        value = ((t->unknown_340 << 8) & 0x7f00) | (value & 0xff);
    }
    return SzModel_SetRefID(packed, value, unknown);
}

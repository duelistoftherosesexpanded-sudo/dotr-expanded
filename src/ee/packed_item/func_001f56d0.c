#include "dotr/ee/packed_item.h"

int SzEffect_ClearRefID2(int packed, int value)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    int i;
    int v;

    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    packed = SzDuel_GetUnitModelID(row, column);
    if (packed < 0) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        v = SzModel_InqRefID(packed, i);
        if (v >= 0 && v < 8 && ((v & 0x7f00) >> 8) == value) {
            SzModel_SetRefID_RefNo(packed, -1, i);
        }
    }
    return 1;
}

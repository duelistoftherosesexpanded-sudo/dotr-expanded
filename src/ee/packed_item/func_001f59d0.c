#include "dotr/ee/packed_item.h"

int SzEffect_SetCardBusyFlag2(int packed, int flag)
{
    int row = packed >> 8;
    int column = packed & 0xff;
    if (row < 0 || row >= 3 || column < 0 || column >= 11) {
        return 0;
    }
    return SzDuel_SetCardBusyFlag(row, column, flag);
}

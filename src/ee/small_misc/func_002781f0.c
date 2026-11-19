#include "dotr/ee/small_misc.h"

int TaLogic_IsUnitWin(int unit1OwnerId, int unit2, int col, int row)
{
    return TaLogic_IsUnitWin2(func_001df540_word(unit1OwnerId >> 8, unit1OwnerId & 0xff), unit2, col, row);
}

#include "dotr/ee/position_rules.h"

int TaLogic009_GetEmptyDefensePoint(void)
{
    if (SzDuel_GetMapUnit(2, 2) == 0 && SzDuel_GetField(2, 2) != 8) {
        return 0x202;
    }
    if (SzDuel_GetMapUnit(5, 2) == 0 && SzDuel_GetField(5, 2) != 8) {
        return 0x205;
    }
    return -1;
}

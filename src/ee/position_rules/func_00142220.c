#include "dotr/ee/position_rules.h"

int TaLogic011_IsUseBraveHourGlass(int side)
{
    if (SzDuel_GetLP() < 0x4b0) {
        return 0;
    }
    if (SzDuel_GetUnitMonsterCount(side) >= 4) {
        return 1;
    }
    return 0;
}

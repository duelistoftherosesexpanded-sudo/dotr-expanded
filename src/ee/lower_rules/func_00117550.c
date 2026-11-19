#include "dotr/ee/lower_rules.h"

int TaLogic001_IsUseBraveHourGlass(int side)
{
    int n;

    if (SzDuel_GetLP() < 1500) {
        return 0;
    }
    n = SearchUnitByRange(0, 0, side, 1, 0x2001C, 14);
    n += SearchUnitByRange(0, 0, side, 1, 0x2001C, 14);
    if (n != 0 && SzDuel_GetUnitMonsterCount(side) >= 4) {
        return 1;
    }
    return 0;
}

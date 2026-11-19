#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__0011aa10(int side)
{
    if (SzDuel_GetLP() > 1500) {
        if (SzDuel_GetUnitMonsterCount(side) >= 3) {
            return 0;
        }
        return 0;
    }
    return 0;
}

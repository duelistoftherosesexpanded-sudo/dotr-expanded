#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__0013da70(int side)
{
    if (SzDuel_GetLP() > 1000) {
        if (SzDuel_GetUnitMonsterCount(side) >= 5) {
            return 0;
        }
        return 0;
    }
    return 0;
}

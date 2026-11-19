#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__00162580(int side)
{
    if (SzDuel_GetLP() > 1200) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 500;
        }
        return 1000;
    }
    return 2000;
}

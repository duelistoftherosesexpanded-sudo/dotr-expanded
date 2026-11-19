#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__00172310(int side)
{
    if (SzDuel_GetLP() > 1200) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 1000;
        }
        return 2000;
    }
    return 2400;
}

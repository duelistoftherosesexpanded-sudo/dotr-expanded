#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__0014f9b0(int side)
{
    if (SzDuel_GetLP() > 1200) {
        if (SzDuel_GetUnitMonsterCount(side) >= 5) {
            return 750;
        }
        return 1500;
    }
    return 2400;
}

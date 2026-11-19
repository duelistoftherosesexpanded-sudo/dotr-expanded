#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__0014b330(int side)
{
    if (SzDuel_GetLP() > 1500) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 750;
        }
        return 1500;
    }
    return 2000;
}

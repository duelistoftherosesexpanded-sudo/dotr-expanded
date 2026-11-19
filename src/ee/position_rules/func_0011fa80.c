#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__0011fa80(int side)
{
    if (SzDuel_GetLP() > 1000) {
        if (SzDuel_GetUnitMonsterCount(side) >= 3) {
            return 500;
        }
        return 1000;
    }
    return 2000;
}

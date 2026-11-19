#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__0010eda0(int side)
{
    if (SzDuel_GetLP() > 1500) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 0;
        }
        return 0;
    }
    return 0;
}

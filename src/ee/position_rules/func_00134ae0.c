#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__00134ae0(int side)
{
    if (SzDuel_GetLP() > 1000) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 750;
        }
        return 1500;
    }
    return 1700;
}

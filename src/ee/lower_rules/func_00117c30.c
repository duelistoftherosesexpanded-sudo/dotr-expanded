#include "dotr/ee/lower_rules.h"

int TaLogic_GetWarnHideLvl__00117c30(int side)
{
    int warnHideLevel;

    if (SzDuel_GetLP() > 1200) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            warnHideLevel = 750;
        } else {
            warnHideLevel = 1500;
        }
        return warnHideLevel;
    }
    return 2500;
}

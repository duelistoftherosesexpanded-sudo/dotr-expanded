#include "dotr/ee/lower_rules.h"

int TaLogic_GetWarnHideLvl__0015d120(int side)
{
    int r;

    if (SzDuel_GetLP() > 1200) {
        if (SzDuel_GetUnitMonsterCount(side) >= 5) {
            r = 750;
        } else {
            r = 1500;
        }
        return r;
    }
    return 2000;
}

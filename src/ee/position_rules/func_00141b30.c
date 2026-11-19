#include "dotr/ee/position_rules.h"

int TaLogic_GetWarnHideLvl__00141b30(int side)
{
    if (SzDuel_GetLP() > 2000) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 0;
        }
        return 0;
    }
    return 0;
}

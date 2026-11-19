#include "dotr/ee/lower_rules.h"

int TaLogic001_IsUseLifeHourGlass(int side)
{
    int i;
    unsigned char need;
    unsigned char have = 0;

    if (SzDuel_GetLP() < 1500) {
        return 0;
    }
    need = TaLogic_GetGraveCardCount(side, 3, 2);
    if (need > 0) {
        for (i = 0; i < 8; i++) {
            if (Ta_GetSummonablePieceWithOrder(side, i, 1) >= 0) {
                have++;
            }
        }
        if (have >= need && 5 - SzDuel_GetUnitMonsterCount(side) >= need) {
            return 1;
        }
    }
    have = 0;
    need = TaLogic_GetGraveCardCount(side, 3, 0);
    if (need > 0) {
        for (i = 0; i < 8; i++) {
            if (Ta_GetSummonablePieceWithOrder(side, i, 1) >= 0) {
                have++;
            }
        }
        if (have >= need && 5 - SzDuel_GetUnitMonsterCount(side) >= need) {
            return 1;
        }
    }
    return 0;
}

#include "dotr/ee/small_misc.h"

int TaLogic_GetUnitNum(int side, int countSpells)
{
    if (countSpells == 1) {
        return SzDuel_GetUnitSpellCount(side);
    }
    return SzDuel_GetUnitMonsterCount(side);
}

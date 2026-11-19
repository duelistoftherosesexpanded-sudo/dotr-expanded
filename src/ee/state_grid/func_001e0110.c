#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitCount(int side)
{
    return SzDuel_GetUnitMonsterCount(side) + SzDuel_GetUnitSpellCount(side);
}

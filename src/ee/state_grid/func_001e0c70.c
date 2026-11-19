#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitLeaderAbility2(struct DotrStateItem *unit, int abil, int extraDataOut)
{
    return SzDuel_GetUnitLeaderAbility3(unit, abil, extraDataOut, 0);
}

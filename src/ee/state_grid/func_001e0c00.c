#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitLeaderAbility(int side, int pos, int abil, int extraDataOut)
{
    return SzDuel_GetUnitLeaderAbility2(dotr_state_item_checked(side, pos), abil, extraDataOut);
}

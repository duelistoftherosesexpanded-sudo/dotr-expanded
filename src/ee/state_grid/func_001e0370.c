#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitDP(int side, int pos, int ignoreBoosts)
{
    return SzDuel_GetUnitDP2(dotr_state_item_checked(side, pos), ignoreBoosts);
}

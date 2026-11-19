#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitLevel(int side, int pos, int ignoreBoosts)
{
    return SzDuel_GetUnitLevel2(dotr_state_item_checked(side, pos), ignoreBoosts);
}

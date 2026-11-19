#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts)
{
    return SzDuel_GetUnitAP2(dotr_state_item_checked(side, pos), ignoreBoosts);
}

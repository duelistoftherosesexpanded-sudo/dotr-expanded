#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitKindOfs(int side, int pos)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) {
        return -1;
    }
    return unit->constInfo ? unit->constInfo->kindOfs : -1;
}

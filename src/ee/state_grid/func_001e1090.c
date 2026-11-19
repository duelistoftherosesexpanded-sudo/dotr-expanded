#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitLoc(int side, int pos)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    return unit ? (unit->locRow << 8) | unit->locCol : -1;
}

#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitKind(int side, int pos, int value)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) {
        return 0;
    }
    unit->kind = value;
    return 1;
}

#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlGoalC(int side, int pos, int value)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) {
        return 0;
    }
    unit->ctlGoalC = value;
    return 1;
}

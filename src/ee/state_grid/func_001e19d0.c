#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlGoalM(int side, int pos, int value)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    if (p == 0) {
        return 0;
    }
    p->ctlGoalM = value;
    return 1;
}

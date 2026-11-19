#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitCtlGoalM(int a, int b)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    return p ? p->ctlGoalM : -1;
}

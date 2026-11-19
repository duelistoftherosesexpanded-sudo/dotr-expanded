#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitCtlGoalC(int a, int b)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    return p ? p->ctlGoalC : -1;
}

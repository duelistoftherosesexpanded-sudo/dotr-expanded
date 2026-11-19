#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitCtlRateC(int side, int pos)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    return p ? p->ctlRateC : -1;
}

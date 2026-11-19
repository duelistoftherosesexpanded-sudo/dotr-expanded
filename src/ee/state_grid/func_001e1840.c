#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlModeC(int a, int b, int value)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    if (p == 0) {
        return 0;
    }
    p->ctlModeC = value;
    return 1;
}

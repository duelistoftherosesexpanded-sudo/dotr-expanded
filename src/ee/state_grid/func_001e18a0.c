#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlModeM(int side, int pos, int value)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    if (p == 0) {
        return 0;
    }
    p->unknown_4f = value;
    return 1;
}

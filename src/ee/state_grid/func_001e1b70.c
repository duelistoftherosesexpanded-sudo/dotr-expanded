#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    if (p == 0) {
        return 0;
    }
    p->unknown_56.unknown_bits_0_1 = pair & 3;
    if (value >= 0) {
        p->ctlRateC = value;
    }
    return 1;
}

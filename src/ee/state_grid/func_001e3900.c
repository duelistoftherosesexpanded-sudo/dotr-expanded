#include "dotr/ee/state_grid.h"

int SzDuel_SetCardBusyFlag(int side, int pos, int flag)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    if (p == 0) {
        return 0;
    }
    p->status.unknown_bit_7 = flag ? 1 : 0;
    return 1;
}

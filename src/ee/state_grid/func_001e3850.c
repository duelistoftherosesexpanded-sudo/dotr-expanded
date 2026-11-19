#include "dotr/ee/state_grid.h"

int SzDuel_SetBusyFlag(int side, int pos, int flag)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    if (p == 0) {
        return 0;
    }
    p->status.unknown_bit_7 = flag ? 1 : 0;
    p->modelStatusFlags.unknown_bit_0 = flag ? 1 : 0;
    return 1;
}

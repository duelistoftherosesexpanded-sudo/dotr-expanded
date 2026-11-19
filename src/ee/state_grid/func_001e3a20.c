#include "dotr/ee/state_grid.h"

int SzDuel_SetModelBusyFlag(int side, int pos, int flag)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    if (p == 0) {
        return 0;
    }
    p->modelStatusFlags.unknown_bit_0 = flag ? 1 : 0;
    return 1;
}

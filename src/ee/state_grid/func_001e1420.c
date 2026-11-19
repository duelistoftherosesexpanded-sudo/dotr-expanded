#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCurseFlag(int a, int b, int flag)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    if (p == 0) {
        return 0;
    }
    p->modelStatusFlags.unknown_bit_3 = flag ? 1 : 0;
    return 1;
}

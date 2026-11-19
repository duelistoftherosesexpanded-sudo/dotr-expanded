#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitCurseFlag(int a, int b)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    if (p == 0) {
        return 0;
    }
    return p->modelStatusFlags.unknown_bit_3 ? 1 : 0;
}

#include "dotr/ee/state_grid.h"

int SzDuel_IsUnitMovableOnLabyrinth(int side, int pos)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    if (p == 0) {
        return 0;
    }
    return (GetMoveRev(p, p->locCol, p->locRow) >> 4) ? 1 : 0;
}

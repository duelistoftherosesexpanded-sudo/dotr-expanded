#include "dotr/ee/state_grid.h"

int SzDuel_CalcUnitMovable(int side, int pos)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    return p ? GetMoveRev(p, p->locCol, p->locRow) & 0xf : 0;
}

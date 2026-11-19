#include "dotr/ee/state_grid.h"

int SzDuel_GetCardRgba(int side, int pos, struct DotrStateCellQuad *v)
{
    struct DotrStateItem *p = dotr_state_item_checked(side, pos);
    struct DotrStateItemObject *o;
    if (p == 0) {
        return 0;
    }
    o = p->cardModel;
    if (o == 0) {
        return 0;
    }
    if (v) {
        *v = o->unknown_40;
    }
    return 1;
}

#include "dotr/ee/state_grid.h"

int SzDuel_SetCardPos(int a, int b, struct DotrStateVector *cardPos)
{
    struct DotrStateItem *p = dotr_state_item_checked(a, b);
    struct DotrStateItemObject *o;
    if (p == 0) {
        return 0;
    }
    o = p->cardModel;
    if (o == 0) {
        return 0;
    }
    if (cardPos) {
        o->unknown_00 = *cardPos;
    }
    return 1;
}

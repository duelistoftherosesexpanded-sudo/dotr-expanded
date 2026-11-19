#include "dotr/ee/state_grid.h"

int SzDuel_SetCardScale(int a, int b, struct DotrStateVector *v)
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
    if (v) {
        o->unknown_30 = *v;
    }
    return 1;
}

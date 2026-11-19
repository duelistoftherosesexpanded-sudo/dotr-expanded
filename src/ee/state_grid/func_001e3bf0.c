#include "dotr/ee/state_grid.h"

int SzDuel_SetCardRot(int a, int b, struct DotrStateVector *cardRotation)
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
    if (cardRotation) {
        o->unknown_10 = *cardRotation;
    }
    return 1;
}

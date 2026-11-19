#include "dotr/ee/side_table.h"

int SzData_GetDeckCost(int a, int k)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(a), k);
    if (a < 0 || a >= 2) {
        return -1;
    }
    return q->deckCost;
}

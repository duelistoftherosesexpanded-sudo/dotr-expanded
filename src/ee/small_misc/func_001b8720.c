#include "dotr/ee/small_misc.h"

#define DOTR_BAG_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_BAG_MIN(a, b) ((a) < (b) ? (a) : (b))

void lSzDuel_ReBuildBag(void)
{
    struct DotrMiscControlLists *p;
    int side;
    short *list;
    int i;
    int hi;

    side = data_00317490_lists.side;
    p = &data_00317490_lists;
    i = 0;
    list = data_00317490_lists.lists[0];
    data_00317490_lists.pairs[0].count = 0;
    for (; i < 854; i++, list++) {
        if (SzData_GetBagCount(side, i, -1) > 0) {
            *list = i;
            p->pairs[0].count++;
        } else {
            *list = 999;
        }
    }
    hi = DOTR_BAG_MAX(p->pairs[0].count - 5, 0);
    p->pairs[0].top = DOTR_BAG_MIN(DOTR_BAG_MAX(p->pairs[0].top, 0), hi);
    lSzDuel_SortBag();
}

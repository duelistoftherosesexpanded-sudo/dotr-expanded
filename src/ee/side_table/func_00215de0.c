#include "dotr/ee/side_table.h"

int SzData_SetRank(int a, int id)
{
    struct DotrSide *p = DOTR_SIDE(a);
    struct DotrSideRecord *r;
    int i;
    unsigned short *t;
    if (a < 0 || a >= 2) {
        return -1;
    }
    if (id == 671) {
        id = 999;
    }
    if (id < 0 || id >= 683) {
        return -1;
    }
    r = &p->cards[id];
    for (i = 12, t = &gSzData_awExpTable[12]; i >= 0; i--, t--) {
        if (r->exp >= *t) {
            break;
        }
    }
    if (r->rank != 0) i = DOTR_SIDE_MAX(i, 1);
    r->rank = i;
    return r->rank;
}

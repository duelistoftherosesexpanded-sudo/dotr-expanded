#include "dotr/ee/side_table.h"

int SzData_SetPassFlag(int a, int id, int flag)
{
    struct DotrSide *p = DOTR_SIDE(a);
    struct DotrSideRecord *r;
    if (a < 0 || a >= 2) {
        return 0;
    }
    if (id == 671) {
        id = 999;
    }
    if (id < 0 || id >= 854) {
        return 0;
    }
    r = &p->cards[id];
    r->unknown_48_bit_15 = flag ? 1 : 0;
    return 1;
}

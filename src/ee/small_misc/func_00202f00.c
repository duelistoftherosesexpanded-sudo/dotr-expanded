#include "dotr/ee/small_misc.h"

int SzText_GetClutGsAddr(int count)
{
    struct DotrMiscAccumulator *p = gSzText_tl;
    int r;
    if (gSzText_tl[0].unknown_04 + count > gSzText_tl[0].unknown_02) {
        return -1;
    }
    r = p->unknown_00 + p->unknown_04;
    p->unknown_04 += count;
    return r;
}

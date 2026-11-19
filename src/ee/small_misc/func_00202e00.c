#include "dotr/ee/small_misc.h"

/* The two-step division reproduces the two arithmetic shifts. */
int SzText_InitClutManage(int base, int size)
{
    gSzText_tl[0].unknown_00 = base;
    gSzText_tl[0].unknown_02 = (size << 13) / 64 / 4;
    gSzText_tl[0].unknown_04 = 0;
    return gSzText_tl[0].unknown_02;
}

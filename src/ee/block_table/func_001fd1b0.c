#include "dotr/ee/block_table.h"

int SzFade_GetRestCount(int i)
{
    struct DotrBlockEntry *e;
    if (i < 0 || i >= 2) {
        return -1;
    }
    e = &gSzFade_fl[i];
    if (e->unknown_07.unknown_bit_6) {
        return -1;
    }
    return e->unknown_06 - e->unknown_05;
}

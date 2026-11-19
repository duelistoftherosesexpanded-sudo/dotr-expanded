#include "dotr/ee/block_table.h"

int SzFade_SetKillFlag(int i)
{
    struct DotrBlockEntry *e;
    if (i < 0 || i >= 2) {
        return 0;
    }
    e = &gSzFade_fl[i];
    if (e->unknown_07.unknown_bit_6) {
        return 0;
    }
    e->unknown_07.unknown_bit_7 = -1;
    return 1;
}

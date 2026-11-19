#include "dotr/ee/block_table.h"

int SzFade_SetPauseFlag(int i, int flag)
{
    struct DotrBlockEntry *e;
    if (i < 0 || i >= 2) {
        return 0;
    }
    e = &gSzFade_fl[i];
    if (e->unknown_07.unknown_bit_6) {
        return 0;
    }
    e->unknown_08.unknown_bit_0 = (signed char)flag;
    return 1;
}

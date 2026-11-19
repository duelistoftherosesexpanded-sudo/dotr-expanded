#include "dotr/ee/block_table.h"

void SzFade_Init(void)
{
    struct DotrBlockEntry *e = gSzFade_fl;
    int n = 2;
    while (n-- != 0) {
        e->unknown_07.unknown_bit_6 = -1;
        e++;
    }
}

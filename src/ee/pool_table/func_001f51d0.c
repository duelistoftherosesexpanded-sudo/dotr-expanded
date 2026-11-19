#include "dotr/ee/pool_table.h"

int SzEffect_IsUse(int index)
{
    struct DotrPoolEntry24 *e = &gSzEffect_el_pool.unknown_100[index];
    int r = 0;
    if (index < 0 || index >= 24) {
        return 0;
    }
    if (e->unknown_00_bits_0_2 == 1) {
        r |= 1;
    }
    if (e->unknown_00_bits_0_2 == 2) {
        r |= 2;
    }
    return r;
}

#include "dotr/ee/pool_table.h"

int SzEffect_IsUse2(int a, int b)
{
    struct DotrPoolEntry24 *e = gSzEffect_el_pool.unknown_100;
    int r = 0;
    int i;
    for (i = 0; i < 24; i++, e++) {
        if (a >= 0 && e->unknown_02_bits_0_2 != a) {
            continue;
        }
        if (b >= 0 && e->unknown_02_bits_3_12 != b) {
            continue;
        }
        if (e->unknown_00_bits_0_2 == 1) {
            r |= 1;
        }
        if (e->unknown_00_bits_0_2 == 2) {
            r |= 2;
        }
    }
    return r;
}

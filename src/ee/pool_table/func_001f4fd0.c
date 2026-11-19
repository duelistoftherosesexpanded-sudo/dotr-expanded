#include "dotr/ee/pool_table.h"

int SzEffect_Sleep2(int a, int b, int flag)
{
    struct DotrPoolEntry24 *e = gSzEffect_el_pool.unknown_100;
    int r = 0;
    int i;
    for (i = 0; i < 24; i++, e++) {
        if (a < 0 || e->unknown_02_bits_0_2 == a) {
            if (b < 0 || e->unknown_02_bits_3_12 == b) {
                r |= SzEffect_Sleep(i, flag);
            }
        }
    }
    return r;
}

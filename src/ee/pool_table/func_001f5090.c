#include "dotr/ee/pool_table.h"

/* The per-entry step is an inline helper that repeats the index bound, which reproduces the
 * original; the original helper form is unknown. */
static inline int dotr_pool_stop(int index)
{
    struct DotrPoolEntry24 *q = &gSzEffect_el_pool.unknown_100[index];

    if (index < 0 || index >= 24) {
        return 0;
    }
    if (q->unknown_00_bits_0_2 == 2) {
        return SzEffect_Destroy(index);
    }
    if (q->unknown_00_bits_0_2 != 1) {
        return 0;
    }
    q->unknown_02_bit_14 = -1;
    return 1;
}

int SzEffect_Erase2(int a, int b)
{
    struct DotrPoolEntry24 *e = gSzEffect_el_pool.unknown_100;
    int r = 0;
    int i;
    for (i = 0; i < 24; i++, e++) {
        if (a < 0 || e->unknown_02_bits_0_2 == a) {
            if (b < 0 || e->unknown_02_bits_3_12 == b) {
                r |= dotr_pool_stop(i);
            }
        }
    }
    return r;
}

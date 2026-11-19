#include "dotr/ee/pool_table.h"

void SzEffect_TrimModule(void)
{
    struct DotrPoolEntry8 *e = gSzEffect_el_pool.unknown_000;
    int i;
    for (i = 0; i < 32L; i++, e++) {
        if (e->unknown_bits_0_2 == 2) {
            if (SzEffect_IsUse2(e->unknown_bits_3_5, e->unknown_bits_6_15) == 0) {
                e->unknown_bits_0_2 = 3;
            }
        }
    }
}

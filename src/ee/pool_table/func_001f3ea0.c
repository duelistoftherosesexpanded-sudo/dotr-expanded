#include "dotr/ee/pool_table.h"

struct DotrPoolEntry8 *SzEffect_GetModuleInfo(int a, int b)
{
    struct DotrPoolEntry8 *e = gSzEffect_el_pool.unknown_000;
    int i;
    for (i = 0; i < 32L; i++, e++) {
        if (a >= 0 && e->unknown_bits_3_5 != a) {
            continue;
        }
        if (b < 0 || e->unknown_bits_6_15 == b) {
            return e;
        }
    }
    return 0;
}

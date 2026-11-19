#include "dotr/ee/pool_table.h"

/* The body of SzEffect_GetModuleInfo (0x1f3ea0), which 0x1f4cb0 contains inline; the original
 * helper form is unknown. */
static inline struct DotrPoolEntry8 *dotr_pool_find_small(int a, int b)
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

int SzEffect_Destroy(int index)
{
    struct DotrPoolEntry24 *q = &gSzEffect_el_pool.unknown_100[index];
    struct DotrPoolEntry8 *e;

    if (index < 0 || index >= 24) {
        return 0;
    }
    if (q->unknown_02_bits_0_2 == 1 || q->unknown_02_bits_0_2 == 2) {
        SzEffect_ClearRefID2(q->unknown_06, q->unknown_02_bits_0_2);
        SzEffect_ClearRefID2(q->unknown_04, q->unknown_02_bits_0_2);
    }
    q->unknown_00_bits_0_2 = 0;
    if (q->unknown_08 != 0) {
        q->unknown_08(q->unknown_0c);
    }
    e = dotr_pool_find_small(q->unknown_02_bits_0_2, q->unknown_02_bits_3_12);
    if (e == 0) {
        return 1;
    }
    if (e->unknown_bits_0_2 == 2) {
        if (SzEffect_IsUse2(e->unknown_bits_3_5, e->unknown_bits_6_15) == 0) {
            e->unknown_bits_0_2 = 3;
        }
    }
    return 1;
}

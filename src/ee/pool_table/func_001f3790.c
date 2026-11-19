#include "dotr/ee/pool_table.h"

/* MATCHING DEVICE: the second loop bound is a long constant, which reproduces the
 * original's sign-extending compare. */
void SzEffect_Clear(void)
{
    struct DotrPool *t = &gSzEffect_el_pool;
    struct DotrPoolEntry24 *e = gSzEffect_el_pool.unknown_100;
    struct DotrPoolEntry8 *q = gSzEffect_el_pool.unknown_000;
    int i;
    for (i = 0; i < 24; i++, e++) {
        e->unknown_00_bits_0_2 = 0;
    }
    for (i = 0; i < 32L; i++, q++) {
        q->unknown_bits_0_2 = 0;
        if (q->unknown_04) {
            SzMem_Free(q->unknown_04);
            q->unknown_04 = 0;
        }
    }
    t->unknown_348 = 0;
    t->unknown_34c = 0;
    t->unknown_340 = -1;
    t->unknown_344 = -1;
}

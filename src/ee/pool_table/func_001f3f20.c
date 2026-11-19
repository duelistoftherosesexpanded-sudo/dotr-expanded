#include "dotr/ee/pool_table.h"

/* The body of SzEffect_GetModuleInfo, which the target repeats in place. */
static inline struct DotrPoolEntry8 *find(int a, int b)
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

static inline void play(struct DotrPoolCue *q, struct DotrPoolEntry24 *t)
{
    int j;
    unsigned short low;
    unsigned short now;
    int back;

    back = func_001a8760_int();
    now = t->unknown_00_bits_3_15;
    low = now - back;
    for (j = 0; j < 64; j++, q++) {
        if (q->tag == 15) {
            break;
        }
        if (q->time >= low && q->time < now) {
            func_00217710_cue(1, q->value);
        }
    }
}

void SzEffect_ReqSE(void)
{
    struct DotrPoolEntry24 *t = gSzEffect_el_pool.unknown_100;
    int i;
    struct DotrPoolEntry8 *e;

    for (i = 0; i < 24; i++, t++) {
        if (t->unknown_02_bits_0_2 == 0 && t->unknown_00_bits_0_2 == 1) {
            e = find(t->unknown_02_bits_0_2, t->unknown_02_bits_3_12);
            if (e != 0 && e->unknown_bits_0_2 == 2 && e->unknown_04 != 0) {
                play(e->unknown_04, t);
            }
        }
    }
}

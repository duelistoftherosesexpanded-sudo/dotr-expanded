#include "dotr/ee/block_table.h"

/* Same body as SzFade_GetRestCount. */
static inline int pending(int i)
{
    struct DotrBlockEntry *e;
    if (i < 0 || i >= 2) {
        return -1;
    }
    e = &gSzFade_fl[i];
    if (e->unknown_07.unknown_bit_6) {
        return -1;
    }
    return e->unknown_06 - e->unknown_05;
}

int SzFade_IsDoing(int flag)
{
    int n = flag ? 1 : 0;
    int i;
    for (i = 0; i < 2; i++) {
        if (!(pending(i) < n)) {
            return 1;
        }
    }
    return 0;
}

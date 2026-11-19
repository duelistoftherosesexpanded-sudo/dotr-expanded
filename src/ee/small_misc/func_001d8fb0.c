#include "dotr/ee/small_misc.h"

/* Returns 0 for a null object. Otherwise stores the four integers as floats
 * at +0x10, +0x14, +0x20 and +0x24, zeroes the words after each pair, stores
 * the two halfwords at +0x9a and +0x98, calls SzDuel_DoCardControl0 and returns 1. */
int lSzDuel_SetCardCtl(struct DotrMiscRect *p, int a, int b, int c, int d, short e, short f)
{
    if (p == 0) {
        return 0;
    }
    p->unknown_10 = a;
    p->unknown_14 = b;
    p->unknown_18 = 0;
    p->unknown_1c = 0;
    p->unknown_20 = c;
    p->unknown_24 = d;
    p->unknown_28 = 0;
    p->unknown_2c = 0;
    p->unknown_98 = f;
    p->unknown_9a = e;
    SzDuel_DoCardControl0(p);
    return 1;
}

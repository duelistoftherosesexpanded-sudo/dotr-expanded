#include "dotr/ee/small_misc.h"

int SzText_SleepRequest(int a, int b, int flag)
{
    struct DotrMiscScanEntry *e = gSzText_aItem;
    int r = 0;
    int i;
    for (i = 0; i < 1024; i++, e++) {
        if (e->unknown_00 == 0) {
            continue;
        }
        if (!e->unknown_08.unknown_bit_0) {
            continue;
        }
        if (a >= 0 && e->unknown_08.unknown_bits_3_15 != a) {
            continue;
        }
        if (b >= 0 && e->unknown_0a != b) {
            continue;
        }
        e->unknown_08.unknown_bit_1 = (signed char)(flag ? 1 : 0);
        r = 1;
    }
    return r;
}

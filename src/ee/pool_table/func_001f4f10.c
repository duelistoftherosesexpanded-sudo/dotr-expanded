#include "dotr/ee/pool_table.h"

int SzEffect_Sleep(int index, int flag)
{
    struct DotrPoolEntry24 *q = &gSzEffect_el_pool.unknown_100[index];
    int state;
    if (index < 0 || index >= 24) {
        return 0;
    }
    state = q->unknown_00_bits_0_2;
    if (state == 0) {
        return 0;
    }
    if (flag) {
        if (state != 1) {
            return 0;
        }
    } else {
        if (state != 2) {
            return 0;
        }
        q->unknown_00_bits_0_2 = 1;
    }
    q->unknown_02_bit_13 = (signed char)flag;
    return 1;
}

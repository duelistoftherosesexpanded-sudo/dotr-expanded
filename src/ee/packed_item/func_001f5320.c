#include "dotr/ee/packed_item.h"

int SzEffect_GetCount(int effectIndex)
{
    struct DotrPackedEntry *e = &gSzEffect_el.unknown_100[effectIndex];
    if (effectIndex < 0 || effectIndex >= 24) {
        return -1;
    }
    return e->unknown_bits_3_15;
}

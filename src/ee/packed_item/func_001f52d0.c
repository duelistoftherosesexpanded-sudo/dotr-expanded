#include "dotr/ee/packed_item.h"

int SzEffect_GetStatus(int index)
{
    struct DotrPackedEntry *e = &gSzEffect_el.unknown_100[index];
    if (index < 0 || index >= 24) {
        return -1;
    }
    return e->unknown_10;
}

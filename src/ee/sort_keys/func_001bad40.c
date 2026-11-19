#include "dotr/ee/sort_keys.h"

int lSzDuel_CardColorCmp(unsigned short *a, unsigned short *b)
{
    int id_a = *a;
    int id_b = *b;
    struct DotrSortEntry *entry_a = (struct DotrSortEntry *)SzDuel_GetCardConstInfo(id_a);
    struct DotrSortEntry *entry_b = (struct DotrSortEntry *)SzDuel_GetCardConstInfo(id_b);
    int key_a, key_b;

    key_a = entry_a ? entry_a->kind >> 5 : DOTR_SORT_MISSING;
    key_b = entry_b ? entry_b->kind >> 5 : DOTR_SORT_MISSING;
    if (key_a != DOTR_SORT_MISSING) {
        if (key_a != 0 || entry_a->effectId == 0xffff)
            key_a += 6;
    }
    if (key_b != DOTR_SORT_MISSING) {
        if (key_b != 0 || entry_b->effectId == 0xffff)
            key_b += 6;
    }
    if (key_b < key_a)
        return 1;
    return key_a < key_b ? -1 : 0;
}

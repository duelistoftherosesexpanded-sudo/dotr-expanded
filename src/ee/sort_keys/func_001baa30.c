#include "dotr/ee/sort_keys.h"

int lSzDuel_DPCmp(unsigned short *a, unsigned short *b)
{
    int id_a = *a;
    int id_b = *b;
    struct DotrSortEntry *entry_a = (struct DotrSortEntry *)SzDuel_GetCardConstInfo(id_a);
    struct DotrSortEntry *entry_b = (struct DotrSortEntry *)SzDuel_GetCardConstInfo(id_b);
    int key_a, key_b;

    key_a = entry_a ? entry_a->dpWithFlags & 0x1fff : -1;
    key_b = entry_b ? entry_b->dpWithFlags & 0x1fff : -1;
    return key_a < key_b ? 1 : key_b < key_a ? -1 : 0;
}

#include "dotr/ee/sort_keys.h"

int lSzDuel_AttrCmp(unsigned short *a, unsigned short *b)
{
    int id_a = *a;
    int id_b = *b;
    struct DotrSortEntry *entry_a = (struct DotrSortEntry *)SzDuel_GetCardConstInfo(id_a);
    struct DotrSortEntry *entry_b = (struct DotrSortEntry *)SzDuel_GetCardConstInfo(id_b);
    int key_a = 0x7fffffff, key_b = 0x7fffffff;

    if (entry_a)
        key_a = SzDuel_Kind2Attr(entry_a->kind >> 5, entry_a->attr);
    if (entry_b)
        key_b = SzDuel_Kind2Attr(entry_b->kind >> 5, entry_b->attr);
    if (key_b < key_a)
        return 1;
    return key_a < key_b ? -1 : 0;
}

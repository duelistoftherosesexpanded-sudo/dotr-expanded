#include "dotr/ee/sort_keys.h"

int lSzDuel_NameCmp(unsigned short *a, unsigned short *b)
{
    struct DotrSortBlock *block = (struct DotrSortBlock *)&data_00317490;
    int id_a = *a;
    int id_b = *b;
    int key_a, key_b;

    key_a = id_a == 999 ? DOTR_SORT_MISSING : block->ranks[id_a];
    key_b = id_b == 999 ? DOTR_SORT_MISSING : block->ranks[id_b];
    return key_b < key_a ? 1 : key_a < key_b ? -1 : 0;
}

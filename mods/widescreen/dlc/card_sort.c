/* New cards when the card lists are sorted by name: lSzDuel_NameCmp
 * (sort_keys/func_001ba930.c), hooked in mod.json. The game ranks names with a table of one entry
 * per card number; a new card takes its donor's place in it for now. */

#include "dotr/ee/sort_keys.h"
#include "dlc_cards.h"

int DlcSort_NameCmp(unsigned short *a, unsigned short *b)
{
    struct DotrSortBlock *block = (struct DotrSortBlock *)&data_00317490;
    int id_a = DlcCard_Donor(*a);
    int id_b = DlcCard_Donor(*b);
    long key_a, key_b;

    key_a = id_a == 999 ? DOTR_SORT_MISSING : block->ranks[id_a];
    key_b = id_b == 999 ? DOTR_SORT_MISSING : block->ranks[id_b];
    return key_b < key_a ? 1 : key_a < key_b ? -1 : 0;
}

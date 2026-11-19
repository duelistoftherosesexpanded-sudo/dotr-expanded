#include "dotr/ee/sort_keys.h"

int lSzDuel_RankCmp(unsigned short *a, unsigned short *b)
{
    int side = DeckBuildingSide;
    int id_a = *a;
    int id_b = *b;
    int key_a, key_b;

    key_a = id_a >= 683 ? DOTR_SORT_MISSING : SzData_GetRank(side, id_a);
    key_b = id_b >= 683 ? DOTR_SORT_MISSING : SzData_GetRank(side, id_b);
    return key_b < key_a ? 1 : key_a < key_b ? -1 : 0;
}

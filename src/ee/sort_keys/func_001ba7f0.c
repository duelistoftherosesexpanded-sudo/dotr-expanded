#include "dotr/ee/sort_keys.h"

int lSzDuel_NewComerCmp(unsigned short *a, unsigned short *b)
{
    int side = DeckBuildingSide;
    int id_a = *a;
    int id_b = *b;
    int key_a, key_b;

    if (id_a == 999)
        key_a = 2;
    else
        key_a = SzData_IsBagNew(side, id_a) ? 0 : 1;
    if (id_b == 999)
        key_b = 2;
    else
        key_b = SzData_IsBagNew(side, id_b) ? 0 : 1;
    if (key_b < key_a)
        return 1;
    return key_a < key_b ? -1 : 0;
}

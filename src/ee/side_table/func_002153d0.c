#include "dotr/ee/side_table.h"

int SzData_PullDeckLeader(int side, int deckNo)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    struct DotrSideEntry *e;
    int id;
    if (side < 0 || side >= 2) {
        return 0;
    }
    id = q->leader;
    if (id == 999) {
        return 0;
    }
    e = SzData_CheckBag(side, id, deckNo, 1);
    if (e == 0) {
        return 0;
    }
    q->leader = 999;
    e->unknown_04_bits_30_31 = 3;
    e->unknown_00_bit_31 = 0;
    return 1;
}

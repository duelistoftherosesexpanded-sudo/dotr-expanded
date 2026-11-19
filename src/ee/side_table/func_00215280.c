#include "dotr/ee/side_table.h"

int SzData_PushDeckLeader(int side, int deckNo, int cardNo)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    struct DotrSideEntry *e;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    if (q->leader != 999) {
        return 0;
    }
    e = SzData_CheckBag(side, cardNo, 3, 0);
    if (e == 0) {
        return 0;
    }
    q->leader = cardNo;
    e->unknown_04_bits_30_31 = deckNo;
    e->unknown_00_bit_31 = 1;
    return 1;
}

#include "dotr/ee/side_table.h"

int SzData_GetCardNo(int side, int deckNo, int cardSlot)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    if (side < 0 || side >= 2) {
        return -1;
    }
    if (cardSlot < 0 || cardSlot >= 40) {
        return -1;
    }
    return q->cards[cardSlot];
}

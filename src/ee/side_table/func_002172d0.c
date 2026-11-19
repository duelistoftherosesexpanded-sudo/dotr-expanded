#include "dotr/ee/side_table.h"

int SzData_SetRankDirect(int side, int cardNo, int rank)
{
    struct DotrSide *p = DOTR_SIDE(side);
    struct DotrSideRecord *r;
    if (side < 0 || side >= 2) {
        return -1;
    }
    if (rank < 0 || rank >= 13) {
        return -1;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 683) {
        return -1;
    }
    r = &p->cards[cardNo];
    r->rank = rank;
    r->exp = gSzData_awExpTable[rank];
    return r->rank;
}

#include "dotr/ee/side_table.h"

struct DotrSideEntry *SzData_GetCardInfo(int side, int cardNo, int n)
{
    struct DotrSide *p = DOTR_SIDE(side);
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (n < 0 || n >= 9) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    return &p->cards[cardNo].copies[n];
}

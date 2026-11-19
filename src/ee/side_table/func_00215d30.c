#include "dotr/ee/side_table.h"

int SzData_GetRank(int side, int cardNo)
{
    struct DotrSide *p = DOTR_SIDE(side);
    if (side < 0 || side >= 2) {
        return -1;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 683) {
        return -1;
    }
    return p->cards[cardNo].rank;
}

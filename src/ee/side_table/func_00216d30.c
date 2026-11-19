#include "dotr/ee/side_table.h"

int SzData_GetPassFlag(int side, int cardNo)
{
    struct DotrSide *p = DOTR_SIDE(side);
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    return p->cards[cardNo].unknown_48_bit_15 ? 1 : 0;
}

#include "dotr/ee/side_table.h"

int SzData_IsBagNew(int side, int cardNo)
{
    struct DotrSide *p;
    int i;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    p = DOTR_SIDE(side);
    for (i = 0; i < 5; i++) {
        if (p->newcomers[i] == cardNo) {
            return 1;
        }
    }
    return 0;
}

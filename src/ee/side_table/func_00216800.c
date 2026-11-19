#include "dotr/ee/side_table.h"

int SzData_PushBagNew(int side, int cardNo)
{
    struct DotrSide *p = dotr_side_select(side);
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
    for (i = 0; i < 5; i++) {
        if (p->newcomers[i] == cardNo || i + 1 >= 5) {
            for (; i > 0; i--) p->newcomers[i] = p->newcomers[i - 1];
            break;
        }
    }
    p->newcomers[0] = cardNo;
    return 1;
}

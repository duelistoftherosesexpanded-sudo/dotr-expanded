#include "dotr/ee/side_table.h"

int SzData_RollSpecial(int side, int cardNo)
{
    int i;
    struct DotrSide *p;
    struct DotrSideAlternative *t;
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
    t = aSpecialCard;
    for (i = 0; i < 16; i++, t++) {
        int r = SzDuel_Xax2New(t->unknown_00_bits_0_13);
        if (r == cardNo) {
            p->ownedAltArts[i] = (p->ownedAltArts[i] + 1) % (aSpecialCard[i].unknown_00_bits_14_15 + 1);
            return 1;
        }
    }
    return 0;
}

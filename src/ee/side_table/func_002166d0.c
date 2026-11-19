#include "dotr/ee/side_table.h"

int SzData_ExchSpecial(int side, int cardNo)
{
    struct DotrSide *p;
    struct DotrSideAlternative *altArtSet;
    int i;
    if (side < 0 || side >= 2) {
        return cardNo;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return cardNo;
    }
    p = DOTR_SIDE(side);
    altArtSet = aSpecialCard;
    for (i = 0; i < 16; i++, altArtSet++) {
        int newId = SzDuel_Xax2New(altArtSet->unknown_00_bits_0_13);
        if (newId == cardNo) {
            int c = p->ownedAltArts[i];
            if (c != 0) cardNo = aSpecialCard[i].altArts[c - 1];
            break;
        }
    }
    return cardNo;
}

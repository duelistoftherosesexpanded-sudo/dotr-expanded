#include "dotr/ee/side_table.h"

int SzData_CheckDup(int side, int key, int uniqueId, int cardNo)
{
    struct DotrSideEntry *e;
    int remainingNumberOfCardsToCheck;
    
    if (side < 0 || side >= 2) {
    
        return -1;
    
    }
    if (key == 0) {
        key = dotr_side_select(side)->unknown_ff98;
    }
    if (key == 0) {
        return -1;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return -1;
    }
    e = DOTR_SIDE_ENTRIES(DOTR_SIDE(side), cardNo);
    remainingNumberOfCardsToCheck = 9;
    while (remainingNumberOfCardsToCheck-- != 0) {
        if (e->unknown_00_bits_0_30 != 0) {
            if (e->unknown_04_bits_0_29 == uniqueId) {
                return 1;
            }
        }
        e++;
    }
    return 0;
}

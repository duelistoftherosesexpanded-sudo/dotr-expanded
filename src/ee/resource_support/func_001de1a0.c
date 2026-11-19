#include "dotr/ee/resource_support.h"

int SzDuel_New2Xax(int cardNo)
{
    unsigned short *entry = AlternateCardIds;
    int i;
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 999;
    }
    for (i = 0; i < 854; i++) {
        if (*entry == cardNo) {
            return i;
        }
        entry++;
    }
    return 999;
}

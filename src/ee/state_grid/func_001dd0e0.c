#include "dotr/ee/state_grid.h"

struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int cardNo)
{
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    return &gSzDuel_CardConstInfo[cardNo];
}

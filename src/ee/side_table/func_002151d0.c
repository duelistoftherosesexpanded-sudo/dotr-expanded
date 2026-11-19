#include "dotr/ee/side_table.h"

int SzData_CheckDeck(int side, int deckNo, int cardNo)
{
    struct DotrSideList *deck = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    int i;
    if (side < 0 || side >= 2) {
        return -1;
    }
    for (i = 0; i < 40; i++) {
        if (deck->cards[i] == cardNo) {
            return i;
        }
    }
    return -1;
}

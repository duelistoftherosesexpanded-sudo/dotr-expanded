#include "dotr/ee/side_table.h"

int SzData_GetDeckCount(int side, int deckNo)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    int n = 0;
    int i;
    if (side < 0 || side >= 2) {
        return -1;
    }
    for (i = 0; i < 40; i++) {
        if (q->cards[i] != 999) {
            n++;
        }
    }
    return n;
}

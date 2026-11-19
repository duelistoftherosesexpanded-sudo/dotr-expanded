#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

int SzData_CalcDeckCost(int side, int deckNo)
{
    struct DotrSideList *deck = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    int i;
    if (side < 0 || side >= 2) {
        return -1;
    }
    deck->deckCost = 0;
    for (i = 0; i < 40; i++) {
        struct DotrStateEntry20 *cardInfo = SzDuel_GetCardConstInfo(deck->cards[i]);
        if (cardInfo != 0) deck->deckCost += ((unsigned char *)cardInfo)[3];
    }
    return deck->deckCost;
}

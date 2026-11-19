#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

static inline int dotr_side_count(int a, int k, int value)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(a), k);
    int n = 0;
    int i;
    if (a < 0 || a >= 2) {
        return -1;
    }
    for (i = 0; i < 40; i++) {
        if (q->cards[i] == value) {
            n++;
        }
    }
    return n;
}

static inline int dotr_side_find(int a, int k, int value)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(a), k);
    int i;
    if (a < 0 || a >= 2) {
        return -1;
    }
    for (i = 0; i < 40; i++) {
        if (q->cards[i] == value) {
            return i;
        }
    }
    return -1;
}

int SzData_PushDeck(int side, int deckNo, int cardNo)
{
    struct DotrSideList *dataDeck = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    struct DotrSideEntry *e;
    struct DotrStateEntry20 *t;
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
    if (dotr_side_count(side, deckNo, cardNo) >= 3) {
        return 0;
    }
    i = dotr_side_find(side, deckNo, 999);
    if (i < 0) {
        return 0;
    }
    if ((e = SzData_CheckBag(side, cardNo, 3, 0)) == 0) {
        return 0;
    }
    if ((t = SzDuel_GetCardConstInfo(cardNo)) == 0) {
        return 0;
    }
    dataDeck->deckCost += ((unsigned char *)t)[3];
    dataDeck->cards[i] = cardNo;
    e->unknown_04_bits_30_31 = deckNo;
    return 1;
}

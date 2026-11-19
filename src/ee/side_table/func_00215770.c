#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

int SzData_PullDeck(int side, int deckNo, int deckSlot)
{
    struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    struct DotrSideEntry *e;
    struct DotrStateEntry20 *cardConstInfo;
    unsigned short *slot;
    int id;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (deckSlot < 0 || deckSlot >= 40) {
        return 0;
    }
    slot = &q->cards[deckSlot];
    id = *slot;
    if ((e = SzData_CheckBag(side, id, deckNo, 0)) == 0) {
        return 0;
    }
    if ((cardConstInfo = SzDuel_GetCardConstInfo(id)) == 0) {
        return 0;
    }
    q->deckCost -= ((unsigned char *)cardConstInfo)[3];
    *slot = 999;
    e->unknown_04_bits_30_31 = 3;
    return 1;
}

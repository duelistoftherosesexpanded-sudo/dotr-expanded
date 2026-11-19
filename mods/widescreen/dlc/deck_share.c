/* The three decks share the cards owned: a card's copies serve whichever deck is being built, so
 * three copies (DLC_CARD_COPIES, the most a deck holds and the most a player owns) fill every deck.
 *
 * The game keeps where each owned copy is (struct DotrSideEntry: deck 0, 1, 2 or the chest, and the
 * mark of the copy leading its deck), and the chest shows the copies in the chest. When the deck
 * builder opens (DlcBuildDeck_InitBuildDeck, card_builddeck.c) and when its deck tab changes
 * (SzData_SetCurrentDeck, hooked here), every card's copies are dealt again: as many as the
 * current deck's list holds of it (its leader too) go to that deck, the rest to the chest. So the
 * chest shows everything the current deck is not using, the other decks' copies included. A deck's
 * list is what a duel plays, so the other decks are unchanged.
 *
 * Copies are found through SzData_GetCardInfo (hooked: the game's records for its own cards,
 * card_save.c's for the new ones). */

#include "dotr/ee/side_table.h"
#include "dlc_cards.h"

extern struct DotrSideEntry *DlcSave_GetCardInfo(int side, int cardNo, int n);

void DlcDeck_Share(int side, int deckNo)
{
    struct DotrSide *p;
    struct DotrSideList *deck;
    int id, n, i, need, end = DlcCard_End(), leader;

    if (side < 0 || side >= 2 || deckNo < 0 || deckNo >= 3) {
        return;
    }
    p = DOTR_SIDE(side);
    deck = DOTR_SIDE_LIST(p, deckNo);
    leader = deck->leader & 0x0fff;
    for (id = 0; id < end; id++) {
        int owned = 0;
        if (!DlcCard_Valid(id)) {
            continue;
        }
        for (n = 0; n < 9; n++) {
            struct DotrSideEntry *e = DlcSave_GetCardInfo(side, id, n);
            if (e != 0 && e->unknown_00_bits_0_30 != 0) {
                owned++;
            }
        }
        if (owned == 0) {
            continue;
        }
        for (i = 0, need = 0; i < 40; i++) {
            need += deck->cards[i] == id;
        }
        need += leader == id;
        /* The leader's copy first (it carries the mark), then the deck's, then the chest. */
        for (n = 0, i = 0; n < 9; n++) {
            struct DotrSideEntry *e = DlcSave_GetCardInfo(side, id, n);
            if (e == 0 || e->unknown_00_bits_0_30 == 0) {
                continue;
            }
            e->unknown_00_bit_31 = 0;
            if (i < need) {
                e->unknown_04_bits_30_31 = deckNo;
                if (i == 0 && leader == id) {
                    e->unknown_00_bit_31 = 1;
                }
                i++;
            } else {
                e->unknown_04_bits_30_31 = 3;
            }
        }
    }
}

/* SzData_SetCurrentDeck (side_table/func_00215920.c), and the deal for the deck chosen. */
int DlcDeck_SetCurrentDeck(int side, int deckNo)
{
    struct DotrSide *p = DOTR_SIDE(side);

    if (side < 0 || side >= 2) {
        return 0;
    }
    if (deckNo < 0 || deckNo >= 3) {
        return 0;
    }
    p->flags.unknown_bits_0_1 = deckNo;
    DlcDeck_Share(side, deckNo);
    return 1;
}

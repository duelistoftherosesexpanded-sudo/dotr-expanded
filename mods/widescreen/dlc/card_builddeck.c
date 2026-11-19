/* New cards in the deck builder's list of owned cards: SzDuel_InitBuildDeck
 * (list_reset/func_001b8520.c), hooked in mod.json. The unchanged original sets the list up for
 * the game's cards in its own 854-entry buffer; the list is then refilled in the DLC's longer one
 * with every owned card, the game's and the new ones (card_bag.c), and sorted. */

#include "dotr/ee/list_reset.h"
#include "dlc_cards.h"

extern void DlcPort_SzDuel_InitBuildDeck(int side);
extern int DlcBag_Fill(int side);
extern void DlcBag_SortBag(void);
extern void DlcDeck_Share(int side, int deckNo);
extern int SzData_GetCurrentDeck(int side);

void DlcBuildDeck_InitBuildDeck(int side)
{
    struct DotrListControl *c = DOTR_LIST_CONTROL;

    /* The decks share the cards owned (deck_share.c): the copies dealt to this deck first. */
    DlcDeck_Share(side, SzData_GetCurrentDeck(side));
    DlcPort_SzDuel_InitBuildDeck(side);
    c->id_count = DlcBag_Fill(side);
    DlcBag_SortBag();
    /* Opened from the Card Shop podium: the deck list becomes the shop's cards (shop.c). */
    {
        extern void DlcShop_Init(int side);
        DlcShop_Init(side);
    }
}

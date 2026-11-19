/* The list of owned cards (the "bag" of the deck builder) with room for every card number.
 *
 * The game's list (the build-deck control's lists[0], set by SzDuel_InitBuildDeck to its own
 * buffer of 854 entries, one per card number: the number when owned, else 999, then sorted with
 * 999 last) cannot hold the game's cards and the new ones together. The list is pointed at
 * dlc_bag_list instead, DLC_BAG_SIZE entries: every owned card, then 999s. It is filled after
 * SzDuel_InitBuildDeck (card_builddeck.c) and by lSzDuel_ReBuildBag (small_misc/func_001b8720.c),
 * and lSzDuel_SortBag (small_misc/func_001ba5f0.c, which sorted 854) sorts its whole length;
 * all three are hooked in mod.json. The rest of the deck builder reads the list through the
 * pointer and the owned count. */

#include "dotr/ee/small_misc.h"
#include "dlc_cards.h"
#include "dlc_port.h"

#define DOTR_BAG_MAX(a, b) ((a) > (b) ? (a) : (b))
#define DOTR_BAG_MIN(a, b) ((a) < (b) ? (a) : (b))
#define DLC_BAG_SIZE (DLC_CARD_ORIGINAL + DLC_CARD_SLOTS)

static short dlc_bag_list[DLC_BAG_SIZE];

/* Fills the list with side's owned cards (the game's, then the new ones); returns how many. */
int DlcBag_Fill(int side)
{
    struct DotrMiscControlLists *p = &data_00317490_lists;
    int end = DlcCard_End();
    int i, n = 0;

    p->lists[0] = dlc_bag_list;
    for (i = 0; i < end && n < DLC_BAG_SIZE; i++) {
        /* A disabled card (a later era than the one chosen, eras.c) stays out of the chest. */
        if (DlcCard_Valid(i) && SzData_GetBagCount(side, i, -1) > 0
            && !(DlcCard_IsNew(i) && DlcCard_Status(i) == DLC_STATUS_DISABLED)) {
            dlc_bag_list[n++] = i;
        }
    }
    for (i = n; i < DLC_BAG_SIZE; i++) {
        dlc_bag_list[i] = 999;
    }
    return n;
}

void DlcBag_SortBag(void)
{
    short *list = data_00317490_lists.lists[0];

    func_00199bc0((int)list, list == dlc_bag_list ? DLC_BAG_SIZE : 854, 2, lSzDuel_SortFunc1);
}

void DlcBag_ReBuildBag(void)
{
    struct DotrMiscControlLists *p = &data_00317490_lists;
    int hi;

    p->pairs[0].count = DlcBag_Fill(data_00317490_lists.side);
    hi = DOTR_BAG_MAX(p->pairs[0].count - DLC_ROWS, 0);
    p->pairs[0].top = DOTR_BAG_MIN(DOTR_BAG_MAX(p->pairs[0].top, 0), hi);
    DlcBag_SortBag();
}

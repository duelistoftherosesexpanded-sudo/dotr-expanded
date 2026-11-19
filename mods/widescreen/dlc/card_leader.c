/* Deck leaders (DLC_ALL_LEADERS, dlc_options.h).
 *
 * The deck builder (SzDuel_ActBuildDeck) lets a card lead only when SzData_GetRank gives it a rank
 * above 0; a monster that has never led has rank 0, and SzData_GetRank answers -1 for anything
 * from 683 up, new cards included. SzData_PushDeckLeader also refuses numbers from 854 up. Both
 * are hooked here (mod.json); the game's own path runs through unchanged copies (DlcPort_...).
 *
 *   SzData_GetRank       a monster of the game's with rank 0 answers 1 (the first rank); a new
 *                        monster card answers 1. The stored rank is not changed, so experience
 *                        still builds the real rank as before.
 *   SzData_PushDeckLeader  a new monster card is put in the deck's leader place as the game does
 *                        for its own (the copy in the chest marked as that deck's leader).
 *
 * New cards have no leader abilities: SzDuel_GetUnitLeaderAbility3 answers none for 854 up. */

#include "dotr/ee/side_table.h"
#include "dlc_cards.h"
#include "dlc_options.h"

extern int DlcPort_SzData_GetRank(int, int);
extern int DlcPort_SzData_PushDeckLeader(int, int, int);
struct DotrSideEntry *DlcSave_CheckBag(int side, int cardNo, int kind, int flag);

static int new_monster(int id)
{
    return DlcCard_IsNew(id) && DlcCard_Donor(id) < 683;
}

int DlcLeader_GetRank(int side, int cardNo)
{
    int rank;

    if (DLC_ALL_LEADERS && new_monster(cardNo)) {
        return side < 0 || side >= 2 ? -1 : 1;
    }
    rank = DlcPort_SzData_GetRank(side, cardNo);
    if (DLC_ALL_LEADERS && rank == 0) {
        return 1;
    }
    return rank;
}

/* No demotion (DLC_NO_DEMOTION, dlc_options.h). A card's experience drops when its unit is
 * destroyed (-4) and a leader's when its side loses (-4 to -40, end of duel); after the duel
 * SzData_SetRank works the rank out again from the experience, so a leader could fall a rank.
 * Now experience only grows, and a rank once reached is kept. */
extern int DlcPort_SzData_CalcExp(int side, int cardNo, int op, int change);
extern int DlcPort_SzData_SetRank(int side, int id);

int DlcLeader_CalcExp(int side, int cardNo, int op, int change)
{
    if (DLC_NO_DEMOTION && ((op == 0 && change < 0) || (op == 1 && change > 0))) {
        return 1;
    }
    return DlcPort_SzData_CalcExp(side, cardNo, op, change);
}

int DlcLeader_SetRank(int side, int id)
{
    int old, rank;

    if (side < 0 || side >= 2 || id < 0 || id >= 683 || id == 671) {
        return DlcPort_SzData_SetRank(side, id);
    }
    old = DOTR_SIDE(side)->cards[id].rank;
    rank = DlcPort_SzData_SetRank(side, id);
    if (DLC_NO_DEMOTION && rank >= 0 && rank < old) {
        DOTR_SIDE(side)->cards[id].rank = old;
        rank = old;
    }
    /* After a duel SzDuel_Act compares SzData_GetRank (before) with this (after) and shows
     * "Demotion" when this is lower. DlcLeader_GetRank answers 1 for a stored rank of 0, so this
     * must too, or every card that has never led looked demoted. */
    if (DLC_ALL_LEADERS && rank == 0) {
        rank = 1;
    }
    return rank;
}

/* SzData_PullDeckLeader takes the leader out of a deck only when it finds the copy marked as the
 * leader; for a new card whose mark was lost the leader stayed, and the deck builder could then
 * set no other leader in that deck. For a new card: the marked copy, else any copy in that deck,
 * and the leader place is emptied either way. */
extern int DlcPort_SzData_PullDeckLeader(int side, int deckNo);

int DlcLeader_PullDeckLeader(int side, int deckNo)
{
    struct DotrSideList *q;
    struct DotrSideEntry *e;
    int id;

    if (side < 0 || side >= 2) {
        return 0;
    }
    q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    id = q->leader;
    if (!new_monster(id)) {
        return DlcPort_SzData_PullDeckLeader(side, deckNo);
    }
    e = DlcSave_CheckBag(side, id, deckNo, 1);
    if (e == 0) {
        e = DlcSave_CheckBag(side, id, deckNo, 0);
    }
    q->leader = 999;
    if (e != 0) {
        e->unknown_04_bits_30_31 = 3;
        e->unknown_00_bit_31 = 0;
    }
    return 1;
}

int DlcLeader_PushDeckLeader(int side, int deckNo, int cardNo)
{
    struct DotrSideList *q;
    struct DotrSideEntry *e;

    if (!DLC_ALL_LEADERS || !new_monster(cardNo)) {
        return DlcPort_SzData_PushDeckLeader(side, deckNo, cardNo);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    q = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    if (q->leader != 999) {
        return 0;
    }
    e = DlcSave_CheckBag(side, cardNo, 3, 0);
    if (e == 0) {
        return 0;
    }
    q->leader = cardNo;
    e->unknown_04_bits_30_31 = deckNo;
    e->unknown_00_bit_31 = 1;
    return 1;
}

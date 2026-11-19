/* New cards in the player's data: owned copies, the "new" marks and flags.
 *
 * The game keeps 854 records of 76 bytes per player (struct DotrSideRecord: nine copies, rank,
 * experience and two flags), which fill its save block. New cards get the same records here, in
 * the DLC's memory. Each function below is hooked over the game's (mod.json) and, for one of the
 * game's own cards, calls an unchanged copy of the original (DlcPort_..., compiled from the
 * decompiled source); only new cards take the new path.
 *
 * Saving: the save block has no free bytes, but the rank (14 bits) and experience (16 bits) of a
 * card record are only used for monsters (0-682). The new cards are kept in those 30 bits of
 * records 683-810 and 837-840, read as one run of bits: 6 bits a card number (871-998, then
 * 1000 on), how many copies are in deck 0, 1, 2 and the chest (one of the 35 ways to have at most
 * DLC_CARD_COPIES, three). Record 853 carries a mark that the
 * block holds this. (Saves from before card numbers past 999 kept card 871 + s in record 683 + s,
 * three bits a copy; their mark differs, and they still load.) dlc_pack runs before the game copies a block for the
 * memory card (wraps of the save writers, mod.json), dlc_unpack after a load (wraps of the
 * loaders). A copy's owner key and id are not kept; unpacking gives the block's key and a fresh id.
 *
 * A block without the mark (a save made before the DLC) keeps what it had before the load. With
 * DLC_GIVE_ALL_CARDS (dlc_options.h) every new card is topped up to that many copies after each
 * load and at the start of a game (dlc_grant). */

#include "dotr/ee/side_table.h"
#include "dlc_cards.h"
#include "dlc_options.h"
#include "dlc_bosses.h"
#include "dlc_mods.h"
#include "dlc_achievements.h"

static struct DotrSideRecord extra[2][DLC_CARD_SLOTS];

int DlcSave_GetBagCount(int side, int cardNo, int kind);

extern int DlcPort_SzData_CheckBag(int, int, int, int);
extern int DlcPort_SzData_GetCardInfo(int, int, int);
extern int DlcPort_SzData_CheckDup(int, int, int, int);
extern int DlcPort_SzData_PushDeck(int, int, int);
extern int DlcPort_SzData_AddBag(int, int, int);
extern int DlcPort_SzData_AddBag2(int, int, int, int);
extern int DlcPort_SzData_DelBag(int, int);
extern int DlcPort_SzData_ExchSpecial(int, int);
extern int DlcPort_SzData_PushBagNew(int, int);
extern int DlcPort_SzData_IsBagNew(int, int);
extern int DlcPort_SzData_GetBagCount(int, int, int);
extern int DlcPort_SzData_GetBagCountAll(int);
extern int DlcPort_SzData_GetLookFlag(int, int);
extern int DlcPort_SzData_SetLookFlag(int, int, int);
extern int DlcPort_SzData_GetPassFlag(int, int);
extern int DlcPort_SzData_SetPassFlag(int, int, int);
extern void DlcPort_SzData_Initialize(void);
extern void DlcPort_SzData_ClearData(int);

/* The record of new card id for the player whose block is p. */
static struct DotrSideRecord *record(struct DotrSide *p, int id)
{
    return &extra[p == &gSzData_All_sides[0] ? 0 : 1][id - DLC_CARD_FIRST];
}

static void clear_entry(struct DotrSideEntry *e)
{
    e->unknown_00_bits_0_30 = 0;
    e->unknown_04_bits_0_29 = 0;
    e->unknown_04_bits_30_31 = 3;
    e->unknown_00_bit_31 = 0;
}

static void clear_block(int block)
{
    int i, j;

    for (i = 0; i < DLC_CARD_SLOTS; i++) {
        struct DotrSideRecord *r = &extra[block][i];
        for (j = 0; j < 9; j++) {
            clear_entry(&r->copies[j]);
        }
        r->rank = 0;
        r->exp = 0;
        r->unknown_48_bit_14 = 0;
        r->unknown_48_bit_15 = 0;
    }
}

/* DLC_GIVE_ALL_CARDS (dlc_options.h): tops every new card up to that many copies for the player
 * whose block is `block`, the new ones in the chest (kind 3), with the block's owner key (1 before
 * a save has one) and ids of their own. */
/* {copies, game cards too}: DLC_GIVE_ALL_CARDS and DLC_GIVE_ALL_GAME, kept in DLC.BIN where
 * tools/release.py zeroes them for the players' build (build.json symbols "dlc_give_all"). */
volatile const unsigned char dlc_give_all[2] = {DLC_GIVE_ALL_CARDS, DLC_GIVE_ALL_GAME};

static void dlc_grant(int block)
{
    int i, j, have;
    int key = gSzData_All_sides[block].unknown_ff98 != 0 ? gSzData_All_sides[block].unknown_ff98 : 1;
    int give = dlc_give_all[0], game = dlc_give_all[1];

    for (i = 0; i < dlc_card_count && give > 0; i++) {
        int n = dlc_cards[i].number;
        struct DotrSideRecord *r;
        if (!DlcCard_IsNew(n) || DlcCard_Status(n) != DLC_STATUS_NORMAL) {
            continue;
        }
        r = &extra[block][n - DLC_CARD_FIRST];
        for (j = 0, have = 0; j < 9; j++) {
            if (r->copies[j].unknown_00_bits_0_30 != 0) {
                have++;
            }
        }
        for (j = 0; j < 9 && have < give && have < DLC_CARD_COPIES; j++) {
            /* (the new card's copies; the game's own cards follow below) */
            if (r->copies[j].unknown_00_bits_0_30 == 0) {
                r->copies[j].unknown_00_bits_0_30 = key;
                r->copies[j].unknown_04_bits_0_29 = 0x10000000 + n * 16 + j;
                r->copies[j].unknown_04_bits_30_31 = 3;
                r->copies[j].unknown_00_bit_31 = 0;
                have++;
            }
        }
    }
    /* DLC_GIVE_ALL_GAME: the game's own cards too, for the player (block 0), the same way. */
    for (i = 0; i < DLC_CARD_ORIGINAL && game && give > 0 && block == 0; i++) {
        struct DotrSideRecord *r = &gSzData_All_sides[block].cards[i];
        if (!DlcCard_Valid(i)) {
            continue;
        }
        for (j = 0, have = 0; j < 9; j++) {
            have += r->copies[j].unknown_00_bits_0_30 != 0;
        }
        for (j = 0; j < 9 && have < give && have < DLC_CARD_COPIES; j++) {
            if (r->copies[j].unknown_00_bits_0_30 == 0) {
                r->copies[j].unknown_00_bits_0_30 = key;
                r->copies[j].unknown_04_bits_0_29 = 0x20000000 + i * 16 + j;
                r->copies[j].unknown_04_bits_30_31 = 3;
                r->copies[j].unknown_00_bit_31 = 0;
                have++;
            }
        }
    }
}

#define PACK_FIRST_RECORD 683
#define PACK_MARK_RECORD 853
/* The new opponents' marks (bosses.c): two bits each, 15 opponents to a word, word w in the rank
 * and experience of record 852 - w (852, 851, 850; the new cards use records 683 to 810). */
#define PACK_BOSS_RECORD 852
#define PACK_BOSS_LOWEST (PACK_BOSS_RECORD - DLC_BOSS_WORDS + 1)
/* The world the player is in (world.c): record 849, rank WORLD_RANK + world, experience
 * WORLD_EXP (a save without them starts in the game's world). */
#define PACK_WORLD_RECORD (PACK_BOSS_LOWEST - 1)
#define PACK_WORLD_RANK 0x0d10
#define PACK_WORLD_EXP 0x0d1c
/* The community patches chosen (modoptions.c): record 848, rank main + extras << 2, experience
 * MODS_EXP, or MODS_EXP + 1 with the Xbox button pictures (buttons.c); a loaded save brings its
 * choice back. */
#define PACK_MODS_RECORD (PACK_WORLD_RECORD - 1)
#define PACK_MODS_EXP 0x0d4d
/* The card sleeve chosen (sleeves.c): record 847, rank the sleeve, experience SLEEVE_EXP. */
#define PACK_SLEEVE_RECORD (PACK_MODS_RECORD - 1)
#define PACK_SLEEVE_EXP 0x51ee
#define PACK_MARK_RANK 0x0d1c
/* The achievements (achievements.c): five 30-bit words in records 846 down to 842 (earned marks,
 * duels won, duels played, cards bought). A save from before them has zeros there (the rank and
 * experience of a magic card's record), which is the start. */
#define PACK_ACH_RECORD (PACK_SLEEVE_RECORD - 1)
#define PACK_ACH_LOWEST (PACK_ACH_RECORD - DLC_ACH_WORDS + 1)
/* The card shop's stock (shop.c), so a reload does not bring new cards: one record a card from 811
 * (after the new cards' 128), rank its entry (card, sold), experience SHOP_EXP; then a record whose
 * rank says whether the shop is due new cards, experience SHOP_STATE_EXP. A save without them
 * gets new cards when the shop next opens. */
#define PACK_CARD_RECORDS 128
#define PACK_SHOP_FIRST (PACK_FIRST_RECORD + PACK_CARD_RECORDS)
#define PACK_SHOP_EXP 0x5470
#define PACK_SHOP_STATE_EXP 0x5471
#define PACK_SHOP_MAX 25
/* The run of bits the new cards are kept in: records 683-810, then 837 up to the achievements
 * (132 words of 30 bits). Since mark V4, 6 bits a card: which of the 35 ways to have at most
 * DLC_CARD_COPIES (3) copies in decks 0-2 and the chest. Neither flag is kept: a card owned is
 * seen again after a load, and a password may give a card again (up to the 3 copies). Room for
 * 660 cards. */
#define PACK_SPARE_FIRST (PACK_SHOP_FIRST + PACK_SHOP_MAX + 1)
#define PACK_WORDS (PACK_CARD_RECORDS + PACK_ACH_LOWEST - PACK_SPARE_FIRST)
#define PACK_CARD_BITS 6
#define PACK_CARD_BITS_V3 7
#define PACK_CARD_BITS_V2 12
/* The marks of blocks kept the older ways (V1: one record a card, card 871 + s in record 683 + s;
 * V2: 12 bits a card, up to nine copies, both flags; V3: 7 bits, three copies and the password
 * flag). */
#define PACK_MARK_EXP_V1 0xd1c1
#define PACK_MARK_EXP_V2 0xd1c2
#define PACK_MARK_EXP_V3 0xd1c3
#define PACK_MARK_EXP_V4 0xd1c4

extern int DlcShop_StockSize(void);

static struct DotrSideRecord *pack_word(struct DotrSide *p, int w)
{
    return &p->cards[w < PACK_CARD_RECORDS ? PACK_FIRST_RECORD + w : PACK_SPARE_FIRST + w - PACK_CARD_RECORDS];
}

static unsigned int get_word(struct DotrSide *p, int w)
{
    struct DotrSideRecord *r = pack_word(p, w);
    return r->rank | (unsigned int)r->exp << 14;
}

static void put_word(struct DotrSide *p, int w, unsigned int v)
{
    struct DotrSideRecord *r = pack_word(p, w);
    r->rank = v & 0x3fff;
    r->exp = (v >> 14) & 0xffff;
}

/* Bits at..at+n-1 of the run (30 a word, low bits first). */
static unsigned int get_bits(struct DotrSide *p, int at, int n)
{
    unsigned int v = 0;
    int k;

    for (k = 0; k < n; k++, at++) {
        v |= ((get_word(p, at / 30) >> (at % 30)) & 1) << k;
    }
    return v;
}

static void put_bits(struct DotrSide *p, int at, int n, unsigned int v)
{
    int k;

    for (k = 0; k < n; k++, at++) {
        unsigned int w = get_word(p, at / 30);
        w = (w & ~(1u << (at % 30))) | ((v >> k) & 1) << (at % 30);
        put_word(p, at / 30, w);
    }
}

/* The place of a new card number in the run of cards `bits` wide (999 is skipped), or -1 past
 * its room. */
static int pack_place(int id, int bits)
{
    int s = id < DLC_CARD_NONE ? id - DLC_CARD_FIRST : id - DLC_CARD_FIRST - 1;

    return id >= DLC_CARD_FIRST && id != DLC_CARD_NONE && (s + 1) * bits <= 30 * PACK_WORDS ? s : -1;
}

/* Copies a deck 0, 1, 2 and the chest hold (at most `most` in all) <-> one number (below 35 for
 * three, 715 for nine). */
static int counts_index(const int *c, int most)
{
    int a, b, d, e, n = 0;

    for (a = 0; a <= most; a++)
        for (b = 0; a + b <= most; b++)
            for (d = 0; a + b + d <= most; d++)
                for (e = 0; a + b + d + e <= most; e++, n++)
                    if (a == c[0] && b == c[1] && d == c[2] && e == c[3])
                        return n;
    return 0;
}

static void index_counts(int index, int *c, int most)
{
    int a, b, d, e, n = 0;

    for (a = 0; a <= most; a++)
        for (b = 0; a + b <= most; b++)
            for (d = 0; a + b + d <= most; d++)
                for (e = 0; a + b + d + e <= most; e++, n++)
                    if (n == index) {
                        c[0] = a; c[1] = b; c[2] = d; c[3] = e;
                        return;
                    }
    c[0] = c[1] = c[2] = c[3] = 0;
}

/* A card held more than DLC_CARD_COPIES times (a save from before the limit) keeps that many:
 * chest copies go first, then deck copies (a deck whose leader the card is keeps one), which
 * also leave the deck's list. */
static void cap_counts(struct DotrSide *p, int id, int *c)
{
    int k, i, best;
    unsigned char *t = DlcCard_Record(id);

    while (c[0] + c[1] + c[2] + c[3] > DLC_CARD_COPIES) {
        if (c[3] > 0) {
            c[3]--;
            continue;
        }
        for (k = 0, best = -1; k < 3; k++) {
            int keep = DOTR_SIDE_LIST(p, k)->leader == id ? 1 : 0;
            if (c[k] > keep && (best < 0 || c[k] > c[best])) {
                best = k;
            }
        }
        if (best < 0) {
            break;
        }
        c[best]--;
        for (i = 0; i < 40; i++) {
            struct DotrSideList *deck = DOTR_SIDE_LIST(p, best);
            if (deck->cards[i] == id) {
                deck->cards[i] = 999;
                if (t != 0) {
                    deck->deckCost -= t[3];
                }
                break;
            }
        }
    }
}
extern unsigned int DlcShop_SaveEntry(int k);
extern int DlcShop_Stale(void);
extern void DlcShop_Load(const unsigned int *entries, int count, int is_stale);
extern void DlcShop_Reset(void);

static void dlc_pack(int block)
{
    struct DotrSide *p = &gSzData_All_sides[block];
    int s, j, w;

    for (w = 0; w < DLC_BOSS_WORDS; w++) {
        unsigned int b = DlcBosses_State(w) & 0x3fffffff;
        p->cards[PACK_BOSS_RECORD - w].rank = b & 0x3fff;
        p->cards[PACK_BOSS_RECORD - w].exp = b >> 14;
    }
    p->cards[PACK_WORLD_RECORD].rank = PACK_WORLD_RANK + (dlc_world != 0);
    p->cards[PACK_WORLD_RECORD].exp = PACK_WORLD_EXP;
    p->cards[PACK_MODS_RECORD].rank = (DlcMods_Main() & 3) | (DlcMods_Extras() & 0xfff) << 2;
    /* Experience: MODS_EXP + Xbox buttons (1) + 2 x (card era + 1) (eras.c; a save from before
     * eras has 0 there and keeps the era chosen). */
    p->cards[PACK_MODS_RECORD].exp = PACK_MODS_EXP + (DlcButtons_Xbox() ? 1 : 0) + 2 * (DlcEra_Chosen() + 1);
    p->cards[PACK_SLEEVE_RECORD].rank = DlcSleeves_Chosen();
    p->cards[PACK_SLEEVE_RECORD].exp = PACK_SLEEVE_EXP;
    for (w = 0; w < DLC_ACH_WORDS; w++) {
        unsigned int v = block == 0 ? DlcAch_Word(w) & 0x3fffffff : 0;
        p->cards[PACK_ACH_RECORD - w].rank = v & 0x3fff;
        p->cards[PACK_ACH_RECORD - w].exp = v >> 14;
    }
    if (block == 0 && PACK_SHOP_FIRST + PACK_SHOP_MAX < PACK_ACH_LOWEST) {
        for (w = 0; w < PACK_SHOP_MAX; w++) {
            p->cards[PACK_SHOP_FIRST + w].rank = DlcShop_SaveEntry(w);
            p->cards[PACK_SHOP_FIRST + w].exp = PACK_SHOP_EXP;
        }
        p->cards[PACK_SHOP_FIRST + PACK_SHOP_MAX].rank = DlcShop_Stale() ? 1 : 0;
        p->cards[PACK_SHOP_FIRST + PACK_SHOP_MAX].exp = PACK_SHOP_STATE_EXP;
    }
    for (w = 0; w < PACK_WORDS; w++) {
        put_word(p, w, 0);
    }
    for (s = DLC_CARD_FIRST; s < DLC_CARD_LIMIT; s++) {
        struct DotrSideRecord *r = &extra[block][s - DLC_CARD_FIRST];
        int c[4] = {0, 0, 0, 0}, at = pack_place(s, PACK_CARD_BITS);
        if (at < 0 || !DlcCard_IsNew(s)) {
            continue;
        }
        for (j = 0; j < 9; j++) {
            if (r->copies[j].unknown_00_bits_0_30 != 0) {
                c[r->copies[j].unknown_04_bits_30_31]++;
            }
        }
        cap_counts(p, s, c);
        put_bits(p, at * PACK_CARD_BITS, PACK_CARD_BITS, counts_index(c, DLC_CARD_COPIES));
    }
    p->cards[PACK_MARK_RECORD].rank = PACK_MARK_RANK;
    p->cards[PACK_MARK_RECORD].exp = PACK_MARK_EXP_V4;
}

/* New card s's copies: c[k] of them at place k (decks 0-2, the chest 3), with the block's key. */
static void unpack_copies(struct DotrSide *p, struct DotrSideRecord *r, int id, const int *c)
{
    int j, k, n;

    for (j = 0; j < 9; j++) {
        clear_entry(&r->copies[j]);
    }
    if (p->unknown_ff98 == 0) {
        return;
    }
    for (k = 0, j = 0; k < 4; k++) {
        for (n = 0; n < c[k] && j < 9; n++, j++) {
            r->copies[j].unknown_00_bits_0_30 = p->unknown_ff98;
            r->copies[j].unknown_04_bits_0_29 = 0x10000000 + id * 16 + j;
            r->copies[j].unknown_04_bits_30_31 = k;
        }
    }
}

static void dlc_unpack(int block)
{
    struct DotrSide *p = &gSzData_All_sides[block];
    int s, j, w;

    int v2 = p->cards[PACK_MARK_RECORD].exp == PACK_MARK_EXP_V2;
    int v3 = p->cards[PACK_MARK_RECORD].exp == PACK_MARK_EXP_V3;
    int v4 = p->cards[PACK_MARK_RECORD].exp == PACK_MARK_EXP_V4;

    if (p->cards[PACK_MARK_RECORD].rank != PACK_MARK_RANK
        || (p->cards[PACK_MARK_RECORD].exp != PACK_MARK_EXP_V1 && !v2 && !v3 && !v4)) {
        return;
    }
    if (p == DOTR_SIDE(0)) {
        for (w = 0; w < DLC_BOSS_WORDS; w++) {
            DlcBosses_SetState(w, p->cards[PACK_BOSS_RECORD - w].rank
                                  | (unsigned int)p->cards[PACK_BOSS_RECORD - w].exp << 14);
        }
        DlcWorld_Set(p->cards[PACK_WORLD_RECORD].exp == PACK_WORLD_EXP
                     && p->cards[PACK_WORLD_RECORD].rank == PACK_WORLD_RANK + 1);
        if (p->cards[PACK_MODS_RECORD].exp >= PACK_MODS_EXP
            && p->cards[PACK_MODS_RECORD].exp < PACK_MODS_EXP + 2 * (DLC_ERA_COUNT + 1)) {
            int v = p->cards[PACK_MODS_RECORD].exp - PACK_MODS_EXP;
            DlcMods_Set(p->cards[PACK_MODS_RECORD].rank & 3, p->cards[PACK_MODS_RECORD].rank >> 2);
            DlcButtons_Set(v & 1);
            if ((v >> 1) > 0) {
                DlcEra_Load((v >> 1) - 1);
            }
        }
        /* A save from before sleeves keeps the sleeve chosen on the title screen. */
        if (p->cards[PACK_SLEEVE_RECORD].exp == PACK_SLEEVE_EXP) {
            DlcSleeves_Set(p->cards[PACK_SLEEVE_RECORD].rank);
        }
        DlcAch_Reset();
        for (w = 0; w < DLC_ACH_WORDS; w++) {
            DlcAch_SetWord(w, p->cards[PACK_ACH_RECORD - w].rank
                              | (unsigned int)p->cards[PACK_ACH_RECORD - w].exp << 14);
        }
        if (p->cards[PACK_SHOP_FIRST + PACK_SHOP_MAX].exp == PACK_SHOP_STATE_EXP) {
            unsigned int entries[PACK_SHOP_MAX];
            for (w = 0; w < PACK_SHOP_MAX; w++) {
                entries[w] = p->cards[PACK_SHOP_FIRST + w].exp == PACK_SHOP_EXP ? p->cards[PACK_SHOP_FIRST + w].rank : 0;
            }
            DlcShop_Load(entries, PACK_SHOP_MAX, p->cards[PACK_SHOP_FIRST + PACK_SHOP_MAX].rank != 0);
        } else {
            DlcShop_Reset();
        }
    }
    for (s = 0; s < DLC_CARD_SLOTS; s++) {
        int id = DLC_CARD_FIRST + s, c[4] = {0, 0, 0, 0};
        struct DotrSideRecord *r = &extra[block][s];
        unsigned int v = 0;
        if (v4) {
            int at = pack_place(id, PACK_CARD_BITS);
            if (at >= 0) {
                v = get_bits(p, at * PACK_CARD_BITS, PACK_CARD_BITS);
                index_counts(v & 0x3f, c, DLC_CARD_COPIES);
            }
            r->unknown_48_bit_14 = c[0] + c[1] + c[2] + c[3] > 0;
            r->unknown_48_bit_15 = 0;
        } else if (v3) {
            int at = pack_place(id, PACK_CARD_BITS_V3);
            if (at >= 0) {
                v = get_bits(p, at * PACK_CARD_BITS_V3, PACK_CARD_BITS_V3);
                index_counts(v & 0x3f, c, DLC_CARD_COPIES);
            }
            r->unknown_48_bit_14 = c[0] + c[1] + c[2] + c[3] > 0;
            r->unknown_48_bit_15 = (v >> 6) & 1;
        } else if (v2) {
            int at = pack_place(id, PACK_CARD_BITS_V2);
            if (at >= 0) {
                v = get_bits(p, at * PACK_CARD_BITS_V2, PACK_CARD_BITS_V2);
                index_counts(v & 0x3ff, c, 9);
            }
            r->unknown_48_bit_14 = (v >> 10) & 1;
            r->unknown_48_bit_15 = (v >> 11) & 1;
        } else {
            /* The older way: card 871 + s in record 683 + s, three bits a copy. */
            if (s < PACK_CARD_RECORDS) {
                v = p->cards[PACK_FIRST_RECORD + s].rank | (unsigned int)p->cards[PACK_FIRST_RECORD + s].exp << 14;
                for (j = 0; j < 9; j++) {
                    unsigned int k = (v >> (3 * j)) & 7;
                    if (k >= 1 && k <= 4) {
                        c[k - 1]++;
                    }
                }
            }
            r->unknown_48_bit_14 = (v >> 27) & 1;
            r->unknown_48_bit_15 = (v >> 28) & 1;
        }
        if (DlcCard_IsNew(id)) {
            cap_counts(p, id, c);
        }
        unpack_copies(p, r, id, c);
        /* The save keeps where each copy is but not which copy leads its deck (the bit
         * SzData_PullDeckLeader looks for): a deck whose leader is this card gets one of the
         * card's copies in that deck marked again. Without it the leader could not be taken out,
         * and the deck would take no other leader. */
        for (w = 0; w < 3; w++) {
            if (DOTR_SIDE_LIST(p, w)->leader != DLC_CARD_FIRST + s) {
                continue;
            }
            for (j = 0; j < 9; j++) {
                if (r->copies[j].unknown_00_bits_0_30 != 0 && r->copies[j].unknown_04_bits_30_31 == w) {
                    r->copies[j].unknown_00_bit_31 = 1;
                    break;
                }
            }
        }
    }
}

/* Wraps (mod.json): the save writers pack both blocks first; the loaders unpack both blocks after
 * a successful load (16 or 0x100). */
typedef int (*SaveStep)(int, int, int, int, int, int, int, int);
extern unsigned int DlcTramp_TakuSave_card_file_kakikomi[];
extern unsigned int DlcTramp_TakuTrade_Save_card_file_kakikomi_Ver2[];
extern unsigned int DlcTramp_TakuLoad_card_file_open[];
extern unsigned int DlcTramp_TakuCustom_card_file_open[];

static int save_step(unsigned int *trampoline, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    dlc_pack(0);
    dlc_pack(1);
    return ((SaveStep)trampoline)(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* A save from before the three-copy limit: the player's copies of each of the game's cards above
 * DLC_CARD_COPIES go, chest copies first, then deck copies (never the one leading a deck). The
 * decks' lists stay as they are: the decks share the copies left (deck_share.c). */
static void trim_game_copies(int block)
{
    struct DotrSide *p = &gSzData_All_sides[block];
    int id, j, have, pass;

    for (id = 0; id < DLC_CARD_ORIGINAL; id++) {
        struct DotrSideRecord *r = &p->cards[id];
        for (j = 0, have = 0; j < 9; j++) {
            have += r->copies[j].unknown_00_bits_0_30 != 0;
        }
        for (pass = 0; pass < 2 && have > DLC_CARD_COPIES; pass++) {
            for (j = 8; j >= 0 && have > DLC_CARD_COPIES; j--) {
                struct DotrSideEntry *e = &r->copies[j];
                if (e->unknown_00_bits_0_30 == 0 || e->unknown_00_bit_31) {
                    continue;
                }
                if (pass == 0 && e->unknown_04_bits_30_31 != 3) {
                    continue;          /* chest copies first */
                }
                clear_entry(e);
                have--;
            }
        }
    }
}

static int load_step(unsigned int *trampoline, int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    int result = ((SaveStep)trampoline)(a0, a1, a2, a3, a4, a5, a6, a7);

    if (result == 16 || result == 0x100) {
        dlc_unpack(0);
        dlc_unpack(1);
        trim_game_copies(0);
        dlc_grant(0);
        dlc_grant(1);
        /* Goals the loaded game already reaches (cards owned, podiums beaten) are earned now. */
        DlcAch_Check();
    }
    return result;
}

int DlcSave_WriteSave(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return save_step(DlcTramp_TakuSave_card_file_kakikomi, a0, a1, a2, a3, a4, a5, a6, a7);
}

int DlcSave_WriteTrade(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return save_step(DlcTramp_TakuTrade_Save_card_file_kakikomi_Ver2, a0, a1, a2, a3, a4, a5, a6, a7);
}

int DlcSave_Load(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return load_step(DlcTramp_TakuLoad_card_file_open, a0, a1, a2, a3, a4, a5, a6, a7);
}

int DlcSave_LoadCustom(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
    return load_step(DlcTramp_TakuCustom_card_file_open, a0, a1, a2, a3, a4, a5, a6, a7);
}

void DlcSave_Initialize(void)
{
    int w;

    for (w = 0; w < DLC_BOSS_WORDS; w++) {
        DlcBosses_SetState(w, 0);
    }
    DlcWorld_Set(0);
    DlcAch_Reset();
    DlcShop_Reset();
    DlcPort_SzData_Initialize();
    clear_block(0);
    clear_block(1);
    dlc_grant(0);
    dlc_grant(1);
}

void DlcSave_ClearData(int side)
{
    struct DotrSide *p = DOTR_SIDE(side);
    int block = p == &gSzData_All_sides[0] ? 0 : 1;

    DlcPort_SzData_ClearData(side);
    clear_block(block);
    dlc_grant(block);
}

struct DotrSideEntry *DlcSave_CheckBag(int side, int cardNo, int kind, int flag)
{
    struct DotrSideEntry *e;
    int n;

    if (!DlcCard_IsNew(cardNo)) {
        return (struct DotrSideEntry *)DlcPort_SzData_CheckBag(side, cardNo, kind, flag);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    e = record(DOTR_SIDE(side), cardNo)->copies;
    for (n = 0; n < 9; n++, e++) {
        if (e->unknown_00_bits_0_30 != 0 && e->unknown_04_bits_30_31 == kind
            && (e->unknown_00_bit_31 != 0) == (flag == 1)) {
            return e;
        }
    }
    return 0;
}

struct DotrSideEntry *DlcSave_GetCardInfo(int side, int cardNo, int n)
{
    if (!DlcCard_IsNew(cardNo)) {
        return (struct DotrSideEntry *)DlcPort_SzData_GetCardInfo(side, cardNo, n);
    }
    if (side < 0 || side >= 2 || n < 0 || n >= 9) {
        return 0;
    }
    return &record(DOTR_SIDE(side), cardNo)->copies[n];
}

int DlcSave_CheckDup(int side, int key, int uniqueId, int cardNo)
{
    struct DotrSideEntry *e;
    int n;

    if (!DlcCard_IsNew(cardNo)) {
        return DlcPort_SzData_CheckDup(side, key, uniqueId, cardNo);
    }
    if (side < 0 || side >= 2) {
        return -1;
    }
    if (key == 0) {
        key = dotr_side_select(side)->unknown_ff98;
    }
    if (key == 0) {
        return -1;
    }
    e = record(DOTR_SIDE(side), cardNo)->copies;
    for (n = 0; n < 9; n++, e++) {
        if (e->unknown_00_bits_0_30 != 0 && e->unknown_04_bits_0_29 == uniqueId) {
            return 1;
        }
    }
    return 0;
}

/* SzData_PushDeck only reaches the save records through SzData_CheckBag (hooked) and the card
 * record through SzDuel_GetCardConstInfo (hooked); only its range test needs the new numbers. */
int DlcSave_PushDeck(int side, int deckNo, int cardNo)
{
    struct DotrSideList *deck;
    struct DotrSideEntry *e;
    unsigned char *t;
    int i, n, free;

    if (!DlcCard_IsNew(cardNo)) {
        return DlcPort_SzData_PushDeck(side, deckNo, cardNo);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    deck = DOTR_SIDE_LIST(DOTR_SIDE(side), deckNo);
    for (i = 0, n = 0, free = -1; i < 40; i++) {
        if (deck->cards[i] == cardNo) {
            n++;
        }
        if (free < 0 && deck->cards[i] == 999) {
            free = i;
        }
    }
    if (n >= 3 || free < 0) {
        return 0;
    }
    if ((e = DlcSave_CheckBag(side, cardNo, 3, 0)) == 0) {
        return 0;
    }
    if ((t = DlcCard_Record(cardNo)) == 0) {
        return 0;
    }
    deck->deckCost += t[3];
    deck->cards[free] = cardNo;
    e->unknown_04_bits_30_31 = deckNo;
    return 1;
}

static int add(int side, int key, int value, int cardNo, int check)
{
    struct DotrSideRecord *r;
    struct DotrSideEntry *e;
    int n;

    if (side < 0 || side >= 2) {
        return 0;
    }
    if (key == 0) {
        key = dotr_side_select(side)->unknown_ff98;
    }
    if (key == 0) {
        return 0;
    }
    if (check && DlcSave_CheckDup(side, key, value, cardNo) != 0) {
        return 0;
    }
    r = record(DOTR_SIDE(side), cardNo);
    /* At most DLC_CARD_COPIES copies of a new card (the save keeps no more). */
    if (DlcSave_GetBagCount(side, cardNo, -1) >= DLC_CARD_COPIES) {
        return 0;
    }
    for (n = 0, e = r->copies; n < 9; n++, e++) {
        if (e->unknown_00_bits_0_30 == 0) {
            if (!check) {
                do {
                    value = (int)((long)func_0019a5c8() & ~0x40000000L);
                } while (DlcSave_CheckDup(side, key, value, cardNo) != 0);
            }
            e->unknown_04_bits_0_29 = value;
            e->unknown_00_bits_0_30 = key;
            r->unknown_48_bit_14 = 1;
            SzData_PushBagNew(side, cardNo);
            return 1;
        }
    }
    return 0;
}

/* A card that cannot be obtained (dlc_cards.h, its status) is not given to the player (side 0)
 * however it comes (shop, reward, password); the computer's side gets a disabled card's donor. */
/* The player (side 0) owns at most DLC_CARD_COPIES of any card, the game's too: the decks share
 * the copies (deck_share.c), so three fill every deck. (The computer's side is left as the game
 * makes it.) */
static int at_most(int side, int cardNo)
{
    return side == 0 && DlcSave_GetBagCount(side, cardNo, -1) >= DLC_CARD_COPIES;
}

int DlcSave_AddBag(int side, int key, int cardNo)
{
    if (!DlcCard_IsNew(cardNo)) {
        return at_most(side, cardNo) ? 0 : DlcPort_SzData_AddBag(side, key, cardNo);
    }
    if (DlcCard_Status(cardNo) != DLC_STATUS_NORMAL) {
        if (side == 0) {
            return 0;
        }
        if (DlcCard_Status(cardNo) == DLC_STATUS_DISABLED) {
            return DlcPort_SzData_AddBag(side, key, DlcCard_Donor(cardNo));
        }
    }
    return add(side, key, 0, cardNo, 0);
}

int DlcSave_AddBag2(int side, int key, int value, int cardNo)
{
    if (!DlcCard_IsNew(cardNo)) {
        return at_most(side, cardNo) ? 0 : DlcPort_SzData_AddBag2(side, key, value, cardNo);
    }
    if (DlcCard_Status(cardNo) != DLC_STATUS_NORMAL) {
        if (side == 0) {
            return 0;
        }
        if (DlcCard_Status(cardNo) == DLC_STATUS_DISABLED) {
            return DlcPort_SzData_AddBag2(side, key, value, DlcCard_Donor(cardNo));
        }
    }
    return add(side, key, value, cardNo, 1);
}

int DlcSave_DelBag(int side, int cardNo)
{
    struct DotrSideEntry *e;

    if (!DlcCard_IsNew(cardNo)) {
        return DlcPort_SzData_DelBag(side, cardNo);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    e = DlcSave_CheckBag(side, cardNo, 3, 0);
    if (e == 0) {
        return 0;
    }
    clear_entry(e);
    return 1;
}

int DlcSave_PushBagNew(int side, int cardNo)
{
    struct DotrSide *p;
    int i;

    if (!DlcCard_IsNew(cardNo)) {
        return DlcPort_SzData_PushBagNew(side, cardNo);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    p = dotr_side_select(side);
    for (i = 0; i < 5; i++) {
        if (p->newcomers[i] == cardNo || i + 1 >= 5) {
            for (; i > 0; i--) {
                p->newcomers[i] = p->newcomers[i - 1];
            }
            break;
        }
    }
    p->newcomers[0] = cardNo;
    return 1;
}

int DlcSave_IsBagNew(int side, int cardNo)
{
    struct DotrSide *p;
    int i;

    if (!DlcCard_IsNew(cardNo)) {
        return DlcPort_SzData_IsBagNew(side, cardNo);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    p = DOTR_SIDE(side);
    for (i = 0; i < 5; i++) {
        if (p->newcomers[i] == cardNo) {
            return 1;
        }
    }
    return 0;
}

int DlcSave_GetBagCount(int side, int cardNo, int kind)
{
    struct DotrSideEntry *e;
    int n, count = 0;

    if (!DlcCard_IsNew(cardNo)) {
        return DlcPort_SzData_GetBagCount(side, cardNo, kind);
    }
    if (side < 0 || side >= 2) {
        return -1;
    }
    e = record(DOTR_SIDE(side), cardNo)->copies;
    for (n = 0; n < 9; n++, e++) {
        if (e->unknown_00_bits_0_30 != 0 && (kind < 0 || e->unknown_04_bits_30_31 == kind)) {
            count++;
        }
    }
    return count;
}

int DlcSave_GetBagCountAll(int side)
{
    int count = DlcPort_SzData_GetBagCountAll(side);
    int i;

    if (count < 0) {
        return count;
    }
    for (i = 0; i < dlc_card_count; i++) {
        int n = dlc_cards[i].number;
        if (DlcCard_IsNew(n)) {
            count += DlcSave_GetBagCount(side, n, 3);
        }
    }
    return count;
}

int DlcSave_GetLookFlag(int side, int id)
{
    if (!DlcCard_IsNew(id)) {
        return DlcPort_SzData_GetLookFlag(side, id);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    return record(DOTR_SIDE(side), id)->unknown_48_bit_14 ? 1 : 0;
}

int DlcSave_SetLookFlag(int side, int id, int flag)
{
    if (!DlcCard_IsNew(id)) {
        return DlcPort_SzData_SetLookFlag(side, id, flag);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    record(DOTR_SIDE(side), id)->unknown_48_bit_14 = flag ? 1 : 0;
    return 1;
}

int DlcSave_GetPassFlag(int side, int id)
{
    if (!DlcCard_IsNew(id)) {
        return DlcPort_SzData_GetPassFlag(side, id);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    return record(DOTR_SIDE(side), id)->unknown_48_bit_15 ? 1 : 0;
}

int DlcSave_SetPassFlag(int side, int id, int flag)
{
    if (!DlcCard_IsNew(id)) {
        return DlcPort_SzData_SetPassFlag(side, id, flag);
    }
    if (side < 0 || side >= 2) {
        return 0;
    }
    record(DOTR_SIDE(side), id)->unknown_48_bit_15 = flag ? 1 : 0;
    return 1;
}

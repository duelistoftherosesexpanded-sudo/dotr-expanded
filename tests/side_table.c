/* Synthetic tests of the side-record helpers. Both 0xffa0-byte objects and
 * the four tables are host fixtures, not game data; the two outside
 * dependencies are stubs with assumed semantics. */
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/state_queries.h"
#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

struct DotrSide gSzData_All_sides[2];
unsigned char gSzData_bRoseFlag[1];
unsigned short gSzData_awExpTable[13] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
struct DotrSideAlternative aSpecialCard[16];
struct DotrSidePlacement aTreasureData[22];

static struct DotrSide expected[2];
static struct DotrStateEntry20 entry_fixture[854];
static const int bad_side[] = {INT_MIN, -1, 2, INT_MAX};
static const int bad_id[] = {INT_MIN, -1, 671, 854, 999, INT_MAX};

static int entry_missing = -1;

/* Assumed: identifiers below 854 have an entry (check_add can withhold one). */
struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int id)
{
    if (id == entry_missing) {
        return 0;
    }
    return id >= 0 && id < 854 ? &entry_fixture[id] : 0;
}

/* Assumed: an identifier mapping; the fixture maps 5 to 7 and keeps the rest. */
int SzDuel_Xax2New(int id)
{
    return id == 5 ? 7 : id;
}

static int gate_calls, gate_flag, random_calls, random_count, random_values[4];

int SzSound_SetOutput(int flag)
{
    gate_calls++;
    gate_flag = flag;
    return 1;
}

int func_0019a5c8(void)
{
    assert(random_calls < random_count);
    return random_values[random_calls++];
}

static struct DotrSide *selected(int side)
{
    return &gSzData_All_sides[gSzData_bRoseFlag[0] == side ? 0 : 1];
}

static void fill(int value)
{
    memset(gSzData_All_sides, value, sizeof(gSzData_All_sides));
    memcpy(expected, gSzData_All_sides, sizeof(expected));
}

static struct DotrSide *want(int side)
{
    return &expected[gSzData_bRoseFlag[0] == side ? 0 : 1];
}

static int same(void)
{
    return memcmp(gSzData_All_sides, expected, sizeof(expected)) == 0;
}

static void check_layout(void)
{
    assert(offsetof(struct DotrSide, newcomers) == 0xfd88);
    assert(offsetof(struct DotrSide, decks) == 0xfd94);
    assert(offsetof(struct DotrSide, ownedAltArts) == 0xfe90);
    assert(offsetof(struct DotrSide, treasureFlags) == 0xfea0);
    assert(offsetof(struct DotrSide, flags) == 0xff9c);
    assert(offsetof(struct DotrSideRecord, exp) == 0x4a);
    assert((char *)DOTR_SIDE_LIST(&gSzData_All_sides[1], 2) == (char *)&gSzData_All_sides[1].decks[2]);
    assert(DOTR_SIDE_ENTRIES(&gSzData_All_sides[1], 853) == gSzData_All_sides[1].cards[853].copies);
}

static void check_bits(int side)
{
    struct DotrSide *p = selected(side);
    size_t b;
    int v, which;
    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        assert(SzData_GetCurrentDeck(bad_side[b]) == -1 && SzData_SetCurrentDeck(bad_side[b], 1) == 0);
        assert(SzData_GetMapEditFlag(bad_side[b]) == 0 && SzData_SetMapEditFlag(bad_side[b], 1) == 0);
    }
    assert(SzData_SetCurrentDeck(side, -1) == 0 && SzData_SetCurrentDeck(side, 3) == 0 && same());
    for (v = 0; v < 3; v++) {
        fill(v & 1 ? 0xff : 0);
        assert(SzData_SetCurrentDeck(side, v) == 1 && SzData_GetCurrentDeck(side) == v);
        want(side)->flags.unknown_bits_0_1 = (unsigned char)v;
        assert(same());
    }
    for (which = -1; which < 4; which++) {
        for (v = 0; v < 2; v++) {
            fill(v ? 0 : 0xff);
            if (which == 0) p->flags.unknown_bit_2 = (unsigned char)v;
            if (which == 1) p->flags.unknown_bit_3 = (unsigned char)v;
            if (which == 2) p->flags.unknown_bit_4 = (unsigned char)v;
            assert(SzData_GetOption(side, which) == (which < 0 || which > 2 ? -1 : v));
        }
    }
    for (v = 0; v < 2; v++) {
        fill(v ? 0 : 0xff);
        assert(SzData_SetMapEditFlag(side, v ? 0x100 : 0) == 1 && SzData_GetMapEditFlag(side) == v);
        want(side)->flags.unknown_bit_5 = (unsigned char)v;
        assert(same());
    }
}

static void check_lists(int side)
{
    struct DotrSide *p = selected(side);
    size_t b;
    int list, i;
    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        int s = bad_side[b];
        assert(SzData_GetLeader(s, 0) == -1 && SzData_GetDeckCost(s, 0) == -1 && SzData_GetCardNo(s, 0, 0) == -1);
        assert(SzData_CheckDeck(s, 0, 0) == -1 && SzData_GetDeckCount(s, 0) == -1 && SzData_CalcDeckCost(s, 0) == -1);
    }
    assert(same());
    memset(entry_fixture, 0, sizeof(entry_fixture));
    for (i = 0; i < 854; i++) {
        ((unsigned char *)&entry_fixture[i])[3] = (unsigned char)(i % 7);
    }
    for (list = 0; list < 3; list++) {
        struct DotrSideList *q = &p->decks[list];
        int total = 0, used = 0;
        fill(0x33);
        for (i = 0; i < 40; i++) {
            q->cards[i] = (unsigned short)((i % 3) ? 100 + i * 11 + list : 999);
            if (i % 3) {
                total += (100 + i * 11 + list) % 7;
                used++;
            }
        }
        q->leader = (unsigned short)(0x1230 + list);
        q->deckCost = (unsigned short)(0x4560 + list);
        memcpy(expected, gSzData_All_sides, sizeof(expected));
        assert(SzData_GetLeader(side, list) == 0x1230 + list && SzData_GetDeckCost(side, list) == 0x4560 + list);
        assert(SzData_GetCardNo(side, list, -1) == -1 && SzData_GetCardNo(side, list, 40) == -1);
        for (i = 0; i < 40; i++) {
            assert(SzData_GetCardNo(side, list, i) == q->cards[i]);
        }
        assert(SzData_CheckDeck(side, list, 999) == 0 && SzData_CheckDeck(side, list, 100 + 11 + list) == 1);
        assert(SzData_CheckDeck(side, list, 100 + 38 * 11 + list) == 38 && SzData_CheckDeck(side, list, 5) == -1);
        assert(SzData_GetDeckCount(side, list) == used && same());
        /* Vacant identifiers have no entry in the stub and add nothing. */
        assert(SzData_CalcDeckCost(side, list) == total);
        want(side)->decks[list].deckCost = (unsigned short)total;
        assert(same());
    }
}

static void set_entry(struct DotrSideEntry *e, unsigned word, unsigned kind, unsigned flag)
{
    e->unknown_00_bits_0_30 = word;
    e->unknown_04_bits_30_31 = kind;
    e->unknown_00_bit_31 = flag;
}

static void check_entries(int side)
{
    struct DotrSide *p = selected(side);
    struct DotrSideEntry local;
    size_t b;
    int i;
    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        int s = bad_side[b];
        assert(SzData_GetCardInfo(s, 4, 0) == 0 && SzData_CheckBag(s, 4, 1, 0) == 0 && SzData_DelBag(s, 4) == 0);
        assert(SzData_GetBagCountAll(s) == -1 && SzData_GetBagCount(s, 4, -1) == -1);
    }
    for (b = 0; b < sizeof(bad_id) / sizeof(bad_id[0]); b++) {
        int id = bad_id[b];
        assert(SzData_GetCardInfo(side, id, 0) == 0 && SzData_CheckBag(side, id, 1, 0) == 0);
        assert(SzData_DelBag(side, id) == 0 && SzData_GetBagCount(side, id, -1) == -1);
    }
    assert(SzData_GetCardInfo(side, 4, -1) == 0 && SzData_GetCardInfo(side, 4, 9) == 0);
    for (i = 0; i < 9; i++) {
        assert(SzData_GetCardInfo(side, 853, i) == &p->cards[853].copies[i]);
    }
    assert(same());

    /* 0x214fd0 */
    assert(SzData_ClearCardInfo(0) == 0);
    memset(&local, 0xff, sizeof(local));
    assert(SzData_ClearCardInfo(&local) == 1 && local.unknown_00_bits_0_30 == 0 && local.unknown_00_bit_31 == 0);
    assert(local.unknown_04_bits_0_29 == 0 && local.unknown_04_bits_30_31 == 3);
    memset(&local, 0, sizeof(local));
    assert(SzData_ClearCardInfo(&local) == 1 && local.unknown_04_bits_30_31 == 3 && local.unknown_00_bits_0_30 == 0);

    /* Record 40: slots 2 (kind 1, flag 0), 5 (kind 3, flag 0), 7 (kind 3, flag 1), 8 (kind 1, flag 1). */
    set_entry(&p->cards[40].copies[1], 0, 1, 0); /* empty word: ignored */
    set_entry(&p->cards[40].copies[2], 9, 1, 0);
    set_entry(&p->cards[40].copies[5], 9, 3, 0);
    set_entry(&p->cards[40].copies[7], 0x7fffffffu, 3, 1);
    set_entry(&p->cards[40].copies[8], 1, 1, 1);
    set_entry(&p->cards[853].copies[0], 2, 3, 1);
    memcpy(expected, gSzData_All_sides, sizeof(expected));
    assert(SzData_CheckBag(side, 40, 1, 0) == &p->cards[40].copies[2]);
    assert(SzData_CheckBag(side, 40, 1, 1) == &p->cards[40].copies[8]);
    assert(SzData_CheckBag(side, 40, 3, 0) == &p->cards[40].copies[5]);
    assert(SzData_CheckBag(side, 40, 3, 1) == &p->cards[40].copies[7]);
    assert(SzData_CheckBag(side, 40, 3, 2) == &p->cards[40].copies[5]);
    assert(SzData_CheckBag(side, 40, 0, 0) == 0 && SzData_CheckBag(side, 41, 1, 0) == 0);
    assert(SzData_GetBagCountAll(side) == 3);
    assert(SzData_GetBagCount(side, 40, -1) == 4 && SzData_GetBagCount(side, 40, 1) == 2 && SzData_GetBagCount(side, 40, 3) == 2);
    assert(SzData_GetBagCount(side, 40, 2) == 0 && SzData_GetBagCount(side, 39, -1) == 0 && same());
    /* 0x216490 clears the first kind-3, flag-0 entry of the record. */
    assert(SzData_DelBag(side, 41) == 0 && same());
    assert(SzData_DelBag(side, 40) == 1);
    set_entry(&want(side)->cards[40].copies[5], 0, 3, 0);
    assert(same() && SzData_DelBag(side, 40) == 0 && SzData_GetBagCountAll(side) == 2);
}

static void check_records(int side)
{
    static const int operands[] = {-70000, -1, 0, 1, 3, 70000};
    struct DotrSide *p = selected(side);
    size_t b, o;
    int operation, level, flag;
    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        int s = bad_side[b];
        assert(SzData_CalcExp(s, 4, 4, 1) == 0 && SzData_GetRank(s, 4) == -1 && SzData_SetRank(s, 4) == -1);
        assert(SzData_SetRankDirect(s, 4, 1) == -1 && SzData_GetLookFlag(s, 4) == 0 && SzData_SetLookFlag(s, 4, 1) == 0);
        assert(SzData_GetPassFlag(s, 4) == 0 && SzData_SetPassFlag(s, 4, 1) == 0);
    }
    for (b = 0; b < sizeof(bad_id) / sizeof(bad_id[0]); b++) {
        int id = bad_id[b];
        assert(SzData_CalcExp(side, id, 4, 1) == 0 && SzData_GetRank(side, id) == -1 && SzData_SetRank(side, id) == -1);
        assert(SzData_SetRankDirect(side, id, 1) == -1 && SzData_GetPassFlag(side, id) == 0 && SzData_SetPassFlag(side, id, 1) == 0);
        /* These two do not remap 671. */
        assert(SzData_GetLookFlag(side, id) == 0 && (SzData_SetLookFlag(side, id, 0) == (id == 671)));
    }
    /* The level routines stop at 683. */
    assert(SzData_GetRank(side, 683) == -1 && SzData_SetRank(side, 683) == -1 && SzData_SetRankDirect(side, 683, 1) == -1);
    assert(SzData_SetRankDirect(side, 4, -1) == -1 && SzData_SetRankDirect(side, 4, 13) == -1 && same());

    for (operation = -1; operation <= 5; operation++) {
        for (o = 0; o < sizeof(operands) / sizeof(operands[0]); o++) {
            long r;
            int value = operands[o];
            if (operation == 3 && value == 0) continue;
            fill(0x44);
            p->cards[853].exp = 600;
            want(side)->cards[853].exp = 600;
            r = operation == 0 ? 600L + value : operation == 1 ? 600L - value : operation == 2 ? 600L * value
              : operation == 3 ? 600L / value : value;
            if (operation < 0 || operation > 4) {
                assert(SzData_CalcExp(side, 853, operation, value) == 0 && same());
                continue;
            }
            assert(SzData_CalcExp(side, 853, operation, value) == 1);
            want(side)->cards[853].exp = (unsigned short)(r < 0 ? 0 : r > 0xffff ? 0xffff : r);
            assert(same());
        }
    }

    for (level = 0; level < 13; level++) {
        fill(level & 1 ? 0xff : 0);
        assert(SzData_SetRankDirect(side, 682, level) == level && SzData_GetRank(side, 682) == level);
        want(side)->cards[682].rank = (unsigned short)level;
        want(side)->cards[682].exp = gSzData_awExpTable[level];
        assert(same());
        /* Recomputing from the stored halfword gives the same level, except
         * that a non-zero previous level never falls below 1. */
        assert(SzData_SetRank(side, 682) == level && same());
        p->cards[682].exp = (unsigned short)(gSzData_awExpTable[level] + 9);
        want(side)->cards[682].exp = p->cards[682].exp;
        assert(SzData_SetRank(side, 682) == level && same());
    }
    fill(0);
    p->cards[3].rank = 6;
    p->cards[3].exp = 5;
    assert(SzData_SetRank(side, 3) == 1 && p->cards[3].rank == 1);
    p->cards[3].rank = 0;
    assert(SzData_SetRank(side, 3) == 0);
    p->cards[3].exp = 0xffff;
    assert(SzData_SetRank(side, 3) == 12);

    for (flag = 0; flag < 2; flag++) {
        fill(flag ? 0 : 0xff);
        assert(SzData_SetLookFlag(side, 853, flag ? 0x200 : 0) == 1 && SzData_GetLookFlag(side, 853) == flag);
        want(side)->cards[853].unknown_48_bit_14 = (unsigned short)flag;
        assert(same());
        fill(flag ? 0 : 0xff);
        assert(SzData_SetPassFlag(side, 853, flag ? 0x200 : 0) == 1 && SzData_GetPassFlag(side, 853) == flag);
        want(side)->cards[853].unknown_48_bit_15 = (unsigned short)flag;
        assert(same());
    }
}

static void check_recent_and_flags(int side)
{
    static const unsigned short start[5] = {10, 20, 30, 40, 50};
    struct DotrSide *p = selected(side);
    size_t b;
    int bit, flag, i;
    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        int s = bad_side[b];
        assert(SzData_PushBagNew(s, 4) == 0 && SzData_IsBagNew(s, 4) == 0);
        assert(SzData_GetTreasureFlag(s, 1) == 0 && SzData_SetTreasureFlag(s, 1, 1) == 0);
        assert(SzData_RollSpecial(s, 7) == 0 && SzData_ExchSpecial(s, 7) == 7);
    }
    for (b = 0; b < sizeof(bad_id) / sizeof(bad_id[0]); b++) {
        assert(SzData_PushBagNew(side, bad_id[b]) == 0 && SzData_IsBagNew(side, bad_id[b]) == 0);
        /* A rejected identifier is returned after the 671 remap. */
        assert(SzData_RollSpecial(side, bad_id[b]) == 0);
        assert(SzData_ExchSpecial(side, bad_id[b]) == (bad_id[b] == 671 ? 999 : bad_id[b]));
    }
    assert(same());

    /* 0x216800 moves an identifier to the front; a new one drops the last. */
    memcpy(p->newcomers, start, sizeof(start));
    memcpy(want(side)->newcomers, start, sizeof(start));
    assert(SzData_IsBagNew(side, 30) == 1 && SzData_IsBagNew(side, 31) == 0);
    assert(SzData_PushBagNew(side, 30) == 1);
    {
        static const unsigned short moved[5] = {30, 10, 20, 40, 50};
        memcpy(want(side)->newcomers, moved, sizeof(moved));
    }
    assert(same() && SzData_PushBagNew(side, 30) == 1 && same());
    assert(SzData_PushBagNew(side, 853) == 1);
    {
        static const unsigned short pushed[5] = {853, 30, 10, 20, 40};
        memcpy(want(side)->newcomers, pushed, sizeof(pushed));
    }
    assert(same() && SzData_IsBagNew(side, 50) == 0 && SzData_IsBagNew(side, 853) == 1);

    /* 22 flag bits in three bytes. */
    assert(SzData_GetTreasureFlag(side, -1) == 0 && SzData_GetTreasureFlag(side, 22) == 0);
    assert(SzData_SetTreasureFlag(side, -1, 1) == 0 && SzData_SetTreasureFlag(side, 22, 1) == 0 && same());
    for (bit = 0; bit < 22; bit++) {
        for (flag = 0; flag < 2; flag++) {
            fill(flag ? 0 : 0xff);
            assert(SzData_GetTreasureFlag(side, bit) == !flag);
            assert(SzData_SetTreasureFlag(side, bit, flag ? 0x100 : 0) == 1 && SzData_GetTreasureFlag(side, bit) == flag);
            if (flag) {
                want(side)->treasureFlags[bit >> 3] |= (unsigned char)(1 << (bit % 8));
            } else {
                want(side)->treasureFlags[bit >> 3] &= (unsigned char)~(1 << (bit % 8));
            }
            assert(same());
        }
    }

    /* Row 3 maps (through the stub) to identifier 7 with two alternatives;
     * row 9 is identifier 100 with none. */
    memset(aSpecialCard, 0, sizeof(aSpecialCard));
    for (i = 0; i < 16; i++) {
        aSpecialCard[i].unknown_00_bits_0_13 = (unsigned short)(300 + i);
    }
    aSpecialCard[3].unknown_00_bits_0_13 = 5;
    aSpecialCard[3].unknown_00_bits_14_15 = 2;
    aSpecialCard[3].altArts[0] = 801;
    aSpecialCard[3].altArts[1] = 802;
    aSpecialCard[9].unknown_00_bits_0_13 = 100;
    fill(0);
    assert(SzData_ExchSpecial(side, 7) == 7 && SzData_ExchSpecial(side, 8) == 8);
    assert(SzData_RollSpecial(side, 8) == 0 && same());
    assert(SzData_RollSpecial(side, 7) == 1 && SzData_ExchSpecial(side, 7) == 801);
    assert(SzData_RollSpecial(side, 7) == 1 && SzData_ExchSpecial(side, 7) == 802);
    want(side)->ownedAltArts[3] = 2;
    assert(same());
    assert(SzData_RollSpecial(side, 7) == 1 && SzData_ExchSpecial(side, 7) == 7);
    want(side)->ownedAltArts[3] = 0;
    assert(same());
    assert(SzData_RollSpecial(side, 100) == 1 && SzData_ExchSpecial(side, 100) == 100 && same());
}

static void check_moves(int side)
{
    struct DotrSide *p = selected(side);
    struct DotrSideRecord *r = &p->cards[40];
    size_t b;
    int which, flag, i;

    /* 0x214bb0 has no range test on its first argument. */
    for (which = -1; which < 4; which++) {
        for (flag = 0; flag < 2; flag++) {
            fill(flag ? 0 : 0xff);
            gate_calls = 0;
            assert(SzData_SetOption(side, which, flag ? 0x100 : 0) == (which >= 0 && which <= 2));
            if (which == 0) want(side)->flags.unknown_bit_2 = (unsigned char)flag;
            if (which == 1) want(side)->flags.unknown_bit_3 = (unsigned char)flag;
            if (which == 2) want(side)->flags.unknown_bit_4 = (unsigned char)flag;
            assert(same() && gate_calls == (which == 2) && (which != 2 || gate_flag == flag));
        }
    }

    /* 0x215060, 0x216280 and 0x216040 */
    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        int s = bad_side[b];
        assert(SzData_CheckDup(s, 1, 2, 40) == -1 && SzData_AddBag2(s, 1, 2, 40) == 0 && SzData_AddBag(s, 1, 40) == 0);
        assert(SzData_PushDeckLeader(s, 0, 40) == 0 && SzData_PullDeckLeader(s, 0) == 0 && SzData_PullDeck(s, 0, 0) == 0);
    }
    for (b = 0; b < sizeof(bad_id) / sizeof(bad_id[0]); b++) {
        int id = bad_id[b];
        assert(SzData_CheckDup(side, 1, 2, id) == -1 && SzData_AddBag2(side, 1, 2, id) == 0);
        assert(SzData_AddBag(side, 1, id) == 0 && SzData_PushDeckLeader(side, 0, id) == 0);
    }
    /* A zero key falls back to the word at +0xff98; both zero is rejected. */
    assert(SzData_CheckDup(side, 0, 2, 40) == -1 && SzData_AddBag2(side, 0, 2, 40) == 0 && SzData_AddBag(side, 0, 40) == 0);
    assert(same());
    p->unknown_ff98 = 0x1234567;
    want(side)->unknown_ff98 = 0x1234567;
    assert(SzData_CheckDup(side, 0, 2, 40) == 0);
    for (i = 0; i < 5; i++) {
        p->newcomers[i] = (unsigned short)(10 * (i + 1));
        want(side)->newcomers[i] = p->newcomers[i];
    }
    assert(SzData_AddBag2(side, 0, 0x2abcdef1, 40) == 1);
    {
        static const unsigned short pushed[5] = {40, 10, 20, 30, 50};
        struct DotrSideRecord *w = &want(side)->cards[40];
        w->copies[0].unknown_00_bits_0_30 = 0x1234567;
        w->copies[0].unknown_04_bits_0_29 = 0x2abcdef1;
        w->unknown_48_bit_14 = 1;
        memcpy(want(side)->newcomers, pushed, sizeof(pushed));
    }
    assert(same() && SzData_CheckDup(side, 9, 0x2abcdef1, 40) == 1 && SzData_CheckDup(side, 9, 0x2abcdef0, 40) == 0);
    /* A duplicate value is refused. */
    assert(SzData_AddBag2(side, 9, 0x2abcdef1, 40) == 0 && same());
    /* 0x216040 redraws while the masked value is already present. */
    random_values[0] = 0x2abcdef1;
    random_values[1] = 0x6abcdef1;
    random_values[2] = -5;
    random_calls = 0;
    random_count = 3;
    assert(SzData_AddBag(side, 77, 40) == 1 && random_calls == 3);
    want(side)->cards[40].copies[1].unknown_00_bits_0_30 = 77;
    want(side)->cards[40].copies[1].unknown_04_bits_0_29 = 0x3ffffffbu;
    assert(same());
    /* A full record takes no more entries. */
    for (i = 2; i < 9; i++) {
        r->copies[i].unknown_00_bits_0_30 = 1;
        want(side)->cards[40].copies[i].unknown_00_bits_0_30 = 1;
    }
    random_calls = 0;
    assert(SzData_AddBag2(side, 9, 5, 40) == 0 && SzData_AddBag(side, 9, 40) == 0 && random_calls == 0 && same());

    /* 0x215280 and 0x2153d0 move one identifier in and out of a list's
     * halfword at +0x50. */
    fill(0);
    memset(entry_fixture, 0, sizeof(entry_fixture));
    ((unsigned char *)&entry_fixture[40])[3] = 6;
    for (i = 0; i < 3; i++) {
        int n;
        p->decks[i].leader = 999;
        p->decks[i].deckCost = 100;
        for (n = 0; n < 40; n++) {
            p->decks[i].cards[n] = 999;
        }
    }
    set_entry(&r->copies[4], 8, 3, 0);
    memcpy(expected, gSzData_All_sides, sizeof(expected));
    assert(SzData_PullDeckLeader(side, 2) == 0 && SzData_PushDeckLeader(side, 2, 41) == 0 && same());
    assert(SzData_PushDeckLeader(side, 2, 40) == 1);
    want(side)->decks[2].leader = 40;
    set_entry(&want(side)->cards[40].copies[4], 8, 2, 1);
    assert(same() && SzData_PushDeckLeader(side, 2, 40) == 0 && SzData_PullDeckLeader(side, 1) == 0 && same());
    assert(SzData_PullDeckLeader(side, 2) == 1);
    want(side)->decks[2].leader = 999;
    set_entry(&want(side)->cards[40].copies[4], 8, 3, 0);
    assert(same());

    /* 0x215770 vacates a list position whose record has a matching entry. */
    assert(SzData_PullDeck(side, 1, -1) == 0 && SzData_PullDeck(side, 1, 40) == 0 && SzData_PullDeck(side, 1, 7) == 0);
    p->decks[1].cards[7] = 40;
    want(side)->decks[1].cards[7] = 40;
    assert(SzData_PullDeck(side, 1, 7) == 0 && same());
    set_entry(&r->copies[4], 8, 1, 0);
    assert(SzData_PullDeck(side, 1, 7) == 1);
    want(side)->decks[1].cards[7] = 999;
    want(side)->decks[1].deckCost = 94;
    assert(same());
}

/* SzData_PushDeck (branch / loop coverage: both side bounds, 671, both id bounds,
 * two and three copies already listed, no vacancy, first and last vacant
 * position, no entry of kind 3, no 0x1dd0e0 entry). 0x214dd0 is the real
 * function. */
static void check_add(int side)
{
    struct DotrSide *p = selected(side);
    struct DotrSideRecord *r = &p->cards[40];
    size_t b;
    int i;

    fill(0);
    for (b = 0; b < sizeof(bad_side) / sizeof(bad_side[0]); b++) {
        assert(SzData_PushDeck(bad_side[b], 1, 40) == 0);
    }
    for (i = 0; i < 40; i++) {
        p->decks[1].cards[i] = 999;
        want(side)->decks[1].cards[i] = 999;
    }
    p->decks[1].deckCost = want(side)->decks[1].deckCost = 100;
    p->decks[1].cards[0] = want(side)->decks[1].cards[0] = 40;
    p->decks[1].cards[1] = want(side)->decks[1].cards[1] = 40;
    ((unsigned char *)&entry_fixture[40])[3] = 6;
    set_entry(&r->copies[4], 8, 3, 0);
    set_entry(&want(side)->cards[40].copies[4], 8, 3, 0);
    /* id bounds, and 671 (read as 999) */
    assert(SzData_PushDeck(side, 1, -1) == 0 && SzData_PushDeck(side, 1, 854) == 0 && SzData_PushDeck(side, 1, 671) == 0);
    /* no record entry of kind 3 for the id */
    assert(SzData_PushDeck(side, 1, 41) == 0 && same());
    /* no 0x1dd0e0 entry */
    entry_missing = 40;
    assert(SzData_PushDeck(side, 1, 40) == 0 && same());
    entry_missing = -1;
    /* two copies listed: the first vacant position takes the id */
    assert(SzData_PushDeck(side, 1, 40) == 1);
    want(side)->decks[1].cards[2] = 40;
    want(side)->decks[1].deckCost = 106;
    set_entry(&want(side)->cards[40].copies[4], 8, 1, 0);
    assert(same());
    /* three copies listed: refused even with an entry of kind 3 */
    set_entry(&r->copies[5], 8, 3, 0);
    set_entry(&want(side)->cards[40].copies[5], 8, 3, 0);
    assert(SzData_PushDeck(side, 1, 40) == 0 && same());
    /* no vacant position */
    for (i = 0; i < 40; i++) {
        p->decks[1].cards[i] = want(side)->decks[1].cards[i] = 5;
    }
    assert(SzData_PushDeck(side, 1, 40) == 0 && same());
    /* only the last position vacant; the list index goes into the entry */
    p->decks[2].cards[39] = want(side)->decks[2].cards[39] = 999;
    for (i = 0; i < 39; i++) {
        p->decks[2].cards[i] = want(side)->decks[2].cards[i] = 5;
    }
    assert(SzData_PushDeck(side, 2, 40) == 1);
    want(side)->decks[2].cards[39] = 40;
    want(side)->decks[2].deckCost = 6;
    set_entry(&want(side)->cards[40].copies[5], 8, 2, 0);
    assert(same());
}

/* SzData_ClearData (loop coverage): every byte of the selected object is compared
 * with an expectation built here; the other object must stay untouched.
 * 0x214fd0 is the real function. */
static void check_reset(int side, int value)
{
    struct DotrSide *w;
    int i, j, k;

    fill(value);
    w = want(side);
    for (i = 0; i < 854; i++) {
        for (j = 0; j < 9; j++) {
            w->cards[i].copies[j].unknown_00_bits_0_30 = 0;
            w->cards[i].copies[j].unknown_00_bit_31 = 0;
            w->cards[i].copies[j].unknown_04_bits_0_29 = 0;
            w->cards[i].copies[j].unknown_04_bits_30_31 = 3;
        }
        w->cards[i].rank = 0;
        w->cards[i].unknown_48_bit_14 = 0;
        w->cards[i].unknown_48_bit_15 = 0;
        w->cards[i].exp = 0;
    }
    for (i = 0; i < 5; i++) {
        w->newcomers[i] = 999;
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 40; j++) {
            w->decks[i].cards[j] = 999;
        }
        w->decks[i].leader = 999;
        w->decks[i].deckCost = 0;
    }
    memset(w->ownedAltArts, 0, 16);
    memset(w->treasureFlags, 0, 3);
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 7; j++) {
            for (k = 0; k < 7; k++) {
                w->unknown_fea3[i][j][k] = 7;
            }
        }
    }
    w->unknown_ff98 = 0;
    w->flags.unknown_bits_0_1 = 0;
    w->flags.unknown_bit_2 = 1;
    w->flags.unknown_bit_3 = 1;
    w->flags.unknown_bit_4 = 1;
    w->flags.unknown_bit_5 = 0;
    SzData_ClearData(side);
    assert(same());
    /* bytes the reset does not reach keep the fill */
    assert(selected(side)->unknown_fd92[0] == (unsigned char)value);
    assert(selected(side)->unknown_ff9d[0] == (unsigned char)value);
    assert(selected(side)->flags.unknown_bits_6_7 == ((unsigned)value >> 6 & 3));
}

static int clear_calls, clear_value, clear_count;
static void *clear_arg;
void RhMem_FillMemory(void *destination, int value, int size)
{
    clear_calls++; clear_arg = destination; clear_value = value; clear_count = size;
    memset(destination, value, (size_t)size);
}

/* SzData_Initialize (loop-bound coverage: the whole of both objects is compared with an image built
 * here, so every loop's first and last element and the bytes between the loops count). */
static void check_reset_both(void)
{
    struct DotrSide *w;
    int a, i, j, k;

    fill(0x5a);
    memset(expected, 0, sizeof(expected));
    for (a = 0; a < 2; a++) {
        w = &expected[a];
        for (i = 0; i < 854; i++) {
            for (j = 0; j < 9; j++) {
                w->cards[i].copies[j].unknown_04_bits_30_31 = 3;
            }
        }
        for (i = 0; i < 5; i++) {
            w->newcomers[i] = 999;
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 40; j++) {
                w->decks[i].cards[j] = 999;
            }
            w->decks[i].leader = 999;
        }
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 7; j++) {
                for (k = 0; k < 7; k++) {
                    w->unknown_fea3[i][j][k] = 7;
                }
            }
        }
        w->flags.unknown_bit_2 = 1;
        w->flags.unknown_bit_3 = 1;
        w->flags.unknown_bit_4 = 1;
        w->flags.unknown_bit_5 = 1;
    }
    clear_calls = 0;
    SzData_Initialize();
    assert(clear_calls == 1 && clear_arg == gSzData_All_sides && clear_value == 0 && clear_count == 0x1ff40);
    assert(same());
    assert(gSzData_All_sides[1].flags.unknown_bits_6_7 == 0);
}

static void check_placement_lookup(void)
{
    static const int invalid_id[] = {INT_MIN, -1, 22, INT_MAX};
    static const int invalid_coordinate[] = {INT_MIN, -1, 7, INT_MAX};
    static const int invalid_range[] = {INT_MIN, -1, 0};
    struct DotrSidePlacement before[22];
    size_t i;
    assert(offsetof(struct DotrSidePlacement, cardNo) == 2);
    memset(aTreasureData, 0xff, sizeof(aTreasureData));
    assert(SzData_SearchTreasure(0, 0, 0, 1) == 999);
    aTreasureData[0].duelist = 3;
    aTreasureData[0].unknown_01_bits_0_3 = 1;
    aTreasureData[0].unknown_01_bits_4_7 = 5;
    aTreasureData[0].cardNo = 123;
    aTreasureData[1] = aTreasureData[0];
    aTreasureData[1].cardNo = 456;
    /* Different nibble values detect a swapped coordinate view. */
    assert(((unsigned char *)&aTreasureData[0])[1] == 0x51);
    assert(SzData_SearchTreasure(3, 1, 5, 1) == 123);
    assert(SzData_SearchTreasure(3, 5, 1, 1) == 999);
    assert(SzData_SearchTreasure(4, 1, 5, INT_MAX) == 999);
    /* Each distance comparison is strict, for both signs of delta. */
    assert(SzData_SearchTreasure(3, 3, 3, 2) == 999);
    assert(SzData_SearchTreasure(3, 3, 3, 3) == 123);
    assert(SzData_SearchTreasure(3, 1, 3, 2) == 999);
    assert(SzData_SearchTreasure(3, 3, 5, 2) == 999);
    /* Skip a nonmatching first row and reach the final physical row. */
    aTreasureData[0].duelist = 4;
    assert(SzData_SearchTreasure(3, 1, 5, 1) == 456);
    aTreasureData[21].duelist = 21;
    aTreasureData[21].unknown_01_bits_0_3 = 15;
    aTreasureData[21].unknown_01_bits_4_7 = 15;
    aTreasureData[21].cardNo = 65535;
    assert(SzData_SearchTreasure(21, 6, 6, 9) == 999);
    assert(SzData_SearchTreasure(21, 6, 6, 10) == 65535);
    assert(SzData_SearchTreasure(21, 0, 0, 16) == 65535);
    memcpy(before, aTreasureData, sizeof(before));
    for (i = 0; i < sizeof(invalid_id) / sizeof(invalid_id[0]); i++)
        assert(SzData_SearchTreasure(invalid_id[i], 1, 5, 1) == 999);
    for (i = 0; i < sizeof(invalid_coordinate) / sizeof(invalid_coordinate[0]); i++) {
        assert(SzData_SearchTreasure(3, invalid_coordinate[i], 5, 1) == 999);
        assert(SzData_SearchTreasure(3, 1, invalid_coordinate[i], 1) == 999);
    }
    for (i = 0; i < sizeof(invalid_range) / sizeof(invalid_range[0]); i++)
        assert(SzData_SearchTreasure(3, 1, 5, invalid_range[i]) == 999);
    assert(memcmp(before, aTreasureData, sizeof(before)) == 0);
}

/* The group's functions outside the side-record helpers. Prototypes of their
 * callees are repeated here instead of including the callees' group headers. */
struct SoloVector { float x, y, z, w; };
struct SoloRect {
    unsigned char unknown_00[0x10];
    struct SoloVector unknown_10;
    struct SoloVector unknown_20;
    unsigned char unknown_30[0x68];
    short unknown_98;
    short unknown_9a;
};
int lSzDuel_SetCardCtl2(struct SoloRect *p, int a, int b, int c, int d, short e, short f);
int Ta_IsLDRangeAvility(int side, int pos, int ability);
int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
unsigned TaLogic_RemoveHideUnit(unsigned flagList);

static int solo_vec_calls, solo_rect_calls;
static struct SoloRect *solo_rect_seen;
void Ex3d_GetPointOnScreen(struct SoloVector *out, float x, float y)
{
    out->x = x;
    out->y = y;
    solo_vec_calls++;
}
void func_001cdeb0_view(struct SoloRect *p)
{
    assert(solo_vec_calls == 2 && p->unknown_98 == 6 && p->unknown_9a == 5);
    solo_rect_seen = p;
    solo_rect_calls++;
}

/* 0x24f520 mode: 0x271740 answers bit `a` of record_gate. */
static int record_on, record_gate;
static int solo_cell, solo_owner, solo_kind, solo_pre, solo_pre_args[3], solo_pair, solo_pair_args[2];
static int solo_owner_kind, solo_pair_calls, solo_owner_kind_calls;
static int solo_any;
int SzDuel_GetUnitLoc(int a, int b)
{
    if (solo_any) {
        return (a << 8) | b;
    }
    assert(a == 1 && b == 2);
    return solo_cell;
}
int Ta_GetCurrentSide(int a, int b) { assert(a == 1 && b == 2); return solo_owner; }
int SzDuel_GetUnitKind(int a, int b) { assert(record_on ? a == 1 : solo_any ? b == 0 : (a == 1 && b == 2)); return solo_kind; }
int Ta_IsLDRange(int a, int b, int c)
{
    solo_pre_args[0] = a;
    solo_pre_args[1] = b;
    solo_pre_args[2] = c;
    return solo_pre;
}
int Ta_IsLDAbl(int a, int b)
{
    solo_pair_args[0] = a;
    solo_pair_args[1] = b;
    solo_pair_calls++;
    if (record_on) {
        return record_gate >> a & 1;
    }
    return solo_pair;
}
int SzDuel_GetUnitConstKind(int a, int b)
{
    assert(a == solo_owner && b == 0);
    solo_owner_kind_calls++;
    return solo_owner_kind;
}

/* 0x25fd90 scan mode: one live cell, configurable grid answers. */
static int solo_scan_on, solo_scan_x, solo_scan_y, solo_scan_cell, solo_scan_calls, solo_scan_marks;
static int solo_scan_cb, solo_scan_a30, solo_scan_blocked, solo_scan_d10, solo_scan_a90, solo_scan_args[2];
static int solo_cells[3], solo_blocked[3], solo_merge_calls, solo_merge_args[3][3];
/* 0x2670f0 mode: per-cell blocked bits and per-cell modifier entries. */
static int modifier_on, modifier_calls, modifier_log[1024], modifier_blocked[2][11];
static struct DotrStateModifier modifier_entries[2][11];
static int modifier_present[2][11], modifier_final;
int Ta_GetUnitNumFrom22bitFlagList(unsigned value) { assert(value == 0x1000u); return 3; }
int Ta_GetUnitOwnerIDFromLstCount(int word, int n) { assert(word == 0x1000); return solo_cells[n]; }
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    int i;
    if (modifier_on) {
        modifier_log[modifier_calls++] = a << 16 | b << 8 | bit;
        return !(modifier_blocked[a][b] >> bit & 1);
    }
    assert(bit == 4);
    if (solo_scan_on) {
        return solo_scan_blocked;
    }
    for (i = 0; i < 3; i++) {
        if (solo_cells[i] == ((a << 8) | b)) {
            return solo_blocked[i];
        }
    }
    assert(0);
    return 0;
}
unsigned ConvUnitIDtoFlagList_OFF(unsigned x, int flag, int n)
{
    solo_merge_args[solo_merge_calls][0] = (int)x;
    solo_merge_args[solo_merge_calls][1] = flag;
    solo_merge_args[solo_merge_calls][2] = n;
    solo_merge_calls++;
    return (unsigned)n + 1;
}

struct SoloPlayer;
int videoDecIsFlushed(struct SoloPlayer *p);
void videoDecMain(struct SoloPlayer *p);
void TaView_ReqRotCamera(unsigned char b, int delta);
static unsigned char solo_player[0xb0];
static int solo_span_result, solo_ready_result, solo_ready_calls, solo_stop_log[3], solo_stop_count;
volatile int data_0035060c_volatile[4];
char voBuf[0x60];
int viBufCount(void *span) { assert(span == solo_player + 0x48); return solo_span_result; }
int func_0018ee68(struct SoloPlayer *p) { assert((void *)p == solo_player); solo_ready_calls++; return solo_ready_result; }
void func_00243f60_span(void *span) { assert(span == solo_player + 0x48); solo_stop_log[solo_stop_count++] = 1; }
void voBufReset(void *p) { assert(p == voBuf); solo_stop_log[solo_stop_count++] = 2; }
void decBs0(struct SoloPlayer *p) { assert((void *)p == solo_player); solo_stop_log[solo_stop_count++] = 3; }

static int solo_words[4], solo_back[4], solo_submit[5], solo_submit_calls;
static float solo_block[4], solo_out[4];
int SzView_FreeRequest(int a, int b) { assert(a == 0 && b == 0); return 0; }
float *func_001faf60_floats(int index) { assert(index == 0); return solo_block; }
void SzView_V2IV(float *block, int *out) { assert(block == solo_block); memcpy(out, solo_words, sizeof(solo_words)); }
/* Declared with these parameter types in state_records.h (included above). */
void SzView_IV2V(void *source, struct DotrStateVector *saved)
{
    memcpy(solo_back, source, sizeof(solo_back));
    *(float *)saved = 42.0f;
}
void SzView_ReqCamera(int a, int b, int kind, int d, int e, void *data)
{
    int v[5] = {a, b, kind, d, e};
    memcpy(solo_submit, v, sizeof(v));
    memcpy(solo_out, data, sizeof(float));
    solo_submit_calls++;
}

static int solo_wrap(int start, int delta)
{
    solo_words[0] = 7;
    solo_words[1] = start;
    solo_words[2] = 8;
    solo_words[3] = 9;
    TaView_ReqRotCamera(0xab, delta);
    assert(solo_back[0] == 7 && solo_back[2] == 8 && solo_back[3] == 9);
    assert(solo_submit[0] == 0 && solo_submit[1] == 0 && solo_submit[2] == 4 && solo_submit[3] == 0xab);
    assert(solo_submit[4] == 0 && solo_out[0] == 42.0f);
    return solo_back[1];
}

static void check_solo_homed_2(void)
{
    struct SoloPlayer *player = (struct SoloPlayer *)solo_player;

    /* videoDecIsFlushed (branch coverage): span busy, span idle with both results. */
    solo_span_result = 1;
    solo_ready_result = 1;
    assert(videoDecIsFlushed(player) == 0 && solo_ready_calls == 0);
    solo_span_result = 0;
    assert(videoDecIsFlushed(player) == 1 && solo_ready_calls == 1);
    solo_ready_result = 0;
    assert(videoDecIsFlushed(player) == 0 && solo_ready_calls == 2);

    /* 0x2452e0: call order and the final store, with the awaited word
     * already zero. A non-zero word would not terminate on the host, so
     * that outcome of the wait loop is not exercised. */
    data_0035060c_volatile[0] = 0;
    videoDecMain(player);
    assert(solo_stop_count == 3 && solo_stop_log[0] == 1 && solo_stop_log[1] == 2 && solo_stop_log[2] == 3);
    assert(*(int *)(solo_player + 0xa8) == 3);

    /* TaView_ReqRotCamera (branch coverage): sums below and at the 65535 bound on both
     * signs, with every forwarded argument checked in solo_wrap. */
    assert(solo_wrap(100, 20) == 120 && solo_wrap(100, -300) == -200);
    assert(solo_wrap(65000, 534) == 65534 && solo_wrap(65000, 535) == 0);
    assert(solo_wrap(-65000, -534) == -65534 && solo_wrap(-65000, -535) == -131070);
    assert(solo_submit_calls == 6);
}

void Ta_SwapUnit(int a, int b, int c, int d);
int TaHand_GetCardSummonCost2(int cardNo, int teamID);
static int solo_key, solo_put[3][4], solo_put_calls, solo_less, solo_less_calls;
int Ta_SearchMoveTemp(void) { return solo_key; }
void SzDuel_MoveUnit(int a, int b, int c, int d)
{
    int v[4] = {a, b, c, d};
    memcpy(solo_put[solo_put_calls++ % 3], v, sizeof(v));
}
int Ta_GetLDLvRev(int a) { assert(a == 6); solo_less_calls++; return solo_less; }

struct DotrStateRecordView data_00317490_recview[2];
unsigned char turnOwner[16];
static int solo_vec_log[2][5], solo_vec_count, *solo_words_seen;
static float solo_vec_data[2][3];
void func_001fc950_words(int *in, float *out)
{
    solo_words_seen = in;
    out[0] = 5.0f;
    out[1] = 6.0f;
    out[2] = 7.0f;
}
void func_001fa8a0_vec(int a, int b, int kind, unsigned short d, int e, float *data)
{
    int v[5] = {a, b, kind, d, e};
    memcpy(solo_vec_log[solo_vec_count], v, sizeof(v));
    memcpy(solo_vec_data[solo_vec_count], data, sizeof(solo_vec_data[0]));
    solo_vec_count++;
}

static void check_solo_homed_3(void)
{
    static const int put_want[3][4] = {{1, 2, 0x34, 0x12}, {3, 4, 2, 1}, {1, 2, 4, 3}};
    static const int vec_want[2][5] = {{0, 0, 3, 0x2345, 0}, {0, 0, 4, 0x2345, 0}};
    unsigned char *bytes = (unsigned char *)&entry_fixture[40];
    unsigned char saved[3];

    /* Ta_SwapUnit (branch coverage): negative key, then the three writes. */
    solo_any = 1;
    solo_key = -1;
    Ta_SwapUnit(1, 2, 3, 4);
    assert(solo_put_calls == 0);
    solo_key = 0x1234;
    Ta_SwapUnit(1, 2, 3, 4);
    assert(solo_put_calls == 3 && memcmp(solo_put, put_want, sizeof(put_want)) == 0);

    /* TaHand_GetCardSummonCost2 (branch coverage): no entry, upper kind bits set, kind
     * mismatch, kind match. */
    memcpy(saved, bytes, sizeof(saved));
    solo_kind = 0x25;
    solo_less = 3;
    bytes[0] = 0x25;
    bytes[2] = 0x9c;
    assert(TaHand_GetCardSummonCost2(-1, 6) == 255);
    assert(TaHand_GetCardSummonCost2(40, 6) == 9 && solo_less_calls == 0);
    bytes[0] = 0x04;
    assert(TaHand_GetCardSummonCost2(40, 6) == 9 && solo_less_calls == 0);
    bytes[0] = 0x05;
    assert(TaHand_GetCardSummonCost2(40, 6) == 6 && solo_less_calls == 1);
    memcpy(bytes, saved, sizeof(saved));
    solo_any = 0;

    /* lSzDuel_LockCamera (straight-line coverage): record selection, both submissions,
     * 16-bit narrowing of b and the signed byte. */
    turnOwner[0] = 1;
    data_00317490_recview[1].unknown_345 = -3;
    lSzDuel_LockCamera(0x12345, 10, -20);
    assert(solo_words_seen == data_00317490_recview[1].unknown_1d0 && solo_vec_count == 2);
    assert(memcmp(solo_vec_log, vec_want, sizeof(vec_want)) == 0);
    assert(solo_vec_data[0][0] == 10.0f && solo_vec_data[0][1] == -3.0f && solo_vec_data[0][2] == -20.0f);
    assert(solo_vec_data[1][0] == 5.0f && solo_vec_data[1][1] == 6.0f && solo_vec_data[1][2] == 7.0f);
}

void TaView_RestoreOriginalView(void *p, unsigned char b);
void TaTuto_RestoreOriginalView(char *a, unsigned char b);
void TaView_ReqHalfFarView(unsigned char b);
void TaView_ReqFocusCamera2(unsigned char b, int cell, int value);
static int solo_tag_log[2][5], solo_tag_count, solo_cell_args[3], solo_convert_value;
static void *solo_tag_data[2], *solo_cell_vector;
static float solo_first[4];
void func_001fa8a0_tag(int a, int b, int kind, unsigned short d, int e, void *data)
{
    int v[5] = {a, b, kind, d, e};
    memcpy(solo_tag_log[solo_tag_count & 1], v, sizeof(v));
    solo_tag_data[solo_tag_count & 1] = data;
    solo_tag_count++;
}
float *func_001f9d30_floats(int index) { assert(index == 0); return solo_first; }
void SzDuel_CalcUnitPos(int x, int y, int z, struct DotrStateVector *v)
{
    solo_cell_args[0] = x;
    solo_cell_args[1] = y;
    solo_cell_args[2] = z;
    solo_cell_vector = v;
}
void Ta_GetStdFocusView(int *out, int value)
{
    solo_convert_value = value;
    out[0] = 70;
    out[1] = 71;
    out[2] = 72;
    out[3] = 73;
}

static void solo_tag_expect(void *first, void *second)
{
    static const int want[2][5] = {{0, 0, 3, 0xab, 0}, {0, 0, 4, 0xab, 0}};
    assert(solo_tag_count == 2 && memcmp(solo_tag_log, want, sizeof(want)) == 0);
    assert((first == 0 || solo_tag_data[0] == first) && (second == 0 || solo_tag_data[1] == second));
    solo_tag_count = 0;
}

/* TaView_RestoreOriginalView, TaTuto_RestoreOriginalView, TaView_ReqHalfFarView,
 * TaView_ReqFocusCamera2 (straight-line coverage): both submissions with every argument, plus each
 * function's own stores. */
static void check_solo_homed_4(void)
{
    static unsigned char object[0x104];
    char pair[32];

    memset(object, 0x77, sizeof(object));
    TaView_RestoreOriginalView(object, 0xab);
    solo_tag_expect(object + 0xe0, object + 0xf0);
    assert(object[0x102] == 0 && object[0x103] == 0 && object[0x101] == 0x77);

    TaTuto_RestoreOriginalView(pair, 0xab);
    solo_tag_expect(pair, pair + 16);

    solo_block[0] = 1.5f;
    solo_block[1] = 4.0f;
    TaView_ReqHalfFarView(0xab);
    solo_tag_expect(solo_first, solo_block);
    assert(solo_block[0] == 3.0f && solo_block[1] == 4.0f);

    TaView_ReqFocusCamera2(0xab, 0x1234, 55);
    assert(solo_cell_args[0] == 0x34 && solo_cell_args[1] == 0x12 && solo_cell_args[2] == 0);
    assert(solo_convert_value == 55 && solo_back[0] == 70 && solo_back[3] == 73);
    assert(solo_tag_data[0] == solo_cell_vector && solo_tag_data[1] != solo_cell_vector);
    solo_tag_expect(0, 0);
}

static void check_solo_homed(void)
{
    struct SoloRect rect;
    int pass;

    /* lSzDuel_SetCardCtl2 (branch coverage): null object, then every store and call. */
    assert(lSzDuel_SetCardCtl2(0, 1, 2, 3, 4, 5, 6) == 0 && solo_vec_calls == 0 && solo_rect_calls == 0);
    memset(&rect, 0, sizeof(rect));
    assert(lSzDuel_SetCardCtl2(&rect, -1, 2, 3, 4, 5, 6) == 1 && solo_rect_calls == 1 && solo_rect_seen == &rect);
    assert(rect.unknown_10.x == -1.0f && rect.unknown_10.y == 2.0f);
    assert(rect.unknown_20.x == 3.0f && rect.unknown_20.y == 4.0f);

    /* Ta_IsLDRangeAvility and Ta_IsLDRangeAvilityWithLocation (branch coverage): each rejection and the
     * accepted case; pass 0 takes the cell from SzDuel_GetUnitLoc, pass 1 from
     * the arguments. */
    for (pass = 0; pass < 2; pass++) {
        solo_cell = pass == 0 ? 0x1234 : 0;
        solo_owner = 9;
        solo_kind = 0x47;
        solo_owner_kind = 0x27;
        solo_pre = 0;
        solo_pair = 1;
        solo_pair_calls = solo_owner_kind_calls = 0;
#define SOLO_CALL() (pass == 0 ? Ta_IsLDRangeAvility(1, 2, 30) : Ta_IsLDRangeAvilityWithLocation(1, 2, 0x34, 0x12, 30))
        assert(SOLO_CALL() == 0 && solo_pair_calls == 0);
        assert(solo_pre_args[0] == 9 && solo_pre_args[1] == 0x34 && solo_pre_args[2] == 0x12);
        solo_pre = 1;
        solo_pair = 0;
        assert(SOLO_CALL() == 0 && solo_pair_calls == 1 && solo_owner_kind_calls == 0);
        assert(solo_pair_args[0] == 30 && solo_pair_args[1] == 9);
        solo_pair = 1;
        assert(SOLO_CALL() == 1 && solo_owner_kind_calls == 1);
        solo_owner_kind = 0x26;
        assert(SOLO_CALL() == 0 && solo_owner_kind_calls == 2);
#undef SOLO_CALL
    }

    /* TaLogic_RemoveHideUnit (loop and branch coverage): three cells, the middle one
     * blocked; the accumulator chains through the two merges. */
    solo_cells[0] = 0x0102;
    solo_cells[1] = 0x0304;
    solo_cells[2] = 0x0506;
    solo_blocked[1] = 1;
    assert(TaLogic_RemoveHideUnit(0x1000u) == 0x1002u && solo_merge_calls == 2);
    assert(solo_merge_args[0][0] == 1 && solo_merge_args[0][1] == 2 && solo_merge_args[0][2] == 0x1000);
    assert(solo_merge_args[1][0] == 5 && solo_merge_args[1][1] == 6 && solo_merge_args[1][2] == 0x1001);
}

/* 0x1a9800 SzGame_ModeDrawAfter. */
struct SoloModeView {
    unsigned char unknown_000[0x228];
    unsigned char busy;
    unsigned char unknown_229[3];
    unsigned char unknown_bits_0_3 : 4;
    unsigned char mode : 2;
};
struct SoloModeView gSzGame_gl_mode;
void SzGame_ModeDrawAfter(void);
static int solo_mode_result, solo_mode_log[8], solo_mode_count;
int SzGame_GetMode(int a) { assert(a == 1); solo_mode_log[solo_mode_count++] = 100; return solo_mode_result; }
void SzDuel_DrawJoinFade(void) { solo_mode_log[solo_mode_count++] = 1; }
void SzDuel_DrawJoinCard(void) { solo_mode_log[solo_mode_count++] = 2; }
void ExDitherMatrixEz(int a) { assert(a == 2); solo_mode_log[solo_mode_count++] = 3; }
void TkCamp_Draw_After(void) { solo_mode_log[solo_mode_count++] = 4; }
void ShFace_Draw(void) { solo_mode_log[solo_mode_count++] = 5; }
void SzDuel_DrawDialog(void) { solo_mode_log[solo_mode_count++] = 6; }

static void solo_mode_run(int mode, int busy, int result, const int *expect, int n)
{
    int i;

    memset(&gSzGame_gl_mode, 0, sizeof(gSzGame_gl_mode));
    gSzGame_gl_mode.unknown_bits_0_3 = 15;
    gSzGame_gl_mode.mode = mode;
    gSzGame_gl_mode.busy = busy;
    solo_mode_result = result;
    solo_mode_count = 0;
    SzGame_ModeDrawAfter();
    assert(solo_mode_count == n);
    for (i = 0; i < n; i++) {
        assert(solo_mode_log[i] == expect[i]);
    }
}

static void check_solo_homed_5(void)
{
    static const int none[] = { 100 };
    static const int other[] = { 100, 6 };
    static const int one[] = { 100, 1, 2, 6 };
    static const int eleven[] = { 100, 3, 4, 5, 6 };

    solo_mode_run(3, 0, 1, none, 1);
    solo_mode_run(2, 1, 11, none, 1);
    solo_mode_run(2, 0, 1, one, 4);
    solo_mode_run(0, 0, 11, eleven, 5);
    solo_mode_run(2, 0, 0, other, 2);
    solo_mode_run(2, 0, 2, other, 2);
    solo_mode_run(2, 0, 12, other, 2);
}

/* 0x1fc9b0 SzView_InitCamera. */
struct SoloLabelHeader {
    unsigned char unknown_00[12];
    unsigned short unknown_0c;
};
struct SoloLabelRecord {
    int status;
    short selector;
    short priority;
    signed int byte_size : 31;
    signed int flag_008 : 1;
    int unknown_00c[2];
    struct SoloLabelHeader *header;
};
struct SoloLabelHeader *gSzView_pCamera_header[4];
unsigned short gSzView_wFrame[8];
char data_002cfc50[] = "format";
char data_002cfc67[] = "label";
int SzView_InitCamera(int currentMode);
static char solo_label_text[] = "text";
static struct SoloLabelRecord solo_label_record, *solo_label_result;
static int solo_label_log[4], solo_label_count;
void SzView_TermCamera(void) { solo_label_log[solo_label_count++] = 1; }
char *SzFile_GetEnv(const char *label)
{
    assert(label == data_002cfc67);
    solo_label_log[solo_label_count++] = 2;
    return solo_label_text;
}
int func_0019b648(char *destination, const char *format, ...)
{
    va_list args;
    int a, b;

    assert(format == data_002cfc50);
    va_start(args, format);
    assert(va_arg(args, char *) == solo_label_text);
    a = va_arg(args, int);
    b = va_arg(args, int);
    va_end(args);
    assert(a == 77 && b == 77);
    strcpy(destination, "built");
    solo_label_log[solo_label_count++] = 3;
    return 5;
}
void *SzFile_ReadParam(int priority, char *name, int unknown_014, int byte_size, int byte_offset, int unknown_010)
{
    assert(priority == 1 && strcmp(name, "built") == 0 && unknown_014 == 0);
    assert(byte_size == -1 && byte_offset == 0 && unknown_010 == -1);
    solo_label_log[solo_label_count++] = 4;
    return solo_label_result;
}

static int solo_label_run(struct SoloLabelRecord *result, int byte_size, struct SoloLabelHeader *header)
{
    int r;

    solo_label_record.byte_size = byte_size;
    solo_label_record.flag_008 = -1;
    solo_label_record.header = header;
    solo_label_result = result;
    solo_label_count = 0;
    gSzView_pCamera_header[0] = 0;
    gSzView_wFrame[0] = 0x1111;
    r = SzView_InitCamera(77);
    assert(solo_label_count == 4 && solo_label_log[0] == 1 && solo_label_log[1] == 2);
    assert(solo_label_log[2] == 3 && solo_label_log[3] == 4);
    return r;
}

static void check_solo_homed_6(void)
{
    struct SoloLabelHeader header;

    memset(&header, 0, sizeof(header));
    header.unknown_0c = 0xbeef;
    assert(solo_label_run(0, 5, &header) == 0);
    assert(gSzView_pCamera_header[0] == 0 && gSzView_wFrame[0] == 0x1111);
    assert(solo_label_run(&solo_label_record, 0, &header) == 0);
    assert(gSzView_pCamera_header[0] == 0 && gSzView_wFrame[0] == 0x1111);
    assert(solo_label_run(&solo_label_record, 5, 0) == 0);
    assert(gSzView_pCamera_header[0] == 0 && gSzView_wFrame[0] == 0x1111);
    assert(solo_label_run(&solo_label_record, 5, &header) == 1);
    assert(gSzView_pCamera_header[0] == &header && gSzView_wFrame[0] == 0xbeef);
}

/* 0x21c450 Taku_Bunsyou_Set. SoloWord is DotrDrawWord on the host. */
typedef intptr_t SoloWord;
SoloWord Taku_Bunsyou_Set(SoloWord cursor, int index, int a2, int x, int y, int unused, int e);
static int solo_sprite_log[5], solo_sprite_count;
SoloWord SzMsg_LoadFont_Pipe(SoloWord cursor, int a0, int a1, int a2, int a3)
{
    assert(cursor == 0x1000 && a0 == 9 && a1 == 31 && a2 == 0 && a3 == 9 * 22);
    solo_sprite_log[solo_sprite_count++] = 1;
    return 0x2000;
}
SoloWord RhText_SetTextureFlush(SoloWord cursor)
{
    assert(cursor == 0x2000);
    solo_sprite_log[solo_sprite_count++] = 2;
    return 0x3000;
}
void *SzMsg_GetInfo(int index)
{
    assert(index == 9);
    solo_sprite_log[solo_sprite_count++] = 3;
    return 0;
}
void SzMsg_DrawRect_Pipe(void) {}
SoloWord SzMsg_DrawRatioX_Pipe(SoloWord cursor, int index, short x, short y, int e, void (*function)(void), float sx, float sy)
{
    assert(cursor == 0x3000 && index == 9 && x == 0x1234 && y == -2 && e == 55);
    assert(function == SzMsg_DrawRect_Pipe && sx == 1.0f && sy == 1.0f);
    solo_sprite_log[solo_sprite_count++] = 4;
    return 0x4000;
}
void SzMsg_Free(int index)
{
    assert(index == 9);
    solo_sprite_log[solo_sprite_count++] = 5;
}

static void check_solo_homed_7(void)
{
    int i;

    /* Straight-line: x and y are narrowed to 16 bits; the sixth argument is unused. */
    assert(Taku_Bunsyou_Set(0x1000, 9, 31, 0x71234, 0x1fffe, 123, 55) == 0x4000);
    assert(solo_sprite_count == 5);
    for (i = 0; i < 5; i++) {
        assert(solo_sprite_log[i] == i + 1);
    }
}

/* 0x2451a0 videoDecFlush. Uses solo_player (span at +0x48, word at +0xa8). */
struct SoloFloatWord { float value; };
struct SoloFloatWord data_0029b300[4] = { { 2.5f } };
int data_003506c8[4];
int videoDecFlush(struct SoloPlayer *p);
static int solo_fill_out[4], solo_fill_log[3], solo_fill_count, solo_fill_state;
int func_002440f0_five(void *span, int *a, int *b, int *c, int *d)
{
    assert(span == solo_player + 0x48);
    *a = solo_fill_out[0];
    *b = solo_fill_out[1];
    *c = solo_fill_out[2];
    *d = solo_fill_out[3];
    solo_fill_log[solo_fill_count++] = 1;
    return 0;
}
int cpy2area(char *first, int first_size, char *second, int second_size, char *a, int a_size, char *b, int b_size)
{
    float value;

    assert(first == (char *)(intptr_t)0x23456789 && first_size == solo_fill_out[1]);
    assert(second == (char *)(intptr_t)0x2edcba98 && second_size == solo_fill_out[3]);
    memcpy(&value, a, sizeof(value));
    assert(value == 2.5f && a_size == 4 && b == 0 && b_size == 0);
    return 0x77;
}
void func_002441f0_count(void *p, int n)
{
    assert(p == data_003506c8 && n == 0x77);
    solo_fill_log[solo_fill_count++] = 2;
}
void viBufFlush(void *span)
{
    assert(span == solo_player + 0x48);
    solo_fill_log[solo_fill_count++] = 3;
}

static int solo_fill_run(int b, int d, int state)
{
    int r;

    solo_fill_out[0] = (int)0xf3456789;
    solo_fill_out[1] = b;
    solo_fill_out[2] = (int)0x9edcba98;
    solo_fill_out[3] = d;
    solo_fill_count = 0;
    memcpy(solo_player + 0xa8, &state, sizeof(state));
    r = videoDecFlush((struct SoloPlayer *)solo_player);
    memcpy(&state, solo_player + 0xa8, sizeof(state));
    solo_fill_state = state;
    return r;
}

static void check_solo_homed_8(void)
{
    /* Bound: b + d < 4 returns 0 after the first callee only. */
    assert(solo_fill_run(1, 2, 0) == 0 && solo_fill_count == 1 && solo_fill_state == 0);
    assert(solo_fill_run(4, -1, 5) == 0 && solo_fill_count == 1 && solo_fill_state == 5);
    /* Boundary b + d == 4: both later callees; a zero state word becomes 2. */
    assert(solo_fill_run(1, 3, 0) == 1 && solo_fill_count == 3 && solo_fill_state == 2);
    assert(solo_fill_log[0] == 1 && solo_fill_log[1] == 2 && solo_fill_log[2] == 3);
    /* A non-zero state word is kept. */
    assert(solo_fill_run(100, 3, 5) == 1 && solo_fill_count == 3 && solo_fill_state == 5);
}

/* termAll (straight-line): the stubs log each call with its argument. */
int readBuf[4] = { 0x788 }, videoDecTh[4] = { 0x780 }, defaultTh[4] = { 0x778 };
int data_00350730[4] = { 0x730, 0x734 }, videoDec[4], infile[4];
char audioDec[0x20];
void termAll(void);
static intptr_t solo_close_log[13][2];
static int solo_close_count;
static void solo_close(int id, intptr_t arg)
{
    solo_close_log[solo_close_count][0] = id;
    solo_close_log[solo_close_count][1] = arg;
    solo_close_count++;
}
void func_00241ac0_word(int a) { solo_close(1, a); }
void func_002457c0_object(void *p) { solo_close(2, (intptr_t)p); }
void func_0017b730(int a) { solo_close(3, a); }
void func_0017b6f0(int a) { solo_close(4, a); }
void func_0017c490(int a) { solo_close(5, a); }
void func_0017b610(int a, int b) { assert(a == 2); solo_close(6, b); }
void func_0017c3c0(int a) { solo_close(7, a); }
void func_0017b5e0(int a, int b) { assert(a == 2); solo_close(8, b); }
int videoDecDelete(void *p) { solo_close(9, (intptr_t)p); return 0; }
int audioDecDelete(void *p) { solo_close(10, (intptr_t)p); return 0; }
int strFileClose(void *p) { solo_close(11, (intptr_t)p); return 0; }

static void check_solo_homed_9(void)
{
    const intptr_t expect[13][2] = {
        { 1, 0x788 }, { 2, (intptr_t)voBuf }, { 3, 0x780 }, { 4, 0x780 }, { 3, 0x778 }, { 4, 0x778 },
        { 5, 2 }, { 6, 0x730 }, { 7, 2 }, { 8, 0x734 }, { 9, (intptr_t)videoDec },
        { 10, (intptr_t)audioDec }, { 11, (intptr_t)infile },
    };

    solo_close_count = 0;
    termAll();
    assert(solo_close_count == 13 && memcmp(solo_close_log, expect, sizeof(expect)) == 0);
}

/* videoDecCreate (straight-line): every forwarded argument, the five registered
 * callbacks in order and the return value. Uses solo_player (span at +0x48). */
typedef int (*SoloCallback)(void);
int mpegError(void) { return 0; }
int mpegNodata(void) { return 0; }
int mpegStopDMA(void) { return 0; }
int mpegRestartDMA(void) { return 0; }
int mpegTS(void) { return 0; }
int videoDecCreate(struct SoloPlayer *p, int a, int b, int c, unsigned d, int e, int f, int g);
static int solo_setup_step, solo_setup_kinds[5];
static SoloCallback solo_setup_functions[5];
void func_0018eb08(struct SoloPlayer *p, int a, int b)
{
    assert((void *)p == solo_player && a == 11 && b == 12 && solo_setup_step == 0);
    solo_setup_step++;
}
void func_0018ef20(struct SoloPlayer *p, int kind, SoloCallback function, int d)
{
    assert((void *)p == solo_player && d == 0 && solo_setup_step >= 1 && solo_setup_step <= 5);
    solo_setup_kinds[solo_setup_step - 1] = kind;
    solo_setup_functions[solo_setup_step - 1] = function;
    solo_setup_step++;
}
void func_002450f0_player(struct SoloPlayer *p)
{
    assert((void *)p == solo_player && solo_setup_step == 6);
    solo_setup_step++;
}
int func_00243ee0_span(void *span, int a, unsigned b, int c, int d, int e)
{
    assert(span == solo_player + 0x48 && solo_setup_step == 7);
    assert(a == 13 && b == 0xfedcba98u && c == 15 && d == 16 && e == 17);
    solo_setup_step++;
    return 99;
}

static void check_solo_homed_10(void)
{
    static const int kinds[5] = { 0, 1, 2, 3, 5 };
    static const SoloCallback functions[5] = {
        mpegError, mpegNodata, mpegStopDMA, mpegRestartDMA, mpegTS
    };
    int i;

    solo_setup_step = 0;
    assert(videoDecCreate((struct SoloPlayer *)solo_player, 11, 12, 13, 0xfedcba98u, 15, 16, 17) == 1);
    assert(solo_setup_step == 8);
    for (i = 0; i < 5; i++) {
        assert(solo_setup_kinds[i] == kinds[i] && solo_setup_functions[i] == functions[i]);
    }
}

static int solo_grid_log[8][5], solo_grid_count;
static int solo_grid(int id, int a, int b, int c, int d)
{
    int v[5] = { id, a, b, c, d };
    if (solo_scan_on) {
        solo_scan_marks += id == 0x1840;
        return 0;
    }
    assert(solo_grid_count < 8);
    memcpy(solo_grid_log[solo_grid_count++], v, sizeof(v));
    return 0;
}
int SzDuel_SetUnitCtlModeC(int a, int b, int value) { return solo_grid(0x1840, a, b, value, -1); }
int SzDuel_SetUnitCtlModeM(int a, int b, int value) { return solo_grid(0x18a0, a, b, value, -1); }
int SzDuel_SetUnitCtlGoalM(int a, int b, int value) { return solo_grid(0x19d0, a, b, value, -1); }
int SzDuel_SetUnitCtlGoalC(int a, int b, int value) { return solo_grid(0x1970, a, b, value, -1); }
int SzDuel_SetUnitCtlRateM(int a, int b, int pair, int value) { return solo_grid(0x1c10, a, b, pair, value); }
int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value) { return solo_grid(0x1b70, a, b, pair, value); }

/* 0x25ff30 branch coverage: blocked cell, column 0, and neither. */
static void solo_grid_case(int a, int b, int blocked, int pair, int value)
{
    static const int ids[6] = { 0x1840, 0x18a0, 0x19d0, 0x1c10, 0x1970, 0x1b70 };
    int expect[6][2] = { { 1, -1 }, { 7, -1 }, { 128, -1 }, { 0, 0 }, { 128, -1 }, { 1, 128 } };
    int i;

    expect[3][0] = pair;
    expect[3][1] = value;
    solo_cells[0] = (a << 8) | b;
    solo_cells[1] = solo_cells[2] = -1;
    solo_blocked[0] = blocked;
    solo_grid_count = 0;
    TaEff_RestoreStdRate(a, b);
    assert(solo_grid_count == 6);
    for (i = 0; i < 6; i++) {
        assert(solo_grid_log[i][0] == ids[i] && solo_grid_log[i][1] == a && solo_grid_log[i][2] == b);
        assert(solo_grid_log[i][3] == expect[i][0] && solo_grid_log[i][4] == expect[i][1]);
    }
}

static int solo_front[5], solo_start, solo_limit, solo_limit_calls;
int SzDuel_InqHand(int index, int position) { assert(index == 1 && position >= 0 && position < 5); return solo_front[position]; }
int SzDuel_GetHandCount(int index) { assert(index == 1); return solo_start; }
static int tail_on, tail_count, tail_ids[4];
static unsigned short fill_words[22];
int SzDuel_InqDeck(int index, int position)
{
    assert(index == 1);
    if (tail_on) {
        assert(position >= 0 && position < tail_count);
        return tail_ids[position];
    }
    return 0x900 + position;
}
int SzDuel_GetDeckCount(int index) { assert(tail_on && index == 1); return tail_count; }
void TaUtil_GetUnitLst(int kind, int mode, unsigned short *out)
{
    int i;
    assert(tail_on && kind == 1 && mode == 1);
    /* the caller hands over a buffer of 22 empty halfwords */
    for (i = 0; i < 22; i++) assert(out[i] == 0xffff);
    memcpy(out, fill_words, sizeof(fill_words));
}
int TaHand_GetNextHandNum(int index) { assert(index == 1); solo_limit_calls++; return solo_limit; }

/* 0x26fd30 branch coverage: kept and skipped (999) front words, empty and non-empty tail. */
static void solo_collect_cases(void)
{
    int out[8], i;

    for (i = 0; i < 8; i++) {
        out[i] = -5;
    }
    /* Low 12 bits kept; 999 (also with high bits set) skipped; tail empty at start == limit. */
    solo_front[0] = 0x5123; solo_front[1] = 999; solo_front[2] = 0x7000 + 999; solo_front[3] = 998; solo_front[4] = 0;
    solo_start = solo_limit = 3;
    solo_limit_calls = 0;
    TaHand_GetNextHand(1, out);
    assert(out[0] == 0x123 && out[1] == 998 && out[2] == 0 && out[3] == -5 && solo_limit_calls == 1);
    /* All five skipped; tail writes start .. limit - 1 with positions from 0. */
    for (i = 0; i < 5; i++) {
        solo_front[i] = 999;
    }
    out[0] = out[1] = out[2] = -5;
    solo_start = 2;
    solo_limit = 4;
    solo_limit_calls = 0;
    TaHand_GetNextHand(1, out);
    assert(out[0] == -5 && out[1] == -5 && out[2] == 0x900 && out[3] == 0x901 && out[4] == -5);
    assert(solo_limit_calls == 3);
}

static struct DotrSideFadeOwner solo_fade_owner;
static int solo_fade_result, solo_fade_log[4], solo_fade_word;
static float solo_fade_xy[2];
int func_001da2b0_owner(struct DotrSideFadeOwner *owner, int a, float x, float y)
{
    assert(owner == &solo_fade_owner && a == 0);
    solo_fade_log[0]++;
    solo_fade_xy[0] = x;
    solo_fade_xy[1] = y;
    return solo_fade_result;
}
int func_001d9f00_owner(struct DotrSideFadeOwner *owner, int a)
{
    assert(owner == &solo_fade_owner && a == 1);
    solo_fade_log[1]++;
    return solo_fade_result;
}
void func_001da7a0_owner(int a, struct DotrSideFadeOwner *owner)
{
    assert(owner == &solo_fade_owner);
    solo_fade_log[2]++;
    solo_fade_word = a;
}
void func_001dbcf0_owner(struct DotrSideFadeOwner *owner)
{
    assert(owner == &solo_fade_owner && solo_fade_log[2] == 1);
    solo_fade_log[3]++;
}

/* 0x27f180 branch coverage: point present/absent, callee result zero/non-zero, count 0 and 2. */
static void solo_fade_case(struct DotrSidePoint *point, int result, int count)
{
    struct DotrSideFadeRow rows[3];
    int i;

    memset(rows, 0x5a, sizeof(rows));
    memset(&solo_fade_owner, 0, sizeof(solo_fade_owner));
    memset(solo_fade_log, 0, sizeof(solo_fade_log));
    solo_fade_owner.rows = rows;
    solo_fade_owner.count = count;
    solo_fade_owner.unknown_82_bits_12_15 = 5;
    solo_fade_owner.unknown_90 = 0x4321;
    solo_fade_result = result;
    solo_fade_word = -1;
    assert(Ta_LkPrim_DrawCard(&solo_fade_owner, 0x81, point) == 0);
    assert(solo_fade_log[0] == (point != 0) && solo_fade_log[1] == (point == 0));
    assert(solo_fade_log[2] == 1 && solo_fade_log[3] == 1 && solo_fade_word == (result ? 0x4321 : 0));
    if (point) {
        assert(solo_fade_xy[0] == 1.5f && solo_fade_xy[1] == -2.0f);
    }
    for (i = 0; i < 3; i++) {
        assert(rows[i].unknown_10 == 0x5a5a5a5a && rows[i].unknown_17 == 0x5a && rows[i].unknown_18[2] == 0x5a);
        assert(rows[i].color == (i < count ? 0xffffffu : 0x5a5a5au) && rows[i].alpha == (i < count ? 0x81 : 0x5a));
    }
    assert(solo_fade_owner.count == count && solo_fade_owner.unknown_82_bits_12_15 == 5);
}

int SzDuel_SetUnitCtlGoal(int a, int b, int value) { return solo_grid(0x1900, a, b, value, -1); }
int SzDuel_GetUnitCtlRateC(int a, int b)
{
    if (solo_scan_on) {
        solo_scan_args[0] = a;
        solo_scan_args[1] = b;
        return solo_scan_cb;
    }
    solo_grid(0x1cb0, a, b, -1, -1);
    return 0x61;
}
int SzDuel_GetUnitCtlRateM(int a, int b)
{
    if (solo_scan_on) {
        return solo_scan_d10;
    }
    solo_grid(0x1d10, a, b, -1, -1);
    return 0x72;
}
int SzDuel_SetUnitStatus(int a, int b, int bit, int value) { return solo_grid(0x1e70, a, b, bit, value); }

/* 0x2516d0 branch coverage: mode 1 and mode other than 1. */
static void solo_mode_cases(void)
{
    static const int on[7][3] = { { 0x1900, 128, -1 }, { 0x1cb0, -1, -1 }, { 0x1b70, 1, 0x61 }, { 0x1d10, -1, -1 },
                                  { 0x1c10, 1, 0x72 }, { 0x1e70, 2, 1 }, { 0x1e70, 4, 1 } };
    static const int off[5][3] = { { 0x19d0, 0, -1 }, { 0x1d10, -1, -1 }, { 0x1c10, -1, 0x72 }, { 0x1e70, 2, 0 },
                                   { 0x1e70, 4, 0 } };
    int i, mode;

    solo_grid_count = 0;
    TaUtil_DoOpen(4, 6, 1);
    assert(solo_grid_count == 7);
    for (i = 0; i < 7; i++) {
        assert(solo_grid_log[i][0] == on[i][0] && solo_grid_log[i][1] == 4 && solo_grid_log[i][2] == 6);
        assert(solo_grid_log[i][3] == on[i][1] && solo_grid_log[i][4] == on[i][2]);
    }
    for (mode = 0; mode <= 2; mode += 2) {
        solo_grid_count = 0;
        TaUtil_DoOpen(4, 6, mode);
        assert(solo_grid_count == 5);
        for (i = 0; i < 5; i++) {
            assert(solo_grid_log[i][0] == off[i][0] && solo_grid_log[i][1] == 4 && solo_grid_log[i][2] == 6);
            assert(solo_grid_log[i][3] == off[i][1] && solo_grid_log[i][4] == off[i][2]);
        }
    }
}

struct DotrSideVector data_00291120 = { 2.0f, 3.0f, 4.0f, 5.0f };
static int solo_arrow_gate, solo_arrow_mask, solo_arrow_count;
static struct DotrSideVector solo_arrow_seen[4];
static int solo_arrow_index[4];
int SzModel_GetDrawFlag(int index) { assert(index == 0); return solo_arrow_gate; }
int SzModel_IsModelDisp(int index) { assert(index == 3 || index == 4); return solo_arrow_mask >> index & 1; }
int SzModel_ExSort_Pipe(int cursor, int index, struct DotrSideVector *v, int a, int b, int c, int d)
{
    assert(cursor == 100 + solo_arrow_count && a == 0 && b == 0 && c == 0 && d == 0);
    solo_arrow_index[solo_arrow_count] = index;
    solo_arrow_seen[solo_arrow_count++] = *v;
    return cursor + 1;
}
int SzDuel_DrawLine_Pipe(int cursor) { return cursor + 1000; }

/* 0x1aeeb0 branch coverage: gate closed; each index drawn or skipped; z sign per index. */
static void solo_arrow_cases(void)
{
    solo_arrow_gate = 0;
    solo_arrow_mask = 0x18;
    solo_arrow_count = 0;
    assert(SzDuel_DrawStageAfter_Pipe(100) == 100 && solo_arrow_count == 0);
    solo_arrow_gate = 1;
    solo_arrow_mask = 0;
    assert(SzDuel_DrawStageAfter_Pipe(100) == 1100 && solo_arrow_count == 0);
    solo_arrow_mask = 0x18;
    assert(SzDuel_DrawStageAfter_Pipe(100) == 1104 && solo_arrow_count == 4);
    assert(solo_arrow_index[0] == 3 && solo_arrow_index[1] == 3 && solo_arrow_index[2] == 4 && solo_arrow_index[3] == 4);
    assert(solo_arrow_seen[0].x == 2.0f && solo_arrow_seen[0].y == 3.0f && solo_arrow_seen[0].z == -180.0f);
    assert(solo_arrow_seen[0].unknown_w == 5.0f && solo_arrow_seen[1].x == -2.0f && solo_arrow_seen[1].z == -180.0f);
    assert(solo_arrow_seen[2].x == 2.0f && solo_arrow_seen[2].z == 180.0f && solo_arrow_seen[3].x == -2.0f);
    assert(data_00291120.x == 2.0f && data_00291120.z == 4.0f);
    solo_arrow_mask = 0x10;
    solo_arrow_count = 0;
    assert(SzDuel_DrawStageAfter_Pipe(100) == 1102 && solo_arrow_count == 2 && solo_arrow_index[0] == 4);
}

/* Local mirrors of the small_misc.h views used by 0x246340. */
struct SoloEntry52 {
    int active;
    unsigned char id[44];
    int result;
};
struct SoloEntry52 gSh_Face[9];
int gSh_pbVif1[4];
int ShFace_Draw_Pipe(int cursor);
static int solo_slot_log[24], solo_slot_count;
int SzText_LoadRequest(int group, int id)
{
    assert(group == 7);
    solo_slot_log[solo_slot_count++] = 0x100 + id;
    return id & 1 ? 0x700 + id : 0;
}
int RhBase_GetUserGsAddress(void) { solo_slot_log[solo_slot_count++] = 1; return 0x4000; }
int SzText_InitClutManage(int base, int size)
{
    assert(base == 0x4000 && size == 2);
    solo_slot_log[solo_slot_count++] = 2;
    return 0x40;
}
int func_00202b00_word(int cursor, int a0, int a1, int *out)
{
    assert(cursor == 0x1111 && gSh_pbVif1[0] == 0x1111 && a0 == 7 && a1 == 0x4040 && out == 0);
    solo_slot_log[solo_slot_count++] = 3;
    return 0x2222;
}
int func_0020f290_word(int cursor)
{
    assert(cursor == 0x2222 && gSh_pbVif1[0] == 0x2222);
    solo_slot_log[solo_slot_count++] = 4;
    return 0x3333;
}
int SzText_FreeRequest(int a, int b)
{
    assert(a == 7 && b == -1 && gSh_pbVif1[0] == 0x3333);
    solo_slot_log[solo_slot_count++] = 5;
    return 0;
}
void ShFace_DrawOne(struct SoloEntry52 *entry)
{
    solo_slot_log[solo_slot_count++] = 0x200 + (int)(entry - gSh_Face);
}

/* 0x246340 branch coverage: inactive entry, active entry with zero and non-zero result, bound of 8. */
static void solo_slot_cases(void)
{
    static const int expect[11] = { 0x101, 0x102, 0x107, 1, 2, 3, 4, 5, 0x200, 0x207, 0 };
    int i;

    memset(gSh_Face, 0, sizeof(gSh_Face));
    for (i = 0; i < 9; i++) {
        gSh_Face[i].id[0] = (unsigned char)(i + 1);
        gSh_Face[i].result = 0x55;
    }
    gSh_Face[0].active = 1; /* id 1: non-zero result, callee runs */
    gSh_Face[1].active = 1; /* id 2: zero result, no callee */
    gSh_Face[7].active = 1; /* last entry inside the bound; id 7 below */
    gSh_Face[7].id[0] = 7;
    gSh_Face[8].active = 1; /* outside the bound: untouched */
    solo_slot_count = 0;
    assert(ShFace_Draw_Pipe(0x1111) == 0x3333 && gSh_pbVif1[0] == 0x3333);
    assert(solo_slot_count == 10);
    for (i = 0; i < 10; i++) {
        assert(solo_slot_log[i] == expect[i]);
    }
    assert(gSh_Face[0].result == 0x701 && gSh_Face[1].result == 0 && gSh_Face[7].result == 0x707);
    assert(gSh_Face[2].result == 0x55 && gSh_Face[8].result == 0x55);
}

int SzDuel_GetMapUnitID(int x, int y)
{
    assert(solo_scan_on && x >= 0 && x < 7 && y >= 0 && y < 7);
    solo_scan_calls++;
    return x == solo_scan_x && y == solo_scan_y ? solo_scan_cell : -1;
}
static int sweep_second[2][11], sweep_notes[22][2], sweep_note_count;
static int sweep_enabled(void);
int SzDuel_GetUnitCtlGoalC(int a, int b)
{
    if (sweep_enabled()) {
        assert(a >= 0 && a < 2 && b >= 0 && b < 11);
        return sweep_second[a][b];
    }
    assert(a == solo_scan_args[0] && b == solo_scan_args[1]);
    return solo_scan_a30;
}
int SzEffect_SetBusyFlag2(int packed, int flag)
{
    assert(sweep_note_count < 22);
    sweep_notes[sweep_note_count][0] = packed;
    sweep_notes[sweep_note_count][1] = flag;
    sweep_note_count++;
    return 0;
}
static int sweep_on, sweep_levels[2][11], sweep_log[22][5], sweep_count, sweep_marks;
int SzDuel_GetUnitCtlGoalM(int a, int b)
{
    if (sweep_on) {
        assert(a >= 0 && a < 2 && b >= 0 && b < 11);
        return sweep_levels[a][b];
    }
    assert(a == solo_scan_args[0] && b == solo_scan_args[1]);
    return solo_scan_a90;
}
static int sweep_enabled(void) { return sweep_on; }
void Ta_UnitFade(int side, int pos, int dir, int value, int flags)
{
    assert(sweep_on && sweep_count < 22);
    sweep_log[sweep_count][0] = side;
    sweep_log[sweep_count][1] = pos;
    sweep_log[sweep_count][2] = dir;
    sweep_log[sweep_count][3] = value;
    sweep_log[sweep_count][4] = flags;
    sweep_count++;
}

static int solo_scan(int cell, int cb, int a30, int blocked, int d10, int a90)
{
    int result;

    solo_scan_on = 1;
    solo_scan_x = 6;
    solo_scan_y = 6;
    solo_scan_cell = cell;
    solo_scan_cb = cb;
    solo_scan_a30 = a30;
    solo_scan_blocked = blocked;
    solo_scan_d10 = d10;
    solo_scan_a90 = a90;
    solo_scan_calls = solo_scan_marks = 0;
    solo_scan_args[0] = solo_scan_args[1] = -1;
    result = Ta_IsFinishFade();
    assert(solo_scan_calls == 49);
    if (cell >= 0) {
        assert(solo_scan_args[0] == cell >> 8 && solo_scan_args[1] == (cell & 0xff));
    }
    solo_scan_on = 0;
    return result;
}

/* 0x25fd90 branch coverage. */
static void solo_scan_cases(void)
{
    /* no live cell: both counts are zero */
    assert(solo_scan(-1, 0, 0, 0, 0, 0) == 1 && solo_scan_marks == 0);
    /* the two grid values differ: counted but not handled */
    assert(solo_scan(0x203, 5, 6, 0, 0, 0x80) == 0 && solo_scan_marks == 0);
    /* unblocked with a non-zero low byte: handled, marked only at 128 */
    assert(solo_scan(0x203, 0x80, 0x80, 0, 1, 0) == 1 && solo_scan_marks == 1);
    assert(solo_scan(0x203, 5, 5, 0, 1, 0) == 1 && solo_scan_marks == 0);
    /* low byte 6 takes the same path even when blocked; 5 does not */
    assert(solo_scan(0x206, 0x80, 0x80, 1, 1, 0) == 1 && solo_scan_marks == 1);
    assert(solo_scan(0x205, 0x80, 0x80, 1, 1, 0x80) == 0 && solo_scan_marks == 0);
    /* low byte zero unblocked, and blocked: decided by SzDuel_GetUnitCtlRateM (0 or 128) */
    assert(solo_scan(0x200, 5, 5, 0, 0, 0x80) == 1 && solo_scan_marks == 1);
    assert(solo_scan(0x200, 5, 5, 0, 0, 0x7f) == 1 && solo_scan_marks == 0);
    assert(solo_scan(0x205, 5, 5, 1, 0x80, 0x80) == 1 && solo_scan_marks == 1);
    assert(solo_scan(0x205, 5, 5, 1, 0x80, 0) == 1 && solo_scan_marks == 0);
    assert(solo_scan(0x200, 5, 5, 0, 0x7f, 0x80) == 0 && solo_scan_marks == 0);
}

struct DotrSideCounts data_00317490_sidecounts;

static void solo_move_put(struct DotrSideEntry *e, unsigned key, unsigned value, unsigned kind)
{
    set_entry(e, key, kind, 0);
    e->unknown_04_bits_0_29 = value;
}

static int solo_move_has(struct DotrSideEntry *e, unsigned key, unsigned value)
{
    return e->unknown_00_bits_0_30 == key && e->unknown_04_bits_0_29 == value;
}

/* 0x1c33a0 branch coverage, with the real side-table callees. */
static void solo_transfer_case(int side)
{
    static unsigned char first[854], second[854];
    unsigned char *counts = side ? second : first, *unused = side ? first : second;
    struct DotrSide *from, *to;
    int i;

    fill(0);
    from = selected(side);
    to = selected(side ^ 1);
    memset(first, 0, sizeof(first));
    memset(second, 0, sizeof(second));
    data_00317490_sidecounts.first = first;
    data_00317490_sidecounts.second = second;

    /* id 10: two kind-3 entries move, the kind-1 entry stays; the count stops at 3 when none is left */
    solo_move_put(&from->cards[10].copies[0], 5, 100, 3);
    solo_move_put(&from->cards[10].copies[1], 6, 101, 1);
    solo_move_put(&from->cards[10].copies[2], 7, 102, 3);
    counts[10] = 5;
    unused[10] = 9;
    /* id 20: count 1 moves only the first of two */
    solo_move_put(&from->cards[20].copies[3], 8, 200, 3);
    solo_move_put(&from->cards[20].copies[8], 9, 201, 3);
    counts[20] = 1;
    /* id 30: the other side already holds value 300, so that entry is skipped */
    solo_move_put(&from->cards[30].copies[0], 11, 300, 3);
    solo_move_put(&from->cards[30].copies[1], 12, 301, 3);
    solo_move_put(&to->cards[30].copies[0], 99, 300, 1);
    counts[30] = 2;
    /* id 40: zero count leaves a movable entry alone */
    solo_move_put(&from->cards[40].copies[0], 13, 400, 3);
    /* id 50: the other side is full */
    solo_move_put(&from->cards[50].copies[0], 14, 500, 3);
    for (i = 0; i < 9; i++) {
        solo_move_put(&to->cards[50].copies[i], 60 + i, 600 + i, 1);
    }
    counts[50] = 1;
    /* ids 0 and 853: both ends of the walk */
    solo_move_put(&from->cards[0].copies[8], 15, 700, 3);
    solo_move_put(&from->cards[853].copies[0], 16, 800, 3);
    counts[0] = counts[853] = 1;

    lSzDuel_TradeDoSide(side);

    assert(counts[10] == 3 && unused[10] == 9);
    assert(solo_move_has(&to->cards[10].copies[0], 5, 100) && solo_move_has(&to->cards[10].copies[1], 7, 102));
    assert(solo_move_has(&from->cards[10].copies[0], 0, 0) && solo_move_has(&from->cards[10].copies[2], 0, 0));
    assert(from->cards[10].copies[0].unknown_04_bits_30_31 == 3);
    assert(solo_move_has(&from->cards[10].copies[1], 6, 101) && SzData_GetBagCount(side ^ 1, 10, -1) == 2);
    assert(counts[20] == 0 && solo_move_has(&to->cards[20].copies[0], 8, 200));
    assert(solo_move_has(&from->cards[20].copies[3], 0, 0) && solo_move_has(&from->cards[20].copies[8], 9, 201));
    assert(counts[30] == 1 && solo_move_has(&from->cards[30].copies[0], 11, 300));
    assert(solo_move_has(&from->cards[30].copies[1], 0, 0) && solo_move_has(&to->cards[30].copies[1], 12, 301));
    assert(counts[40] == 0 && solo_move_has(&from->cards[40].copies[0], 13, 400) && SzData_GetBagCount(side ^ 1, 40, -1) == 0);
    assert(counts[50] == 1 && solo_move_has(&from->cards[50].copies[0], 14, 500));
    assert(counts[0] == 0 && solo_move_has(&to->cards[0].copies[0], 15, 700));
    assert(counts[853] == 0 && solo_move_has(&to->cards[853].copies[0], 16, 800));
    fill(0);
}

static int pair_ids[2][16], pair_score, pair_score_args[2], pair_gate, pair_gate_args[3], pair_gate_calls;
static int pair_side, pair_side_calls;
int SzDuel_GetUnitCardNo(int a, int b) { return pair_ids[a][b]; }
int Fusion(int a, int b)
{
    pair_score_args[0] = a;
    pair_score_args[1] = b;
    return pair_score;
}
int Ta_GetHandEnchant2(int a, int b, int c)
{
    pair_gate_args[0] = a;
    pair_gate_args[1] = b;
    pair_gate_args[2] = c;
    pair_gate_calls++;
    return pair_gate;
}
int SzDuel_GetTurnOwner(void)
{
    pair_side_calls++;
    return pair_side;
}

static int pair_case(int kind_a, int kind_b, int out[4])
{
    out[0] = out[1] = out[2] = out[3] = 0x5a5a;
    *(unsigned char *)&entry_fixture[10] = kind_a << 5 | 0x1f;
    *(unsigned char *)&entry_fixture[20] = kind_b << 5 | 0x1f;
    pair_gate_calls = pair_side_calls = 0;
    pair_score_args[0] = pair_score_args[1] = -1;
    return Ta_IsHandHeap(1, 2, 0, 3, out);
}

static int pair_is(const int out[4], int a, int b, int c, int d)
{
    return out[0] == a && out[1] == b && out[2] == c && out[3] == d;
}

static void check_pair_result(void)
{
    struct DotrStateEntry20 saved_a = entry_fixture[10], saved_b = entry_fixture[20];
    int out[4];
    int kind;

    pair_ids[1][2] = 10;
    pair_ids[0][3] = 20;
    pair_side = 0x77;
    /* A missing entry on either side returns -1 without writing. */
    pair_ids[1][2] = -1;
    assert(pair_case(0, 0, out) == -1 && pair_is(out, 0x5a5a, 0x5a5a, 0x5a5a, 0x5a5a));
    pair_ids[1][2] = 10;
    pair_ids[0][3] = 900;
    assert(pair_case(0, 0, out) == -1 && pair_is(out, 0x5a5a, 0x5a5a, 0x5a5a, 0x5a5a));
    pair_ids[0][3] = 20;
    /* Kinds 0/0: the 0x2495a0 result, 999 meaning none. */
    pair_score = 999;
    assert(pair_case(0, 0, out) == -1 && pair_is(out, 2, 0x102, 0x103, 0));
    assert(pair_score_args[0] == 10 && pair_score_args[1] == 20 && pair_gate_calls == 0);
    pair_score = 998;
    assert(pair_case(0, 0, out) == 0 && pair_is(out, 0, 0x102, 0x103, 998));
    /* Kinds 0/2: Ta_GetHandEnchant2(side, pos, other id). */
    pair_gate = -1;
    assert(pair_case(0, 2, out) == -1 && pair_is(out, 2, 0x102, 0x103, 0));
    assert(pair_gate_args[0] == 1 && pair_gate_args[1] == 2 && pair_gate_args[2] == 20 && pair_gate_calls == 1);
    pair_gate = 0xffff;
    assert(pair_case(0, 2, out) == 0 && pair_is(out, 0, 0x102, 0x103, 0xffff) && pair_side_calls == 0);
    pair_gate = 0x10000;
    assert(pair_case(0, 2, out) == 1 && pair_is(out, 1, 0x1020103, 0x77, 20) && pair_side_calls == 1);
    /* Kinds 2/0: Ta_GetHandEnchant2(other side, other pos, first id). */
    pair_gate = -5;
    assert(pair_case(2, 0, out) == -1 && pair_is(out, 2, 0x102, 0x103, 0));
    assert(pair_gate_args[0] == 0 && pair_gate_args[1] == 3 && pair_gate_args[2] == 10 && pair_gate_calls == 1);
    pair_gate = 0x1234;
    assert(pair_case(2, 0, out) == 0 && pair_is(out, 0, 0x102, 0x103, 0x1234) && pair_side_calls == 0);
    pair_gate = 0x21234;
    assert(pair_case(2, 0, out) == 0x10001 && pair_is(out, 1, 0x1020103, 0x77, 10) && pair_side_calls == 1);
    /* Every other kind pair is rejected without calling 0x248fd0. */
    for (kind = 0; kind < 8; kind++) {
        if (kind != 0 && kind != 2) {
            assert(pair_case(0, kind, out) == -1 && pair_is(out, 2, 0x102, 0x103, 0) && pair_gate_calls == 0);
            assert(pair_case(kind, 0, out) == -1 && pair_is(out, 2, 0x102, 0x103, 0) && pair_gate_calls == 0);
        }
        if (kind != 0) {
            assert(pair_case(2, kind, out) == -1 && pair_is(out, 2, 0x102, 0x103, 0) && pair_gate_calls == 0);
        }
    }
    entry_fixture[10] = saved_a;
    entry_fixture[20] = saved_b;
}

struct DotrStateEntry8 *Ta_GetUnitEffect(int a, int b, int n)
{
    assert(n == 2 && a >= 0 && a < 2 && b >= 0 && b < 11);
    return modifier_present[a][b] ? (struct DotrStateEntry8 *)&modifier_entries[a][b] : 0;
}
int GetParCap(int value)
{
    modifier_final = value;
    return value + 1;
}

static void modifier_set(int side, int cell, unsigned kind, int id, int op, int amount)
{
    modifier_present[side][cell] = 1;
    modifier_entries[side][cell].head = kind << 8 | 0xc0 | id;
    modifier_entries[side][cell].u.word = (unsigned)op << 16 | amount;
}

static void check_modifiers(void)
{
    int bit;

    assert(sizeof(struct DotrStateModifier) == 8);
    modifier_on = 1;
    /* A zero start returns 0 without any call. */
    modifier_calls = 0;
    modifier_final = -1;
    assert(Ta_GetLPRevByNaturalEffect(5, 0) == 0 && modifier_calls == 0 && modifier_final == -1);
    /* No entries: side 0 cells 0..10 then side 1 cells 1..10, bits 4, 3, 5. */
    assert(Ta_GetLPRevByNaturalEffect(5, 100) == 101 && modifier_final == 100 && modifier_calls == 63);
    assert(modifier_log[0] == 4 && modifier_log[1] == 3 && modifier_log[2] == 5);
    assert(modifier_log[30] == (10 << 8 | 4) && modifier_log[33] == (1 << 16 | 1 << 8 | 4));
    assert(modifier_log[62] == (1 << 16 | 10 << 8 | 5));
    /* Operations 0..3 in cell order; operation 4 and others are ignored. */
    modifier_set(0, 0, 59, 5, 0, 20);  /* 100 + 20 = 120 */
    modifier_set(0, 10, 59, 5, 1, 30); /* 120 - 30 = 90 */
    modifier_set(1, 1, 59, 5, 2, 3);   /* 90 + 90 * 2 = 270 */
    modifier_set(1, 10, 59, 5, 3, 4);  /* 270 - 270 / 4 = 203 */
    modifier_set(0, 5, 59, 5, 4, 1000);
    assert(Ta_GetLPRevByNaturalEffect(5, 100) == 204 && modifier_final == 203);
    /* Division by a zero amount is skipped. */
    modifier_set(1, 10, 59, 5, 3, 0);
    assert(Ta_GetLPRevByNaturalEffect(5, 100) == 271);
    /* Side 1 cell 0 is not visited. */
    modifier_set(1, 0, 59, 5, 0, 7000);
    assert(Ta_GetLPRevByNaturalEffect(5, 100) == 271);
    /* Kind and id must both match (the kind uses all 24 upper bits). */
    assert(Ta_GetLPRevByNaturalEffect(6, 100) == 101);
    modifier_set(0, 0, 59 | 0x10000, 5, 0, 20);
    assert(Ta_GetLPRevByNaturalEffect(5, 100) == 100 - 30 + 140 + 1);
    modifier_set(0, 0, 59, 5, 0, 20);
    /* Each blocked bit (4, 3, 5) removes the cell; the later bits are then not asked. */
    for (bit = 3; bit <= 5; bit++) {
        modifier_blocked[0][0] = 1 << bit;
        modifier_calls = 0;
        assert(Ta_GetLPRevByNaturalEffect(5, 100) == 100 - 30 + 140 + 1);
        assert(modifier_calls == 63 - (bit == 4 ? 2 : bit == 3 ? 1 : 0));
    }
    modifier_blocked[0][0] = 0;
    modifier_on = 0;
}

/* Local mirrors of the small_misc.h declarations used by 0x24f520. */
struct DotrMiscWords4 {
    int unknown_00[4];
};
int TaUnit_AddUnitEfc(int side, int pos, int x, int y, struct DotrMiscWords4 *out);
static int record_me, record_item, record_cell, record_forward, record_forward_calls, record_a, record_b;
static int record_word_calls, record_word_results[3], record_side_args[2];
int func_00250360_pair(int a, int b) { assert(a == 1 && b >= 0); return record_me; }
int func_001df540_word(int a, int b) { assert(a == 1 && b >= 0); return record_item; }
int SzDuel_GetField(int x, int y) { assert(x == 5 && y == 6); return record_cell; }
int Ta_IsVirus(int a, int b, struct DotrMiscWords4 *out)
{
    assert(a == 1 && b == 2 && out != 0);
    record_forward_calls++;
    return record_forward;
}
int Ta_IsLD_DestObj(int a, int b) { record_side_args[0] = a; assert(b == 0x1f); return record_a; }
int Ta_IsLD_CurseObj(int a, int b) { record_side_args[1] = a; assert(b == 0x1f); return record_b; }
unsigned ConvUnitIDtoFlagList(unsigned word, int high, int position)
{
    if (sweep_on) {
        sweep_marks++;
        return word | 1u << (21 - (high * 11 + position));
    }
    assert(record_word_calls < 3 && word == 0 && high == 1 && position == 2);
    return record_word_results[record_word_calls++];
}

static int record_case(int pos, struct DotrMiscWords4 *out)
{
    int i;
    for (i = 0; i < 4; i++) out->unknown_00[i] = 0x5a5a;
    record_word_calls = record_forward_calls = solo_pair_calls = 0;
    return TaUnit_AddUnitEfc(1, pos, 5, 6, out);
}

static int record_is(const struct DotrMiscWords4 *out, int a, int b, int c, int d)
{
    return out->unknown_00[0] == a && out->unknown_00[1] == b && out->unknown_00[2] == c && out->unknown_00[3] == d;
}

static void check_record(void)
{
    struct DotrMiscWords4 out;
    const int top = (int)0x80000000u;

    record_on = modifier_on = 1;
    solo_any = 0;
    solo_kind = 0x1f;
    record_me = 0;
    record_item = 1;
    record_cell = 8;
    solo_pre = 1;
    record_a = record_b = 1;
    record_gate = 1 << 16 | 1 << 15 | 1 << 13;
    record_word_results[0] = 0x11;
    record_word_results[1] = 0x22;
    /* Both words: {113, a, b, top bit}; the other side is 1 when 0x250360 is 0. */
    assert(record_case(2, &out) == 1 && record_is(&out, 113, 0x11, 0x22, top));
    assert(solo_pre_args[0] == 1 && solo_pre_args[1] == 5 && solo_pre_args[2] == 6);
    assert(record_side_args[0] == 1 && record_side_args[1] == 1 && solo_pair_calls == 2 && solo_pair_args[1] == 1);
    record_me = 3;
    assert(record_case(2, &out) == 1 && solo_pre_args[0] == 0 && record_side_args[0] == 0 && record_side_args[1] == 0);
    assert(solo_pair_args[1] == 0);
    /* No item, or position 0: 0 without further calls. */
    record_item = 0;
    assert(record_case(2, &out) == 0 && record_is(&out, 0x5a5a, 0x5a5a, 0x5a5a, 0x5a5a) && solo_pair_calls == 0);
    record_item = 1;
    assert(record_case(0, &out) == 0 && record_is(&out, 0x5a5a, 0x5a5a, 0x5a5a, 0x5a5a) && solo_pair_calls == 0);
    /* Cell kind 9 forwards to 0x24fd10; only a result of 1 returns early. */
    record_cell = 9;
    record_forward = 1;
    assert(record_case(2, &out) == 1 && record_forward_calls == 1 && solo_pair_calls == 0);
    record_forward = 2;
    assert(record_case(2, &out) == 1 && record_forward_calls == 1 && record_is(&out, 113, 0x11, 0x22, top));
    record_cell = 8;
    /* 0x271a40 zero: 0. */
    solo_pre = 0;
    assert(record_case(2, &out) == 0 && record_forward_calls == 0 && solo_pair_calls == 0);
    solo_pre = 1;
    /* Only a (gate 16), only b (gate 15); a positive word on either side is enough. */
    record_gate = 1 << 16;
    assert(record_case(2, &out) == 1 && record_is(&out, 113, 0x11, 0, top) && solo_pair_calls == 2);
    record_gate = 1 << 15;
    assert(record_case(2, &out) == 1 && record_is(&out, 113, 0, 0x11, top));
    /* The side checks and a non-zero kind (>> 5) block each word; then gate 13 is asked. */
    record_gate = 1 << 16 | 1 << 15;
    record_a = 0;
    assert(record_case(2, &out) == 1 && record_is(&out, 113, 0, 0x11, top));
    record_b = 0;
    assert(record_case(2, &out) == 0 && record_word_calls == 0 && solo_pair_calls == 3 && solo_pair_args[0] == 13);
    record_a = record_b = 1;
    /* Third word: gate 13 and grid bit 4 clear give {113, 0, 0, c | top bit}. */
    record_word_results[0] = 0;
    record_word_results[1] = -4;
    record_word_results[2] = 0x33;
    record_gate = 1 << 16 | 1 << 15 | 1 << 13;
    modifier_blocked[1][2] = 1 << 4; /* stub: a blocked bit makes 0x1e2100 return 0 */
    assert(record_case(2, &out) == 1 && record_is(&out, 113, 0, 0, top | 0x33) && record_word_calls == 3);
    record_word_results[2] = 0;
    assert(record_case(2, &out) == 0 && record_is(&out, 0x5a5a, 0x5a5a, 0x5a5a, 0x5a5a));
    record_word_results[2] = 0x33;
    /* Grid bit 4 set, or gate 13 clear: no third word. */
    modifier_blocked[1][2] = 0;
    assert(record_case(2, &out) == 0 && record_word_calls == 2);
    modifier_blocked[1][2] = 1 << 4;
    record_gate = 1 << 16 | 1 << 15;
    assert(record_case(2, &out) == 0 && record_word_calls == 2);
    modifier_blocked[1][2] = 0;
    /* Kind with bits above 4 set blocks a and b; the low five bits go to the side checks. */
    solo_kind = 0x3f;
    record_word_results[0] = 0x11;
    assert(record_case(2, &out) == 0 && record_word_calls == 0);
    solo_kind = 0;
    record_on = modifier_on = 0;
}

void Ta_AllUnitFade2(int a, unsigned char *owner, int c, int dir, int value, int flags);
static void check_sweep(void)
{
    unsigned char owner[0xd0];
    unsigned word;
    int i;

    sweep_on = 1;
    memset(owner, 0, sizeof(owner));
    memset(sweep_levels, 0, sizeof(sweep_levels));
    /* negative dir: only positive levels are applied and marked; 0 and -1 are not */
    sweep_levels[0][0] = 1;
    sweep_levels[0][10] = 5;
    sweep_levels[0][4] = -1;
    sweep_levels[1][0] = 2;
    sweep_levels[1][10] = 1;
    sweep_count = sweep_marks = 0;
    Ta_AllUnitFade2(77, owner, 88, -1, 6, 7);
    assert(sweep_count == 4 && sweep_marks == 4);
    assert(sweep_log[0][0] == 0 && sweep_log[0][1] == 0);
    assert(sweep_log[1][0] == 0 && sweep_log[1][1] == 10);
    assert(sweep_log[2][0] == 1 && sweep_log[2][1] == 0);
    assert(sweep_log[3][0] == 1 && sweep_log[3][1] == 10);
    for (i = 0; i < 4; i++) assert(sweep_log[i][2] == -1 && sweep_log[i][3] == 6 && sweep_log[i][4] == 7);
    memcpy(&word, owner + 0xcc, sizeof(word));
    assert(word == (1u << 21 | 1u << 11 | 1u << 10 | 1u));
    /* dir zero or positive: the marked positions only, levels not asked, word unchanged */
    memset(sweep_levels, 0, sizeof(sweep_levels));
    word = 1u << 20 | 1u << 11 | 1u << 10 | 1u << 22;
    memcpy(owner + 0xcc, &word, sizeof(word));
    sweep_count = sweep_marks = 0;
    Ta_AllUnitFade2(0, owner, 0, 0, 8, 9);
    assert(sweep_count == 3 && sweep_marks == 0);
    assert(sweep_log[0][0] == 0 && sweep_log[0][1] == 1);
    assert(sweep_log[1][0] == 0 && sweep_log[1][1] == 10);
    assert(sweep_log[2][0] == 1 && sweep_log[2][1] == 0);
    for (i = 0; i < 3; i++) assert(sweep_log[i][2] == 0 && sweep_log[i][3] == 8 && sweep_log[i][4] == 9);
    memcpy(&i, owner + 0xcc, sizeof(i));
    assert((unsigned)i == word);
    /* empty word: nothing applied */
    memset(owner, 0, sizeof(owner));
    sweep_count = 0;
    Ta_AllUnitFade2(0, owner, 0, 3, 8, 9);
    assert(sweep_count == 0);
    sweep_on = 0;
}

void Ta_AllUnitFade(unsigned char *mask, unsigned char *owner, int c, int dir, int flag, int value);
static void check_masked_sweep(void)
{
    unsigned char owner[0xd0], mask[12];
    unsigned word;
    int i;

    sweep_on = 1;
    memset(owner, 0, sizeof(owner));
    memset(mask, 0, sizeof(mask));
    memset(sweep_levels, 0, sizeof(sweep_levels));
    memset(sweep_second, 0, sizeof(sweep_second));
    /* masked positions are only noted: side 0 plain, side 1 as (pos - 11) | 0x100 */
    word = 1u << 21 | 1u << 11 | 1u << 10 | 1u;
    memcpy(mask + 8, &word, sizeof(word));
    for (i = 0; i < 11; i++) sweep_levels[0][i] = sweep_levels[1][i] = sweep_second[0][i] = sweep_second[1][i] = 1;
    sweep_levels[0][1] = 0;
    sweep_second[0][2] = 0;
    sweep_levels[1][1] = 0;
    sweep_second[1][2] = 0;
    sweep_levels[0][3] = sweep_second[1][3] = -1;
    sweep_count = sweep_marks = sweep_note_count = 0;
    Ta_AllUnitFade(mask, owner, 55, -2, 4, 6);
    assert(sweep_note_count == 4);
    assert(sweep_notes[0][0] == 0 && sweep_notes[1][0] == 10 && sweep_notes[2][0] == 0x100 && sweep_notes[3][0] == 0x10a);
    for (i = 0; i < 4; i++) assert(sweep_notes[i][1] == 4);
    /* negative dir: the 18 others need both values positive; 3 fail on each side */
    assert(sweep_count == 12 && sweep_marks == 12);
    assert(sweep_log[0][0] == 0 && sweep_log[0][1] == 4);
    assert(sweep_log[5][0] == 0 && sweep_log[5][1] == 9);
    assert(sweep_log[6][0] == 1 && sweep_log[6][1] == 4);
    assert(sweep_log[11][0] == 1 && sweep_log[11][1] == 9);
    for (i = 0; i < 12; i++) assert(sweep_log[i][2] == -2 && sweep_log[i][3] == 6 && sweep_log[i][4] == 1);
    memcpy(&word, owner + 0xcc, sizeof(word));
    assert(word == (0x3fu << 12 | 0x3fu << 1));
    /* non-negative dir: owner bits outside the mask are applied, masked ones noted, word kept */
    word = 1u << 21 | 1u << 20 | 1u << 10 | 1u << 9;
    memcpy(owner + 0xcc, &word, sizeof(word));
    sweep_count = sweep_marks = sweep_note_count = 0;
    Ta_AllUnitFade(mask, owner, 0, 0, 3, 8);
    assert(sweep_note_count == 4 && sweep_notes[0][1] == 3 && sweep_marks == 0);
    assert(sweep_count == 2);
    assert(sweep_log[0][0] == 0 && sweep_log[0][1] == 1 && sweep_log[0][2] == 0 && sweep_log[0][3] == 8 && sweep_log[0][4] == 1);
    assert(sweep_log[1][0] == 1 && sweep_log[1][1] == 1 && sweep_log[1][2] == 0 && sweep_log[1][3] == 8 && sweep_log[1][4] == 1);
    memcpy(&i, owner + 0xcc, sizeof(i));
    assert((unsigned)i == word);
    sweep_on = 0;
}

int TaLogic002_IsActiveMode(void);
static int id_case(int word_index, unsigned short word, int id)
{
    int i;

    for (i = 0; i < 22; i++) fill_words[i] = 0xffff;
    memset(pair_ids, 0, sizeof(pair_ids));
    if (word_index >= 0) {
        fill_words[word_index] = word;
        pair_ids[word >> 8][word & 0xff] = id;
    }
    return TaLogic002_IsActiveMode();
}

static void check_full_list(void)
{
    int side;
    int i;
    for (side = 0; side < 2; side++) {
        struct DotrSideList *q = DOTR_SIDE_LIST(DOTR_SIDE(side), 1);
        for (i = 0; i < 40; i++) {
            q->cards[i] = (unsigned short)(i + 1);
        }
        q->leader = 998;
        assert(lSzDuel_IsDeckReady(side, 1) == 1);
        q->leader = 999;
        assert(lSzDuel_IsDeckReady(side, 1) == 0);
        q->leader = 1000;
        assert(lSzDuel_IsDeckReady(side, 1) == 1);
        q->cards[39] = 999;
        assert(lSzDuel_IsDeckReady(side, 1) == 0);
    }
    assert(lSzDuel_IsDeckReady(-1, 1) == 0 && lSzDuel_IsDeckReady(2, 1) == 0);
}

static void check_id_scan(void)
{
    int saved_front[5], saved_side = pair_side;
    int saved_ids[2][16];

    memcpy(saved_front, solo_front, sizeof(saved_front));
    memcpy(saved_ids, pair_ids, sizeof(saved_ids));
    tail_on = 1;
    pair_side = 1;
    tail_count = 0;
    memset(solo_front, 0, sizeof(solo_front));
    /* nothing anywhere: 1 */
    assert(id_case(-1, 0, 0) == 1);
    /* a position id of 848 or 798 (low 16 bits) answers 0; neighbours do not */
    assert(id_case(0, 0x003, 848) == 0);
    assert(id_case(21, 0x10f, 0x10000 | 798) == 0);
    assert(id_case(5, 0x003, 849) == 1);
    assert(id_case(5, 0x003, 797) == 1);
    /* the five front words are tested on their low 12 bits */
    solo_front[4] = 0x1350;
    assert(id_case(-1, 0, 0) == 0);
    solo_front[4] = 0x351;
    assert(id_case(-1, 0, 0) == 1);
    solo_front[4] = 0;
    solo_front[0] = 798;
    assert(id_case(-1, 0, 0) == 0);
    /* 402, 403 and 422 at any position win over a later 848/798; 401, 404 and 421 do not */
    assert(id_case(0, 0x003, 402) == 1);
    assert(id_case(21, 0x10f, 0x10000 | 403) == 1);
    assert(id_case(7, 0x100, 422) == 1);
    assert(id_case(7, 0x100, 401) == 0);
    assert(id_case(7, 0x100, 404) == 0);
    assert(id_case(7, 0x100, 421) == 0);
    /* the list is scanned up to its count, low 16 bits */
    solo_front[0] = 0;
    tail_ids[0] = 7;
    tail_ids[1] = 798;
    tail_count = 2;
    assert(id_case(-1, 0, 0) == 0);
    tail_ids[1] = 0x10000 | 848;
    assert(id_case(-1, 0, 0) == 0);
    tail_ids[1] = 847;
    assert(id_case(-1, 0, 0) == 1);
    tail_ids[1] = 848;
    tail_count = 1;
    assert(id_case(-1, 0, 0) == 1);
    tail_on = 0;
    pair_side = saved_side;
    memcpy(solo_front, saved_front, sizeof(saved_front));
    memcpy(pair_ids, saved_ids, sizeof(saved_ids));
}

/* TaTuto_GetCardNameMsgID on the real 0x214d00 with the 0x1de540 stub: one assertion per
 * case, per neighbouring unlisted id and for the clear-bit exit. */
static void check_id_swap(int side)
{
    static const int ids[19] = {2551, 3003, 3004, 3005, 2835, 2843, 2849, 2852, 2862, 2863,
                                2865, 2877, 2882, 2884, 2905, 2910, 2912, 2951, 2954};
    static const int others[10] = {0, 27, 2524, 2550, 2552, 3002, 3006, 2834, 2953, 2955};
    struct DotrSide *p = selected(side);
    int saved_side = pair_side, saved_bit = p->flags.unknown_bit_2;
    int i;

    pair_side = side;
    pair_side_calls = 0;
    p->flags.unknown_bit_2 = 0;
    for (i = 0; i < 19; i++) {
        assert(TaTuto_GetCardNameMsgID(ids[i]) == ids[i]);
    }
    assert(pair_side_calls == 19);
    p->flags.unknown_bit_2 = 1;
    for (i = 0; i < 19; i++) {
        assert(TaTuto_GetCardNameMsgID(ids[i]) == 3054 + i);
    }
    for (i = 0; i < 10; i++) {
        assert(TaTuto_GetCardNameMsgID(others[i]) == others[i]);
    }
    assert(pair_side_calls == 48);
    p->flags.unknown_bit_2 = saved_bit;
    pair_side = saved_side;
}

/* Ta_GetRecycleRequestLvl on the real 0x2159d0/0x215d30 with the 0x19a5c8 stub: one
 * assertion per branch outcome and per side of the 90 bound. */
static int roll_case(int side, int id0, int level0, int id1, int level1, int r0, int r1)
{
    struct DotrSide *p = selected(side);

    fill(0);
    DOTR_SIDE_LIST(p, 0)->leader = (unsigned short)id0;
    DOTR_SIDE_LIST(p, 1)->leader = (unsigned short)id1;
    p->cards[5].rank = (unsigned short)level0;
    p->cards[7].rank = (unsigned short)level1;
    random_values[0] = r0;
    random_values[1] = r1;
    random_calls = 0;
    random_count = 2;
    return Ta_GetRecycleRequestLvl(side, 1000);
}

static void check_roll(int side)
{
    /* no usable id: best stays 0, so the sum is r0 % 100 - 2 */
    assert(roll_case(side, 999, 0, 999, 0, 392, 47) == 1008 && random_calls == 2);
    assert(roll_case(side, 999, 0, 999, 0, 391, 47) == 990 && random_calls == 2);
    /* second list larger */
    assert(roll_case(side, 5, 3, 7, 9, 174, 9) == 1010 && random_calls == 2);
    assert(roll_case(side, 5, 3, 7, 9, 173, 24) == 1001 && random_calls == 2);
    /* second list not larger: the first value is kept */
    assert(roll_case(side, 5, 9, 7, 9, 174, 0) == 1001 && random_calls == 2);
    assert(roll_case(side, 5, 9, 7, 3, 173, 11) == 990 && random_calls == 2);
    /* one list skipped */
    assert(roll_case(side, 999, 0, 7, 9, 74, 3) == 1004 && random_calls == 2);
    assert(roll_case(side, 5, 9, 999, 0, 74, 3) == 1004 && random_calls == 2);
    fill(0);
}

static void fever_flag(int id, int on)
{
    ((unsigned char *)&entry_fixture[id])[11] = on ? 0x40 : 0xbf;
}

static void check_fever_gain(void)
{
    static const int fixed[8][2] = {{61, 733}, {722, 811}, {539, 700}, {272, 702},
                                    {716, 715}, {754, 699}, {837, 83}, {60, 87}};
    int i;

    memset(entry_fixture, 0, sizeof(entry_fixture));
    random_count = 1;
    /* each fixed card number, without a random call */
    for (i = 0; i < 8; i++) {
        random_calls = 0;
        assert(TaSlot_GetFeverGain(fixed[i][0]) == fixed[i][1] && random_calls == 0);
    }
    /* no record has the flag (every other bit of the byte set): 999, no random call */
    for (i = 0; i < 854; i++) {
        fever_flag(i, 0);
    }
    random_calls = 0;
    assert(TaSlot_GetFeverGain(62) == 999 && random_calls == 0);
    /* only the last record: one candidate */
    fever_flag(853, 1);
    random_values[0] = 12345;
    assert(TaSlot_GetFeverGain(62) == 853 && random_calls == 1);
    /* all flagged: the eight answers are left out of the list */
    for (i = 0; i < 854; i++) {
        fever_flag(i, 1);
    }
    random_calls = 0;
    random_values[0] = 82;
    assert(TaSlot_GetFeverGain(0) == 82);
    random_calls = 0;
    random_values[0] = 83;
    assert(TaSlot_GetFeverGain(0) == 84);
    random_calls = 0;
    random_values[0] = 85;
    assert(TaSlot_GetFeverGain(0) == 86);
    random_calls = 0;
    random_values[0] = 86;
    assert(TaSlot_GetFeverGain(0) == 88);
    random_calls = 0;
    random_values[0] = 846 + 845;
    assert(TaSlot_GetFeverGain(0) == 853);
    /* a missing record is skipped */
    entry_missing = 40;
    random_calls = 0;
    random_values[0] = 40;
    assert(TaSlot_GetFeverGain(0) == 41);
    random_calls = 0;
    random_values[0] = 845 + 844;
    assert(TaSlot_GetFeverGain(0) == 853 && random_calls == 1);
    entry_missing = -1;
    memset(entry_fixture, 0, sizeof(entry_fixture));
}

static void recycle_entry(int id, int cost, int flag)
{
    ((unsigned char *)&entry_fixture[id])[3] = (unsigned char)cost;
    ((unsigned char *)&entry_fixture[id])[9] = flag ? 0x80 : 0x7f;
}

/* r1 = 0 makes Ta_GetRecycleRequestLvl answer base - r2 % 12 + 1 (both leaders 999). */
static int recycle_case(int side, int cNo, int r0, int r2, int r3)
{
    random_values[0] = r0;
    random_values[1] = 0;
    random_values[2] = r2;
    random_values[3] = r3;
    random_calls = 0;
    random_count = 4;
    return Ta_GetRecycle(side, cNo);
}

static void check_recycle(int side)
{
    unsigned char *own = (unsigned char *)&entry_fixture[10];

    (void)roll_case(side, 999, 0, 999, 0, 0, 0);
    memset(entry_fixture, 0, sizeof(entry_fixture));
    recycle_entry(10, 3, 1);
    recycle_entry(20, 4, 1);
    recycle_entry(682, 4, 1);
    recycle_entry(683, 4, 1);
    recycle_entry(853, 4, 1);
    recycle_entry(30, 4, 0);
    /* kind 0: a draw below 8 takes the range 0..682, level 3 - 0 + 1 = 4 */
    assert(recycle_case(side, 10, 7, 0, 0) == 20 && random_calls == 4);
    assert(recycle_case(side, 10, 17, 0, 3) == 682 && random_calls == 4);
    /* 8 and above: 683..853 */
    assert(recycle_case(side, 10, 8, 0, 0) == 683 && random_calls == 4);
    assert(recycle_case(side, 10, 8, 0, 1) == 853 && random_calls == 4);
    /* another kind: below 4 takes 683..853, 4 and above 0..682; lower five bits ignored */
    own[0] = 0x20;
    assert(recycle_case(side, 10, 3, 0, 0) == 683);
    assert(recycle_case(side, 10, 4, 0, 0) == 20);
    own[0] = 0x1f;
    assert(recycle_case(side, 10, 7, 0, 0) == 20);
    assert(recycle_case(side, 10, 8, 0, 0) == 683);
    /* cNo itself is left out and a missing record skipped: level 5 finds nothing, 4 does */
    recycle_entry(10, 5, 1);
    entry_missing = 20;
    assert(recycle_case(side, 10, 7, 1, 0) == 682 && random_calls == 4);
    entry_missing = -1;
    /* nothing down to level 1: 820 without the last draw */
    assert(recycle_case(side, 10, 7, 3, 0) == 820 && random_calls == 3);
    recycle_entry(40, 1, 1);
    assert(recycle_case(side, 10, 7, 3, 0) == 40 && random_calls == 4);
    /* level 0 or less from the start: no search */
    recycle_entry(10, 0, 1);
    recycle_entry(40, 0, 1);
    assert(recycle_case(side, 10, 7, 1, 0) == 820 && random_calls == 3);
    assert(recycle_case(side, 10, 7, 2, 0) == 820 && random_calls == 3);
    memset(entry_fixture, 0, sizeof(entry_fixture));
    fill(0);
}

int playerNameOffset;
unsigned short StringBlob[16];
unsigned short gSzData_ConstDeck[51][41];
static int const_free_log[8], const_free_count;

int SzText_Free(int group, int id)
{
    const_free_log[const_free_count++] = group * 16 + (id & 15);
    return 0;
}

/* Branch coverage. The effects of the real callees are compared with the same
 * calls made by hand on the same side. */
static void check_const_deck(int side, int deck, int draws)
{
    static struct DotrSide got;
    struct DotrSide *p = &gSzData_All_sides[gSzData_bRoseFlag[0] == side ? 0 : 1];
    unsigned short *tbl = gSzData_ConstDeck[deck];
    int i;

    for (i = 0; i < 41; i++) {
        tbl[i] = (unsigned short)(((i * 7 + deck) % 600 + 1) | (i == 0 ? 0x3000 : 0));
    }
    playerNameOffset = 3;
    StringBlob[5] = 0x2000 | 100;
    StringBlob[6] = 0x4000 | 20;
    StringBlob[7] = 0x8000 | 3;
    StringBlob[8] = 0x1fff;
    random_calls = 0;
    random_count = draws;
    random_values[0] = draws == 2 ? -123 : 1000;
    random_values[1] = (int)0x80000000 + 77;
    const_free_count = 0;
    memset(p, 0x5a, sizeof(*p));
    SzData_SetupFromConstDeck(side, deck);
    assert(random_calls == draws && const_free_count == 3);
    assert(const_free_log[0] == 1 && const_free_log[1] == 0 && const_free_log[2] == 16 + 15);
    if (deck >= 17) {
        assert(p->unknown_ff98 == deck + 0x40000000);
    } else {
        assert(p->unknown_ff98 == (draws == 2 ? 123 + 77 : 1123));
    }
    for (i = 0; i < 5; i++) {
        assert(p->newcomers[i] == 999);
    }
    got = *p;
    memset(p, 0x5a, sizeof(*p));
    SzData_ClearData(side);
    p->unknown_ff98 = got.unknown_ff98;
    for (i = 0; i < 41; i++) {
        SzData_AddBag(side, 0, tbl[i] & 0xfff);
    }
    SzData_SetRankDirect(side, tbl[0] & 0xfff, 3);
    SzData_PushDeckLeader(side, 0, tbl[0] & 0xfff);
    for (i = 1; i < 41; i++) {
        SzData_PushDeck(side, 0, tbl[i] & 0xfff);
    }
    assert(memcmp(&got, p, sizeof(got)) == 0);
}

static void check_const_deck_range(void)
{
    static struct DotrSide before[2];

    memset(gSzData_All_sides, 0x33, sizeof(gSzData_All_sides));
    memcpy(before, gSzData_All_sides, sizeof(before));
    random_calls = 0;
    random_count = 0;
    const_free_count = 0;
    SzData_SetupFromConstDeck(gSzData_bRoseFlag[0], -1);
    SzData_SetupFromConstDeck(gSzData_bRoseFlag[0], 51);
    SzData_SetupFromConstDeck(gSzData_bRoseFlag[0] + 1, 51);
    assert(const_free_count == 0 && memcmp(before, gSzData_All_sides, sizeof(before)) == 0);
    check_const_deck(gSzData_bRoseFlag[0], 0, 1);
    check_const_deck(gSzData_bRoseFlag[0] + 1, 16, 2);
    check_const_deck(gSzData_bRoseFlag[0], 17, 0);
    check_const_deck(gSzData_bRoseFlag[0] + 1, 50, 0);
}

int main(void)
{
    check_id_swap(0);
    check_id_swap(1);
    solo_transfer_case(0);
    solo_transfer_case(1);
    solo_scan_cases();
    solo_slot_cases();
    solo_arrow_cases();
    solo_mode_cases();
    {
        struct DotrSidePoint point = { 1.5f, -2.0f };
        solo_fade_case(&point, 1, 2);
        solo_fade_case(&point, 0, 0);
        solo_fade_case(0, 1, 0);
        solo_fade_case(0, 0, 2);
    }
    solo_collect_cases();
    solo_grid_case(3, 5, 1, 1, 128);
    solo_grid_case(3, 0, 0, 1, 128);
    solo_grid_case(3, 5, 0, 0, 0);
    check_solo_homed();
    check_solo_homed_2();
    check_solo_homed_3();
    check_solo_homed_4();
    check_solo_homed_5();
    check_solo_homed_6();
    check_solo_homed_7();
    check_solo_homed_8();
    check_solo_homed_9();
    check_solo_homed_10();
    static const unsigned char current[] = {0, 1, 7};
    size_t c;
    int side;
    check_layout();
    check_placement_lookup();
    for (c = 0; c < sizeof(current) / sizeof(current[0]); c++) {
        gSzData_bRoseFlag[0] = current[c];
        for (side = 0; side < 2; side++) {
            check_bits(side);
            check_lists(side);
            check_entries(side);
            check_records(side);
            check_recent_and_flags(side);
            check_moves(side);
            check_add(side);
            check_reset(side, 0xff);
            check_reset(side, 0);
            check_reset(side, 0x5a);
        }
    }
    check_reset_both();
    check_pair_result();
    check_modifiers();
    check_record();
    check_sweep();
    check_masked_sweep();
    check_id_scan();
    check_full_list();
    check_roll(0);
    check_roll(1);
    check_fever_gain();
    check_recycle(0);
    check_recycle(1);
    check_const_deck_range();
    puts("side_table: ok");
    return 0;
}

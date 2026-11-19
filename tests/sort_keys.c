/* Synthetic host tests for the sort_keys group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/owner_draws.h"
#include "dotr/ee/sort_keys.h"

unsigned short *data_00317b6c;
static struct DotrSortEntry entries[4];
static int sort_calls, sort_count, sort_size, rank_calls;
static void *sort_base;
unsigned char DeckBuildingSide;
static int count_calls, count_side, count_last_id;
static DotrSortCompare sort_compare;

/* ids 1..4 have entries; every other id has none */
struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int id)
{
    return id >= 1 && id <= 4 ? (struct DotrStateEntry20 *)&entries[id - 1] : 0;
}

int SzDuel_Kind2Attr(int value, int fallback)
{
    rank_calls++;
    return value * 16 + fallback;
}

/* the count is the id's low three bits */
int SzData_GetRank(int side, int id)
{
    count_calls++;
    count_side = side;
    count_last_id = id;
    return id & 7;
}

void func_00199bc0(void *base, int count, int size, DotrSortCompare compare)
{
    sort_calls++;
    sort_base = base;
    sort_count = count;
    sort_size = size;
    sort_compare = compare;
}

struct DotrOwnerDrawControl data_00317490;
#define sort_block ((struct DotrSortBlock *)&data_00317490)
static int flag_calls, flag_side;
static int table_calls[4], table_result[4];
static unsigned short *table_a, *table_b;

/* the answer is the id's low bit */
int SzData_IsBagNew(int side, int id)
{
    flag_calls++;
    flag_side = side;
    return id & 1;
}

static int table_call(int n, unsigned short *a, unsigned short *b)
{
    table_calls[n]++;
    table_a = a;
    table_b = b;
    return table_result[n];
}
static int table_0(unsigned short *a, unsigned short *b) { return table_call(0, a, b); }
static int table_1(unsigned short *a, unsigned short *b) { return table_call(1, a, b); }
static int table_2(unsigned short *a, unsigned short *b) { return table_call(2, a, b); }
static int table_3(unsigned short *a, unsigned short *b) { return table_call(3, a, b); }
DotrSortCompare data_00291330[4] = { table_0, table_1, table_2, table_3 };

/* One assertion per outcome: a's key larger, smaller, equal, and a missing entry
 * on each side and on both. `missing_last` is 1 when a missing entry has the
 * largest key (ascending comparators), -1 when it has the smallest (descending,
 * which also reverses the answers). Entries 1 and 3 hold the small key, 2 the large. */
static void check_compare(DotrSortCompare compare, int direction)
{
    unsigned short small = 1, large = 2, same = 3, none = 900, none_too = 901;

    assert(compare(&large, &small) == direction && compare(&small, &large) == -direction);
    assert(compare(&small, &same) == 0 && compare(&small, &small) == 0);
    assert(compare(&none, &large) == 1 && compare(&large, &none) == -1);
    assert(compare(&none, &none_too) == 0);
}

static void check_comparators(void)
{
    unsigned short ids[3] = { 5, 9, 5 };

    memset(entries, 0xff, sizeof(entries));
    entries[0].kind = 3; entries[1].kind = 200; entries[2].kind = 3;
    check_compare(lSzDuel_ClassCmp, 1);
    memset(entries, 0xff, sizeof(entries));
    entries[0].level = 2; entries[1].level = 15; entries[2].level = 2;
    check_compare(lSzDuel_LevelCmp, 1);
    memset(entries, 0xff, sizeof(entries));
    entries[0].deckCost = 0; entries[1].deckCost = 255; entries[2].deckCost = 0;
    check_compare(lSzDuel_DeckCostCmp, 1);
    /* descending: bits above the 13-bit field are ignored (0xe005 has key 5) */
    memset(entries, 0, sizeof(entries));
    entries[0].apWithFlags = 0xe005; entries[1].apWithFlags = 0x1fff; entries[2].apWithFlags = 0x0005;
    check_compare(lSzDuel_APCmp, -1);
    memset(entries, 0, sizeof(entries));
    entries[0].dpWithFlags = 0xe005; entries[1].dpWithFlags = 0x1fff; entries[2].dpWithFlags = 0x0005;
    check_compare(lSzDuel_DPCmp, -1);
    /* 0x1baaa0: keys 1 * 16 + 2 and 7 * 16 + 15 through the stub; no call for a missing entry */
    memset(entries, 0, sizeof(entries));
    entries[0].kind = 0x3f; entries[0].attr = 2; entries[0].level = 15;
    entries[1].kind = 0xe0; entries[1].attr = 15;
    entries[2].kind = 0x20; entries[2].attr = 2;
    check_compare(lSzDuel_AttrCmp, 1);
    rank_calls = 0;
    ids[0] = 900; ids[1] = 1;
    assert(lSzDuel_AttrCmp(&ids[0], &ids[1]) == 1 && rank_calls == 1);
    /* 0x1bae40: the ids themselves */
    ids[0] = 5; ids[1] = 9;
    assert(lSzDuel_NumberCmp(&ids[1], &ids[0]) == 1 && lSzDuel_NumberCmp(&ids[0], &ids[1]) == -1);
    assert(lSzDuel_NumberCmp(&ids[0], &ids[2]) == 0);
    ids[0] = 0xffff; ids[1] = 0;
    assert(lSzDuel_NumberCmp(&ids[0], &ids[1]) == 1);
}

/* 0x1bac90: each outcome, the 682/683 bound on each side, and the forwarded side. */
static void check_count_compare(void)
{
    unsigned short small = 9, large = 14, same = 17, last = 682, none = 683, none_too = 0xffff;

    DeckBuildingSide = 0x81;
    count_calls = 0;
    assert(lSzDuel_RankCmp(&large, &small) == 1 && lSzDuel_RankCmp(&small, &large) == -1);
    assert(lSzDuel_RankCmp(&small, &same) == 0);
    assert(count_calls == 6 && count_side == 0x81 && count_last_id == 17);
    count_calls = 0;
    assert(lSzDuel_RankCmp(&none, &last) == 1 && count_calls == 1 && count_last_id == 682);
    assert(lSzDuel_RankCmp(&last, &none) == -1 && count_calls == 2);
    assert(lSzDuel_RankCmp(&none, &none_too) == 0 && count_calls == 2);
}

/* 0x1bad40: top-bits key with the +6 step: zero key stays 0 only when the halfword
 * +4 is not 0xffff. */
static void check_group_compare(void)
{
    unsigned short ids[4] = { 1, 2, 3, 4 }, none = 900, none_too = 901;

    memset(entries, 0, sizeof(entries));
    entries[0].kind = 0x1f;                              /* key 0 */
    entries[1].kind = 0x1f; entries[1].effectId = 0xffff; /* key 6 */
    entries[2].kind = 0x20;                              /* key 7 */
    entries[3].kind = 0x3f; entries[3].effectId = 0xffff; /* key 7 */
    assert(lSzDuel_CardColorCmp(&ids[1], &ids[0]) == 1 && lSzDuel_CardColorCmp(&ids[0], &ids[1]) == -1);
    assert(lSzDuel_CardColorCmp(&ids[2], &ids[1]) == 1 && lSzDuel_CardColorCmp(&ids[1], &ids[2]) == -1);
    assert(lSzDuel_CardColorCmp(&ids[2], &ids[3]) == 0 && lSzDuel_CardColorCmp(&ids[0], &ids[0]) == 0);
    entries[3].kind = 0xe0;                              /* key 13 */
    assert(lSzDuel_CardColorCmp(&none, &ids[3]) == 1 && lSzDuel_CardColorCmp(&ids[3], &none) == -1);
    assert(lSzDuel_CardColorCmp(&none, &none_too) == 0);
}

/* 0x1ba630 / 0x1ba710: all keys answering 0, a stop at each of the three positions,
 * and a repeated key skipped at position 1 (equal to 0) and 2 (equal to 0 or 1). */
static void check_key_list(DotrSortCompare compare, unsigned char *keys, unsigned char *other)
{
    unsigned short a = 1, b = 2;
    int stop;

    memset(other, 0, 3);
    keys[0] = 3; keys[1] = 1; keys[2] = 2;
    memset(table_calls, 0, sizeof(table_calls)); memset(table_result, 0, sizeof(table_result));
    assert(compare(&a, &b) == 0 && table_a == &a && table_b == &b);
    assert(table_calls[0] == 0 && table_calls[1] == 1 && table_calls[2] == 1 && table_calls[3] == 1);
    for (stop = 0; stop < 3; stop++) {
        memset(table_calls, 0, sizeof(table_calls)); memset(table_result, 0, sizeof(table_result));
        table_result[keys[stop]] = -1;
        assert(compare(&a, &b) == -1);
        assert(table_calls[1] + table_calls[2] + table_calls[3] == stop + 1 && table_calls[keys[stop]] == 1);
    }
    memset(table_calls, 0, sizeof(table_calls)); memset(table_result, 0, sizeof(table_result));
    keys[0] = 2; keys[1] = 2; keys[2] = 1;
    assert(compare(&a, &b) == 0 && table_calls[2] == 1 && table_calls[1] == 1);
    memset(table_calls, 0, sizeof(table_calls));
    keys[0] = 2; keys[1] = 1; keys[2] = 2;
    assert(compare(&a, &b) == 0 && table_calls[2] == 1 && table_calls[1] == 1);
    memset(table_calls, 0, sizeof(table_calls));
    keys[0] = 2; keys[1] = 1; keys[2] = 1;
    table_result[1] = 1;
    assert(compare(&a, &b) == 1 && table_calls[2] == 1 && table_calls[1] == 1);
    assert(table_calls[0] == 0);
}

/* 0x1ba7f0: keys 0 (odd id through the stub), 1 (even id) and 2 (id 999, no call). */
static void check_flag_compare(void)
{
    unsigned short yes = 5, no = 6, no_too = 8, none = 999, none_too = 999;

    DeckBuildingSide = 0x82;
    flag_calls = 0;
    assert(lSzDuel_NewComerCmp(&no, &yes) == 1 && lSzDuel_NewComerCmp(&yes, &no) == -1);
    assert(lSzDuel_NewComerCmp(&no, &no_too) == 0 && flag_calls == 6 && flag_side == 0x82);
    flag_calls = 0;
    assert(lSzDuel_NewComerCmp(&none, &no) == 1 && lSzDuel_NewComerCmp(&no, &none) == -1 && flag_calls == 2);
    assert(lSzDuel_NewComerCmp(&none, &none_too) == 0 && flag_calls == 2);
}

/* 0x1ba930: each outcome, the id 999 on each side and both, and the id 998. */
static void check_rank_compare(void)
{
    static unsigned short ranks[1000];
    unsigned short small = 1, large = 2, same = 3, last = 998, none = 999, none_too = 999;

    sort_block->ranks = ranks;
    ranks[1] = 7; ranks[2] = 0xffff; ranks[3] = 7; ranks[998] = 0xffff; ranks[999] = 0;
    assert(lSzDuel_NameCmp(&large, &small) == 1 && lSzDuel_NameCmp(&small, &large) == -1);
    assert(lSzDuel_NameCmp(&small, &same) == 0 && lSzDuel_NameCmp(&large, &last) == 0);
    assert(lSzDuel_NameCmp(&none, &large) == 1 && lSzDuel_NameCmp(&large, &none) == -1);
    assert(lSzDuel_NameCmp(&none, &none_too) == 0);
}

/* 0x1ba610: straight-line; every forwarded argument. */
static void check_sort_call(void)
{
    static unsigned short list[41];

    data_00317b6c = list;
    sort_calls = 0;
    lSzDuel_SortDeck();
    assert(sort_calls == 1 && sort_base == &list[1] && sort_count == 40 && sort_size == 2);
    assert(sort_compare == lSzDuel_SortFunc2);
}

int main(void)
{
    check_comparators();
    check_count_compare();
    check_group_compare();
    check_key_list(lSzDuel_SortFunc1, sort_block->keys_a, sort_block->keys_b);
    check_key_list(lSzDuel_SortFunc2, sort_block->keys_b, sort_block->keys_a);
    check_flag_compare();
    check_rank_compare();
    check_sort_call();
    return 0;
}

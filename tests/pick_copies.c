/* Synthetic host tests for the pick_copies group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/pick_copies.h"

static int ids[5];
static int count;
static int fill_calls, fill_side, count_calls;
static int afford_answers[16], afford_calls, afford_values[16];
static unsigned char kind_by[1024];
/* 0x124040: per side and slot answers of the unit queries */
static int spell_mode, spell_ctrl[2][11], spell_attr[2][11], spell_curse[2][11], spell_card[2][11];

static int pair_by[1024], pair_calls, pair_first[8];
static int add_by[1024], add_calls;

void TaHand_GetNextHand(int index, int *out)
{
    fill_side = index;
    fill_calls++;
    memcpy(out, ids, sizeof(ids));
}

int TaHand_GetNextHandNum(int index)
{
    assert(index == 1 || spell_mode);
    count_calls++;
    return count;
}

int TaHand_IsCardSummon2(int index, int value)
{
    assert(index == 1);
    afford_values[afford_calls] = value;
    return afford_answers[afford_calls++];
}

int TaHand_GetCardKind(int id) { return kind_by[id]; }

static int order_calls, order_side, order_ids[5];

int TaLogic005_Is2Head_ThunderDragon(int side, int *list)
{
    order_side = side;
    order_calls++;
    assert(memcmp(list, ids, sizeof(ids)) == 0);
    memcpy(list, order_ids, sizeof(order_ids));
    return 0;
}

int Ta_GetHandEnchant(int a, int b)
{
    pair_first[pair_calls++] = a;
    return pair_by[b];
}

int TaEnc_GetEncScore(int index)
{
    add_calls++;
    return add_by[index];
}

static int mark_calls, mark_id;
static struct DotrStateBlock *mark_block;

void SzDuel_InitCardInfo(struct DotrStateBlock *block, int unknown)
{
    mark_calls++;
    mark_block = block;
    mark_id = unknown;
}

typedef int (*PickFill)(int, unsigned char *, struct DotrPickTotals *);

static void set_ids(int a, int b, int c, int d, int e)
{
    int v[5] = {a, b, c, d, e};
    memcpy(ids, v, sizeof(v));
}

static int run(PickFill f, unsigned char *item, struct DotrPickTotals *totals)
{
    fill_calls = count_calls = afford_calls = pair_calls = add_calls = 0;
    memset(item, 0x11, 12);
    return f(1, item, totals);
}

/* adds: whether the copy adds the 0x2493a0 values to the totals */
static void check_fill(PickFill f, int adds)
{
    unsigned char item[12];
    unsigned char none[5] = {0xff, 0xff, 0xff, 0xff, 0xff};
    struct DotrPickTotals totals;
    int i;

    memset(&totals, 0, sizeof(totals));
    memset(kind_by, 0, sizeof(kind_by));
    memset(add_by, 0, sizeof(add_by));
    for (i = 0; i < 1024; i++) pair_by[i] = -1;
    count = 5;
    set_ids(20, 21, 22, 23, 24);

    /* no totals object: -1 after the fill and the id read, nothing counted */
    assert(run(f, item, 0) == -1);
    assert(memcmp(item + 6, none, 5) == 0 && item[5] == 0x11 && item[11] == 0x11);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 0);

    /* the slot whose id equals the totals id and is affordable becomes item[6];
       a first-slot kind of 1 (0x20) ends there */
    totals.id_3c = 22;
    totals.total_38 = 5;
    totals.total_3a = 9;
    afford_answers[0] = 1;
    kind_by[22] = 0x20;
    assert(run(f, item, &totals) == 1);
    assert(item[6] == 2 && memcmp(item + 7, none, 4) == 0);
    assert(afford_calls == 1 && afford_values[0] == 22 && count_calls == 6 && pair_calls == 0);
    assert(totals.total_38 == 5 && totals.total_3a == 9);

    /* the answer must be exactly 1, and the last passing slot wins */
    set_ids(22, 21, 22, 22, 24);
    afford_answers[0] = 1; afford_answers[1] = 0; afford_answers[2] = 2;
    assert(run(f, item, &totals) == 1 && item[6] == 0 && afford_calls == 3);
    afford_answers[1] = 1;
    assert(run(f, item, &totals) == 1 && item[6] == 2);
    /* the count bounds the first walk: three slots hide the match at slot 3 */
    afford_answers[2] = 1;
    assert(run(f, item, &totals) == 1 && item[6] == 3);
    count = 3;
    afford_answers[1] = 0;
    assert(run(f, item, &totals) == 1 && item[6] == 0 && afford_calls == 2 && count_calls == 4);
    count = 5;

    /* first-slot kind 0 (0x1f): slots other than item[6] with a non-negative pair
       answer and kind 2 are appended and their 0x2493a0 value added to both totals */
    set_ids(20, 21, 22, 23, 24);
    afford_answers[0] = 1;
    kind_by[22] = 0x1f;
    pair_by[20] = 0; pair_by[21] = -1; pair_by[22] = 0; pair_by[23] = 7; pair_by[24] = 7;
    kind_by[20] = 0x40; kind_by[21] = 0x40; kind_by[23] = 0x5f; kind_by[24] = 0x60;
    add_by[20] = 100; add_by[23] = 0x1fff9;
    assert(run(f, item, &totals) == 1);
    assert(item[6] == 2 && item[7] == 0 && item[8] == 3 && item[9] == 0xff && item[10] == 0xff && item[11] == 0x11);
    assert(pair_calls == 4 && pair_first[0] == 22 && pair_first[3] == 22 && add_calls == (adds ? 4 : 0) && count_calls == 12);
    if (adds) {
        assert(totals.total_38 == 5 + 100 - 7 && totals.total_3a == 9 + 100 - 7);
    } else {
        assert(totals.total_38 == 5 && totals.total_3a == 9);
    }
    /* kind 1 (0x3f) is not kind 2 */
    kind_by[20] = 0x3f;
    assert(run(f, item, &totals) == 1 && item[7] == 3 && item[8] == 0xff && add_calls == (adds ? 2 : 0));
    /* ids 795 and 796 are never appended, 794 and 797 are */
    set_ids(794, 795, 22, 796, 797);
    pair_by[794] = pair_by[795] = pair_by[796] = pair_by[797] = 0;
    kind_by[794] = kind_by[795] = kind_by[796] = kind_by[797] = 0x40;
    assert(run(f, item, &totals) == 1);
    assert(item[6] == 2 && item[7] == 0 && item[8] == 4 && item[9] == 0xff && add_calls == (adds ? 4 : 0));
    /* the count bounds the second walk: three slots hide slot 4 */
    count = 3;
    assert(run(f, item, &totals) == 1 && item[7] == 0 && item[8] == 0xff && pair_calls == 2);
    /* all four other slots appended fill item[7..10] and nothing beyond */
    count = 5;
    set_ids(794, 794, 22, 794, 794);
    assert(run(f, item, &totals) == 1);
    assert(item[7] == 0 && item[8] == 1 && item[9] == 3 && item[10] == 4 && item[11] == 0x11);
}

typedef int (*PickFirst)(int, unsigned char *, struct DotrStateBlock *, int);

static int run_first(PickFirst f, unsigned char *item, int id)
{
    fill_calls = count_calls = afford_calls = mark_calls = 0;
    memset(item, 0x11, 12);
    return f(1, item, (struct DotrStateBlock *)item, id);
}

static void check_first(PickFirst f)
{
    unsigned char item[12];
    unsigned char untouched[12];

    memset(untouched, 0x11, sizeof(untouched));
    memset(kind_by, 0, sizeof(kind_by));
    kind_by[22] = 0x5f;
    set_ids(20, 22, 21, 22, 22);

    /* a count of zero: -1 after the id read, nothing stored or forwarded */
    count = 0;
    assert(run_first(f, item, 22) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 1 && afford_calls == 0 && mark_calls == 0);
    assert(memcmp(item, untouched, 12) == 0);

    /* no slot holds the id: -1, the affordability callee is never asked */
    count = 5;
    assert(run_first(f, item, 23) == -1 && afford_calls == 0 && count_calls == 6 && mark_calls == 0);

    /* the first matching slot whose answer is exactly 1 is taken: slot stored in
       item[6], block and id forwarded, kind >> 5 returned */
    afford_answers[0] = 1;
    assert(run_first(f, item, 22) == 2);
    assert(item[6] == 1 && item[5] == 0x11 && item[7] == 0x11);
    assert(afford_calls == 1 && afford_values[0] == 22 && count_calls == 2);
    assert(mark_calls == 1 && mark_id == 22 && mark_block == (struct DotrStateBlock *)item);

    /* answers other than 1 pass over a slot */
    afford_answers[0] = 0; afford_answers[1] = 2; afford_answers[2] = 1;
    assert(run_first(f, item, 22) == 2 && item[6] == 4 && afford_calls == 3 && mark_calls == 1);
    afford_answers[2] = 0;
    assert(run_first(f, item, 22) == -1 && afford_calls == 3 && mark_calls == 0 && item[6] == 0x11);

    /* the count bounds the walk: four slots hide slot 4, two reach slot 1 */
    afford_answers[2] = 1;
    count = 4;
    assert(run_first(f, item, 22) == -1 && afford_calls == 2 && count_calls == 5);
    afford_answers[0] = 1;
    count = 2;
    assert(run_first(f, item, 22) == 2 && item[6] == 1);
    count = 1;
    assert(run_first(f, item, 22) == -1 && afford_calls == 0);

    /* only the low 16 bits of the id take part in the comparison */
    count = 5;
    assert(run_first(f, item, 0x10000 + 22) == 2 && item[6] == 1 && mark_id == 22);
}

/* Stubs for the 0x117bd0 pair: the record's second word is forwarded to the
 * filler, which writes three words the answering callee checks. */
static struct DotrPickPair pair_record;
static int record_index, record_zero, triple_id, answer_value, answer_result;

struct DotrPickPair *TaHand_GetCardEffect(int a, int b)
{
    record_index = a;
    record_zero = b;
    return &pair_record;
}

void Ta_GetCeremSacrifice(int *triple, int id)
{
    triple_id = id;
    triple[0] = id + 1; triple[1] = id + 2; triple[2] = id + 3;
}

int Ta_GetSacrificeUnit(int *triple, int value)
{
    assert(triple[0] == triple_id + 1 && triple[1] == triple_id + 2 && triple[2] == triple_id + 3);
    answer_value = value;
    return answer_result;
}

static void check_triple(int (*f)(int, int))
{
    pair_record.unknown_0 = 0x111; pair_record.unknown_4 = 0x222;
    /* negative answer: 0 */
    answer_result = -1; record_zero = 9;
    assert(f(31, 4) == 0);
    assert(record_index == 4 && record_zero == 0 && triple_id == 0x222 && answer_value == 31);
    /* boundary 0 and a positive answer: 1 */
    answer_result = 0;
    assert(f(31, 4) == 1);
    answer_result = 5;
    assert(f(31, 4) == 1);
}

/* Stubs for the 0x138f90 family: a list of entries per answer word, a pair
 * answer per cell and a score per call. */
static int cellword_walk, cellword_by[4], cellword_calls;
static int list_count, list_word, list_calls, entry_by[4], cell_answer, score_by[4], score_calls;

int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    list_word = (int)value;
    list_calls++;
    return list_count;
}

int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word == list_word && n >= 0 && n < 4);
    return entry_by[n];
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == entry_by[score_calls] >> 8 && b == (entry_by[score_calls] & 0xff));
    return cell_answer;
}

int TaUtil_GetUnitAP(int a, int b, int c, int d)
{
    assert(a == entry_by[score_calls] >> 8 && b == (entry_by[score_calls] & 0xff));
    assert(c == (cell_answer & 0xff) && d == cell_answer >> 8);
    return score_by[score_calls++];
}

static int run_limit(int (*f)(int, int), int n, int s0, int s1, int s2)
{
    list_count = n; list_calls = 0; score_calls = 0; list_word = -1;
    score_by[0] = s0; score_by[1] = s1; score_by[2] = s2;
    return f(31, 4);
}

static void check_limit(int (*f)(int, int), int limit, int is_signed)
{
    pair_record.unknown_0 = 0x111; pair_record.unknown_4 = 0x222;
    entry_by[0] = 0x0305; entry_by[1] = 0x1207; entry_by[2] = 0x00ff;
    cell_answer = 0x0409;
    /* negative answer: 0 before any list call */
    answer_result = -1; record_zero = 9;
    assert(run_limit(f, 3, 0, 0, 0) == 0 && list_calls == 0);
    assert(record_index == 4 && record_zero == 0 && triple_id == 0x222 && answer_value == 31);
    /* answer 0 (boundary), empty list: 1 */
    answer_result = 0;
    assert(run_limit(f, 0, 0, 0, 0) == 1 && list_calls == 1 && list_word == 0 && score_calls == 0);
    /* every score below the limit: 1 after the whole list */
    answer_result = 7;
    assert(run_limit(f, 3, limit - 1, 0, 1) == 1 && score_calls == 3 && list_word == 7);
    /* a negative score is below a constant limit (signed compare) */
    if (is_signed) {
        assert(run_limit(f, 3, -5, 0, 0) == 1 && score_calls == 3);
    }
    /* a score at the limit stops the walk: first, last */
    assert(run_limit(f, 3, limit, 0, 0) == 0 && score_calls == 1);
    assert(run_limit(f, 3, 0, limit - 1, limit) == 0 && score_calls == 3);
    assert(run_limit(f, 3, 0, limit + 1, 0) == 0 && score_calls == 2);
    /* count bound: the entry past the count is not read */
    assert(run_limit(f, 2, 0, 0, limit) == 1 && score_calls == 2);
}

/* 0x15d030: the same walk with limit 0, so only negative scores pass. */
static void check_limit_zero(int (*f)(int, int))
{
    pair_record.unknown_0 = 0x111; pair_record.unknown_4 = 0x222;
    entry_by[0] = 0x0305; entry_by[1] = 0x1207; entry_by[2] = 0x00ff;
    cell_answer = 0x0409;
    /* negative answer: 0 before any list call */
    answer_result = -1; record_zero = 9;
    assert(run_limit(f, 3, -1, -1, -1) == 0 && list_calls == 0);
    assert(record_index == 4 && record_zero == 0 && triple_id == 0x222 && answer_value == 31);
    /* answer 0 (boundary), empty list: 1 */
    answer_result = 0;
    assert(run_limit(f, 0, 0, 0, 0) == 1 && list_calls == 1 && list_word == 0 && score_calls == 0);
    /* every score negative: 1 after the whole list */
    answer_result = 7;
    assert(run_limit(f, 3, -1, -3000, -2) == 1 && score_calls == 3 && list_word == 7);
    /* a score of 0 (boundary) or above stops the walk: first, second, last */
    assert(run_limit(f, 3, 0, -1, -1) == 0 && score_calls == 1);
    assert(run_limit(f, 3, -1, 1, -1) == 0 && score_calls == 2);
    assert(run_limit(f, 3, -1, -1, 0) == 0 && score_calls == 3);
    /* count bound: the entry past the count is not read */
    assert(run_limit(f, 2, -1, -1, 0) == 1 && score_calls == 2);
}

/* 0x10f940: the limit walk, with four cell words that reject before scoring. */
static void check_limit_kinds(int (*f)(int, int))
{
    static const int rejected[] = {402, 403, 427, 428};
    static const int passed[] = {401, 404, 426, 429};
    unsigned i;

    cellword_walk = 1;
    cellword_by[0] = cellword_by[1] = cellword_by[2] = 0;
    cellword_calls = 0;
    answer_result = -1;
    assert(run_limit(f, 3, 0, 0, 0) == 0 && cellword_calls == 0);
    check_limit(f, 3000, 1);
    answer_result = 7;
    for (i = 0; i < 4; i++) {
        /* a rejected word in the second cell: 0 with only the first cell scored */
        cellword_by[1] = rejected[i]; cellword_calls = 0;
        assert(run_limit(f, 3, 0, 0, 0) == 0 && score_calls == 1 && cellword_calls == 2);
        /* the words next to each pair pass */
        cellword_by[1] = passed[i]; cellword_calls = 0;
        assert(run_limit(f, 3, 0, 0, 0) == 1 && score_calls == 3 && cellword_calls == 3);
    }
    cellword_walk = 0;
}

/* 0x112850 pair: the limit comes from the high halfword of the 0x26fbb0 word. */
static int limit_word, limit_id, limit_calls;

int TaHand_GetCardAPDP(int id)
{
    limit_id = id;
    limit_calls++;
    return limit_word;
}

static void check_record_limit(int (*f)(int, int))
{
    limit_word = (2600 << 16) | 0x1234;
    limit_calls = 0;
    check_limit(f, 2600, 0);
    /* one limit read per call, with the record's second word, also when the answer is negative */
    assert(limit_calls == 7 && limit_id == 0x222);
    /* the halfword is taken unsigned and the score compared unsigned */
    limit_word = (int)0x80000000;
    answer_result = 7;
    assert(run_limit(f, 3, 0x7fff, 0, 0) == 1 && score_calls == 3);
    assert(run_limit(f, 3, 0x8000, 0, 0) == 0 && score_calls == 1);
    assert(run_limit(f, 3, -1, 0, 0) == 0 && score_calls == 1);
}

/* 0x15e440 pair: five cells of one row and a two-argument answer per pair. */
static int row_cells[6], row_seen, row_calls, pair_answer_by[8], pair_answer_calls;

static int grid_mode, grid_entry[8], grid_cell[8], grid_null[8], grid_i, grid_guard, grid_word_by[8];
static int grid_veto_a[8], grid_veto_b[8];
static int entry_calls, item_calls, cell_calls, word_calls, key_calls, veto_a_calls, veto_b_calls;

int SzDuel_GetMapUnitID(int x, int y)
{
    if (grid_mode) {
        assert(x == (grid_entry[grid_i] & 0xff) && y == grid_entry[grid_i] >> 8);
        cell_calls++;
        return grid_cell[grid_i];
    }
    assert(x >= 1 && x <= 5 && x == row_calls + 1);
    row_seen = y;
    row_calls++;
    return row_cells[x];
}

int func_00250360_pair(int a, int b)
{
    if (spell_mode) {
        assert(a >= 0 && a < 2 && b >= 0 && b < 11);
        return spell_ctrl[a][b];
    }
    pair_answer_calls++;
    return pair_answer_by[((a << 8) | b) & 7];
}

static int run_row(int (*f)(int, int, unsigned char), int c1, int c2, int c5, int c)
{
    memset(row_cells, 0, sizeof(row_cells));
    row_cells[1] = c1; row_cells[2] = c2; row_cells[5] = c5;
    row_calls = 0; pair_answer_calls = 0; row_seen = -1;
    return f(2, 3, (unsigned char)c);
}

static void check_row(int (*f)(int, int, unsigned char))
{
    int i;

    /* key is (2 << 8) | 3 = 0x203, answer slot 3 */
    for (i = 0; i < 8; i++) {
        pair_answer_by[i] = 10 + i;
    }
    /* row 3 for c == 1 (low byte only), row 4 otherwise; all cells zero: 1 without asking */
    assert(run_row(f, 0, 0, 0, 1) == 1 && row_seen == 3 && row_calls == 5 && pair_answer_calls == 0);
    assert(run_row(f, 0, 0, 0, 0x101) == 1 && row_seen == 3);
    assert(run_row(f, 0, 0, 0, 0) == 1 && row_seen == 4);
    assert(run_row(f, 0, 0, 0, 2) == 1 && row_seen == 4 && row_calls == 5);
    /* a cell equal to the key: 1 at once */
    assert(run_row(f, 0x203, 0x104, 0, 1) == 1 && row_calls == 1 && pair_answer_calls == 0);
    /* a cell with another answer goes on (also a negative cell); last cell bound */
    assert(run_row(f, 0x104, 0, 0, 1) == 1 && row_calls == 5 && pair_answer_calls == 2);
    assert(run_row(f, -1, 0, 0x105, 1) == 1 && row_calls == 5 && pair_answer_calls == 4);
    /* a cell with the same answer as (a, b): 0, first and last place */
    pair_answer_by[4] = 13;
    assert(run_row(f, 0x104, 0x203, 0, 1) == 0 && row_calls == 1 && pair_answer_calls == 2);
    assert(run_row(f, 0, 0x105, 0x104, 1) == 0 && row_calls == 5 && pair_answer_calls == 4);
    /* the key wins over an earlier differing cell */
    assert(run_row(f, 0x105, 0x203, 0x104, 1) == 1 && row_calls == 2);
}

static void check_mask(void (*f)(struct DotrPickMask *, unsigned char))
{
    struct DotrPickMask m;

    m.unknown_0 = 0x11111111; m.half_4 = 0x1234; m.unknown_6 = 0x5678; m.goalLoc1 = 0x9abcdef0;
    /* n >= 2: unchanged */
    f(&m, 2);
    assert(m.half_4 == 0x1234 && m.goalLoc1 == 0x9abcdef0);
    f(&m, 0xff);
    assert(m.half_4 == 0x1234 && m.goalLoc1 == 0x9abcdef0);
    /* n == 1: high parts become ones */
    f(&m, 1);
    assert(m.half_4 == 0xff34 && m.goalLoc1 == 0xffffdef0);
    /* n == 0: low parts become ones */
    m.half_4 = 0x1234; m.goalLoc1 = 0x9abcdef0;
    f(&m, 0);
    assert(m.half_4 == 0x12ff && m.goalLoc1 == 0x9abcffff);
    assert(m.unknown_0 == 0x11111111 && m.unknown_6 == 0x5678);
}

/* 0x11c3e0 pair: only goalLoc1 changes, for n 0 and 1. */
static void check_mask_word(void (*f)(struct DotrPickMask *, unsigned char))
{
    struct DotrPickMask m;

    m.unknown_0 = 0x11111111; m.half_4 = 0x1234; m.unknown_6 = 0x5678; m.goalLoc1 = 0x9abcdef0;
    f(&m, 2);
    assert(m.goalLoc1 == 0x9abcdef0);
    f(&m, 0xff);
    assert(m.goalLoc1 == 0x9abcdef0);
    f(&m, 1);
    assert(m.goalLoc1 == 0xffffdef0);
    m.goalLoc1 = 0x9abcdef0;
    f(&m, 0);
    assert(m.goalLoc1 == 0x9abcffff);
    assert(m.unknown_0 == 0x11111111 && m.half_4 == 0x1234 && m.unknown_6 == 0x5678);
}

/* Stubs for the 0x112a40 pair (side 2). */
int Ta_GetSommonPiecePreference(int a, int i)
{
    assert(a == 2 && i >= 0 && i < 8);
    grid_i = i;
    entry_calls++;
    return grid_entry[i];
}

struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(x == (grid_entry[grid_i] & 0xff) && y == grid_entry[grid_i] >> 8);
    item_calls++;
    return grid_null[grid_i] ? NULL : (struct DotrStateItem *)grid_entry;
}

int SzDuel_GetUnitAttr(int a, int b)
{
    assert(spell_mode && a >= 0 && a < 2 && b >= 0 && b < 6);
    return spell_attr[a][b];
}

int SzDuel_GetUnitCardNo(int a, int b)
{
    if (spell_mode) {
        assert(a >= 0 && a < 2 && b >= 6 && b < 11);
        return spell_card[a][b];
    }
    if (cellword_walk) {
        /* asked for the cell about to be scored */
        assert(a == entry_by[score_calls] >> 8 && b == (entry_by[score_calls] & 0xff));
        cellword_calls++;
        return cellword_by[score_calls];
    }
    if (word_calls++ == 0) {
        assert(a == 2 && b == 0);
        return grid_guard;
    }
    assert(a == grid_cell[grid_i] >> 8 && b == (grid_cell[grid_i] & 0xff));
    return grid_word_by[b];
}

static int grid_bit_flip[5][8], grid_bit_calls[5];

/* Answers bit 4 set and the others clear unless flipped for that low byte. */
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == grid_cell[grid_i] >> 8 && b == (grid_cell[grid_i] & 0xff));
    assert(bit == 4 || bit == 0 || bit == 1);
    grid_bit_calls[bit]++;
    return (bit == 4) ^ grid_bit_flip[bit][b];
}

int Ta_GetExzodiaPartsCardNo(int a)
{
    assert(a >= 0 && a < 4);
    key_calls++;
    return 100 + a;
}

int SzDuel_GetUnitCurseRest(int a, int b)
{
    if (spell_mode) {
        assert(a >= 0 && a < 2 && b >= 0 && b < 11);
        return spell_curse[a][b];
    }
    assert(a == grid_cell[grid_i] >> 8 && b == (grid_cell[grid_i] & 0xff));
    veto_a_calls++;
    return grid_veto_a[b];
}

int SzDuel_GetUnitSpellRest(int a, int b)
{
    assert(a == grid_cell[grid_i] >> 8 && b == (grid_cell[grid_i] & 0xff));
    veto_b_calls++;
    return grid_veto_b[b];
}

static int run_grid(int (*f)(int), int guard)
{
    grid_guard = guard;
    entry_calls = item_calls = cell_calls = word_calls = key_calls = veto_a_calls = veto_b_calls = 0;
    return f(2);
}

/* The four flags are never cleared by the code, so only runs in which every
 * flag is set have a defined sum; the rejecting branches are observed through
 * the callee call counts. */
static void check_grid(int (*f)(int))
{
    int i;

    grid_mode = 1;
    memset(grid_null, 0, sizeof(grid_null));
    memset(grid_veto_a, 0, sizeof(grid_veto_a));
    memset(grid_veto_b, 0, sizeof(grid_veto_b));
    for (i = 0; i < 8; i++) {
        grid_entry[i] = ((i + 1) << 8) | (i + 3);
    }
    /* cells 0..3 set flag 0..3 */
    for (i = 0; i < 4; i++) {
        grid_cell[i] = (2 << 8) | i;
        grid_word_by[i] = 100 + i;
    }
    grid_cell[4] = (2 << 8) | 6; grid_word_by[6] = 102; grid_veto_b[6] = 1; /* second veto */
    grid_cell[5] = -1;                                                       /* negative cell */
    grid_cell[6] = (3 << 8) | 4; grid_word_by[4] = 100;                      /* other side */
    grid_cell[7] = (2 << 8) | 5; grid_word_by[5] = 101; grid_veto_a[5] = 1;  /* first veto */
    /* guard word other than 58: 0, nothing else asked */
    assert(run_grid(f, 57) == 0 && word_calls == 1 && entry_calls == 0);
    assert(run_grid(f, 59) == 0 && word_calls == 1 && entry_calls == 0);
    /* every flag set: 4; eight entries for each of four keys */
    assert(run_grid(f, 58) == 4);
    assert(entry_calls == 32 && item_calls == 32 && cell_calls == 32);
    assert(word_calls == 1 + 28 && key_calls == 28 && veto_a_calls == 7 && veto_b_calls == 6);
    /* a null item skips the cell read */
    grid_null[5] = 1;
    assert(run_grid(f, 58) == 4 && item_calls == 32 && cell_calls == 28 && key_calls == 28);
    /* boundary: cell 0 is taken (non-negative) */
    grid_null[5] = 0;
    grid_cell[5] = 0; grid_word_by[0] = 100;
    assert(run_grid(f, 58) == 4 && key_calls == 32 && veto_a_calls == 8 && veto_b_calls == 7);
    grid_mode = 0;
}

/* 0x268460: the check_grid outcomes, then each of the three bit tests
 * rejecting the redundant cell (low byte 6) before the vetoes are asked. */
static void check_grid_bits(int (*f)(int))
{
    static const int bits[3] = {4, 0, 1};
    static const int want[3][3] = {{8, 7, 7}, {8, 8, 7}, {8, 8, 8}};
    int i;

    memset(grid_bit_flip, 0, sizeof(grid_bit_flip));
    check_grid(f);
    grid_mode = 1;
    memset(grid_bit_calls, 0, sizeof(grid_bit_calls));
    assert(run_grid(f, 58) == 4 && veto_a_calls == 8 && veto_b_calls == 7);
    assert(grid_bit_calls[4] == 8 && grid_bit_calls[0] == 8 && grid_bit_calls[1] == 8);
    for (i = 0; i < 3; i++) {
        grid_bit_flip[bits[i]][6] = 1;
        memset(grid_bit_calls, 0, sizeof(grid_bit_calls));
        assert(run_grid(f, 58) == 4 && key_calls == 32 && veto_a_calls == 7 && veto_b_calls == 6);
        assert(grid_bit_calls[4] == want[i][0] && grid_bit_calls[0] == want[i][1] && grid_bit_calls[1] == want[i][2]);
        grid_bit_flip[bits[i]][6] = 0;
    }
    grid_mode = 0;
}

typedef int (*PickGiven)(int, int *, struct DotrStateBlock *, unsigned char *, int);

static int run_given(PickGiven f, unsigned char *item, int id)
{
    count_calls = afford_calls = mark_calls = fill_calls = 0;
    memset(item, 0x11, 12);
    return f(1, ids, (struct DotrStateBlock *)item, item, id);
}

static void check_given(PickGiven f)
{
    unsigned char item[12];
    int i;

    memset(kind_by, 0, sizeof(kind_by));
    kind_by[22] = 0x5f;
    set_ids(20, 22, 21, 22, 22);
    for (i = 0; i < 16; i++) {
        afford_answers[i] = 0;
    }

    /* a count of zero: -1, nothing asked or stored; the id array is the caller's */
    count = 0;
    assert(run_given(f, item, 22) == -1 && count_calls == 1 && afford_calls == 0 && fill_calls == 0);

    /* first answer not 1: every slot is passed over after one question each */
    count = 5;
    assert(run_given(f, item, 22) == -1 && afford_calls == 5 && count_calls == 6 && item[6] == 0x11);

    /* first answer 1 but another id: one question only for that slot */
    afford_answers[0] = 1;
    assert(run_given(f, item, 22) == -1 && afford_calls == 5 && afford_values[0] == 20);

    /* slot 1: first answer 1, id equal, second answer not 1: passed over */
    afford_answers[1] = 1; afford_answers[2] = 2;
    assert(run_given(f, item, 22) == -1 && afford_calls == 6 && afford_values[2] == 22 && mark_calls == 0);

    /* both answers 1: slot stored, block and id forwarded, kind >> 5 returned */
    afford_answers[2] = 1;
    assert(run_given(f, item, 22) == 2 && item[6] == 1 && item[5] == 0x11 && item[7] == 0x11);
    assert(afford_calls == 3 && count_calls == 2 && mark_calls == 1 && mark_id == 22);
    assert(mark_block == (struct DotrStateBlock *)item);

    /* the count bounds the walk; only the low 16 bits of the id are compared */
    count = 1;
    assert(run_given(f, item, 22) == -1 && afford_calls == 1);
    count = 5;
    assert(run_given(f, item, 0x10000 + 22) == 2 && item[6] == 1);
}

/* 0x114d40 family: nodes, scores and the slot-pick callee are all stubs. */
static struct DotrPickNode made[5], final[5];
static int node_make_calls, node_score[5], node_score_calls, slot_copy_calls;
static int slot_pick_calls, slot_pick_which, slot_pick_ids_ok;
static struct DotrPickNode *slot_pick_node;
static int score_mode = 3, score_mode_calls, score_any, alt_make_calls;

struct DotrPickNode *TaLogic_GetFusionTree(int side, struct DotrPickNode *node, unsigned char slot, unsigned short id,
                                   int unknown, short *first, short *second)
{
    assert(side == 1 && node == 0 && unknown == 0 && *first == 0 && *second == 0);
    assert(slot < 5 && id == ids[slot]);
    node_make_calls++;
    *first = 7;
    *second = 9;
    return &made[slot];
}

struct DotrPickNode *TaLogic_GetFusionRes(struct DotrPickNode *node)
{
    return &final[node - made];
}

int TaLogic_GetEnchantQue(int side, struct DotrPickNode *node, int a, int b)
{
    assert(side == 1 && a == 0 && b == 0 && node >= final && node < final + 5);
    assert(score_mode >= 3 || score_any);
    node_score_calls++;
    return node_score[node - final];
}

/* 0x10e320 and 0x11a0d0 ask 0x23c160 which scorer to use. */
int Taku_Get_WinCount(void)
{
    score_mode_calls++;
    return score_mode;
}

int func_00279130_pick(int side, struct DotrPickNode *node, int a, int b)
{
    assert(side == 1 && a == 0 && b == 0 && node >= final && node < final + 5);
    assert(score_mode < 3);
    node_score_calls++;
    return node_score[node - final];
}

void TaLogic_CopyArray(unsigned char *source, unsigned char *destination)
{
    slot_copy_calls++;
    memcpy(destination, source, 5);
}

static void slot_pick(int which, int side, unsigned short *item, int *table)
{
    assert(side == 1);
    slot_pick_calls++;
    slot_pick_which = which;
    slot_pick_node = (struct DotrPickNode *)item;
    slot_pick_ids_ok = memcmp(table, ids, sizeof(ids)) == 0;
}

void TaLogic_ConvEmptyCombo__00114f20(int side, unsigned short *item, int *table) { slot_pick(0, side, item, table); }
void TaLogic_ConvEmptyCombo__00128900(int side, unsigned short *item, int *table) { slot_pick(1, side, item, table); }
void TaLogic_ConvEmptyCombo__0012dfc0(int side, unsigned short *item, int *table) { slot_pick(2, side, item, table); }
void TaLogic_ConvEmptyCombo__00132ee0(int side, unsigned short *item, int *table) { slot_pick(3, side, item, table); }
void TaLogic_ConvEmptyCombo__00136d60(int side, unsigned short *item, int *table) { slot_pick(4, side, item, table); }
void TaLogic_ConvEmptyCombo__0013b600(int side, unsigned short *item, int *table) { slot_pick(5, side, item, table); }
void TaLogic_ConvEmptyCombo__00144630(int side, unsigned short *item, int *table) { slot_pick(6, side, item, table); }
void TaLogic_ConvEmptyCombo__00148d40(int side, unsigned short *item, int *table) { slot_pick(7, side, item, table); }
void TaLogic_ConvEmptyCombo__0014d5c0(int side, unsigned short *item, int *table) { slot_pick(8, side, item, table); }
void TaLogic_ConvEmptyCombo__00160030(int side, unsigned short *item, int *table) { slot_pick(9, side, item, table); }
void TaLogic_ConvEmptyCombo__00164ec0(int side, unsigned short *item, int *table) { slot_pick(10, side, item, table); }
void TaLogic_ConvEmptyCombo__001691e0(int side, unsigned short *item, int *table) { slot_pick(11, side, item, table); }
void TaLogic_ConvEmptyCombo__001700d0(int side, unsigned short *item, int *table) { slot_pick(12, side, item, table); }
void TaLogic_ConvEmptyCombo__0010e6b0(int side, unsigned short *item, int *table) { slot_pick(13, side, item, table); }
void TaLogic_ConvEmptyCombo__0011a2f0(int side, unsigned short *item, int *table) { slot_pick(14, side, item, table); }
void TaLogic_ConvEmptyCombo__0011f150(int side, unsigned short *item, int *table) { slot_pick(15, side, item, table); }
void TaLogic_ConvEmptyCombo__00124a30(int side, unsigned short *item, int *table) { slot_pick(16, side, item, table); }
void TaLogic_ConvEmptyCombo__0013fa90(int side, unsigned short *item, int *table) { slot_pick(17, side, item, table); }
void TaLogic_ConvEmptyCombo__001566d0(int side, unsigned short *item, int *table) { slot_pick(18, side, item, table); }
void TaLogic_ConvEmptyCombo__00151880(int side, unsigned short *item, int *table) { slot_pick(19, side, item, table); }

typedef int (*PickBest)(int, unsigned char *, struct DotrPickBlock *);

static int run_best(PickBest f, unsigned char *item, struct DotrPickBlock *block)
{
    fill_calls = count_calls = afford_calls = mark_calls = 0;
    node_make_calls = node_score_calls = slot_copy_calls = slot_pick_calls = 0;
    memset(item, 0x11, 12);
    if (block) {
        block->apBoost = block->dpBoost = 0x2222;
    }
    return f(1, item, block);
}

static void check_best(PickBest f, int which)
{
    unsigned char item[12];
    unsigned char untouched[12];
    struct DotrPickTarget target;
    struct DotrPickBlock block;
    int i;

    memset(untouched, 0x11, sizeof(untouched));
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_answers, 0, sizeof(afford_answers));
    memset(&target, 0, sizeof(target));
    target.apWithFlags = 0xe000 + 3;
    target.dpWithFlags = (short)(0xe000 + 4);
    block.target = &target;
    set_ids(20, 21, 22, 23, 24);
    for (i = 0; i < 5; i++) {
        final[i].id = (unsigned short)(100 + i);
        memset(final[i].slots, 0x40 + i, 5);
        made[i].id = 0x7777;
    }

    /* a null block: -1 before any callee */
    count = 5;
    assert(run_best(f, item, 0) == -1 && fill_calls == 0 && count_calls == 0);

    /* a count of zero: -1 after the id read */
    count = 0;
    assert(run_best(f, item, &block) == -1 && fill_calls == 1 && fill_side == 1 && count_calls == 1);
    assert(node_make_calls == 0 && memcmp(item, untouched, 12) == 0 && block.apBoost == 0x2222);

    /* every kind >> 5 nonzero: nothing is built or scored */
    count = 5;
    for (i = 20; i < 25; i++) {
        kind_by[i] = 0x20;
    }
    assert(run_best(f, item, &block) == -1 && node_make_calls == 0 && node_score_calls == 0 && afford_calls == 0);
    assert(count_calls == 6 && memcmp(item, untouched, 12) == 0);

    /* kinds below 0x20 are scored; no affordable slot: the highest score, nothing picked */
    for (i = 20; i < 25; i++) {
        kind_by[i] = 0x1f;
    }
    kind_by[21] = 0x20;
    node_score[0] = 0x00050006; node_score[1] = 0x7fffffff; node_score[2] = 0x00090001;
    node_score[3] = 0x00090001; node_score[4] = 0x00020002;
    afford_answers[0] = 0; afford_answers[1] = 2; afford_answers[2] = 0; afford_answers[3] = 0;
    assert(run_best(f, item, &block) == 0x00090001 && node_make_calls == 4 && node_score_calls == 4 && afford_calls == 4);
    assert(afford_values[0] == 20 && afford_values[1] == 22 && afford_values[3] == 24);
    assert(slot_pick_calls == 0 && slot_copy_calls == 0 && mark_calls == 0 && memcmp(item, untouched, 12) == 0);
    assert(block.apBoost == 0x2222 && block.dpBoost == 0x2222);

    /* an affordable slot above the best affordable score is picked even when
       an unaffordable score is higher; the return value is still the highest */
    afford_answers[0] = 1; afford_answers[1] = 0; afford_answers[2] = 0; afford_answers[3] = 1;
    node_score[4] = 0x00050006;
    assert(run_best(f, item, &block) == 0x00090001);
    /* slot 4 equals slot 0's score: not strictly above, so only slot 0 is picked */
    assert(slot_pick_calls == 1 && slot_pick_which == which && slot_pick_node == &final[0] && slot_pick_ids_ok);
    assert(slot_copy_calls == 1 && memcmp(item + 6, final[0].slots, 5) == 0 && item[5] == 0x11 && item[11] == 0x11);
    assert(mark_calls == 1 && mark_id == 100 && mark_block == (struct DotrStateBlock *)&block);
    assert(block.apBoost == 5 - 3 && block.dpBoost == 6 - 4);

    /* one above: the later slot replaces the earlier pick and raises the result */
    node_score[2] = 0x00010001; node_score[3] = 0x00010002; node_score[4] = 0x00050007;
    assert(run_best(f, item, &block) == 0x00050007 && slot_pick_calls == 2 && slot_pick_node == &final[4]);
    assert(memcmp(item + 6, final[4].slots, 5) == 0 && mark_calls == 2 && mark_id == 104);
    assert(block.apBoost == 5 - 3 && block.dpBoost == 7 - 4);

    /* a score of -1 does not exceed the initial best; 0 does */
    node_score[0] = -1; node_score[2] = -2; node_score[3] = -3; node_score[4] = -1;
    assert(run_best(f, item, &block) == -1 && slot_pick_calls == 0 && memcmp(item, untouched, 12) == 0);
    node_score[4] = 0;
    assert(run_best(f, item, &block) == 0 && slot_pick_calls == 1 && slot_pick_node == &final[4]);
    assert(block.apBoost == -3 && block.dpBoost == -4);

    /* the count bounds the walk */
    count = 4;
    assert(run_best(f, item, &block) == -1 && node_make_calls == 3 && slot_pick_calls == 0 && count_calls == 5);
}

/* TaLogic015_SetComboQue (branch coverage): only affordable slots are built and scored. The arm that
   raises the result without a pick cannot be reached here (result and best move together)
   and is not host-tested. */
static void check_best_afford(PickBest f, int which)
{
    unsigned char item[12];
    unsigned char untouched[12];
    struct DotrPickTarget target;
    struct DotrPickBlock block;
    int i;

    memset(untouched, 0x11, sizeof(untouched));
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_answers, 0, sizeof(afford_answers));
    memset(&target, 0, sizeof(target));
    target.apWithFlags = 0xe000 + 3;
    target.dpWithFlags = (short)(0xe000 + 4);
    block.target = &target;
    set_ids(20, 21, 22, 23, 24);
    for (i = 0; i < 5; i++) {
        final[i].id = (unsigned short)(100 + i);
        memset(final[i].slots, 0x40 + i, 5);
        made[i].id = 0x7777;
    }

    /* a null block: -1 before any callee; a count of zero: -1 after the id read */
    count = 5;
    assert(run_best(f, item, 0) == -1 && fill_calls == 0 && count_calls == 0);
    count = 0;
    assert(run_best(f, item, &block) == -1 && fill_calls == 1 && fill_side == 1 && count_calls == 1);
    assert(node_make_calls == 0 && memcmp(item, untouched, 12) == 0 && block.apBoost == 0x2222);

    /* every kind >> 5 nonzero: 0x26fb10 is not asked */
    count = 5;
    for (i = 20; i < 25; i++) {
        kind_by[i] = 0x20;
    }
    assert(run_best(f, item, &block) == -1 && node_make_calls == 0 && node_score_calls == 0 && afford_calls == 0);
    assert(count_calls == 6 && memcmp(item, untouched, 12) == 0);

    /* kinds below 0x20 are asked; answers other than 1 (0 and 2): nothing built or scored */
    for (i = 20; i < 25; i++) {
        kind_by[i] = 0x1f;
    }
    kind_by[21] = 0x20;
    node_score[0] = 0x00050006; node_score[1] = 0x7fffffff; node_score[2] = 0x00090001;
    node_score[3] = 0x00090001; node_score[4] = 0x00050006;
    afford_answers[0] = 0; afford_answers[1] = 2; afford_answers[2] = 0; afford_answers[3] = 0;
    assert(run_best(f, item, &block) == -1 && node_make_calls == 0 && node_score_calls == 0 && afford_calls == 4);
    assert(afford_values[0] == 20 && afford_values[1] == 22 && afford_values[3] == 24);
    assert(slot_pick_calls == 0 && slot_copy_calls == 0 && mark_calls == 0 && memcmp(item, untouched, 12) == 0);
    assert(block.apBoost == 0x2222 && block.dpBoost == 0x2222);

    /* slots 0 and 4 affordable with equal scores: not strictly above, so only slot 0 is picked */
    afford_answers[0] = 1; afford_answers[3] = 1;
    assert(run_best(f, item, &block) == 0x00050006 && node_make_calls == 2 && node_score_calls == 2);
    assert(slot_pick_calls == 1 && slot_pick_which == which && slot_pick_node == &final[0] && slot_pick_ids_ok);
    assert(slot_copy_calls == 1 && memcmp(item + 6, final[0].slots, 5) == 0 && item[5] == 0x11 && item[11] == 0x11);
    assert(mark_calls == 1 && mark_id == 100 && mark_block == (struct DotrStateBlock *)&block);
    assert(block.apBoost == 5 - 3 && block.dpBoost == 6 - 4);

    /* one above: the later slot replaces the earlier pick; one below: it does not */
    node_score[4] = 0x00050007;
    assert(run_best(f, item, &block) == 0x00050007 && slot_pick_calls == 2 && slot_pick_node == &final[4]);
    assert(memcmp(item + 6, final[4].slots, 5) == 0 && mark_calls == 2 && mark_id == 104);
    assert(block.apBoost == 5 - 3 && block.dpBoost == 7 - 4);
    node_score[4] = 0x00050005;
    assert(run_best(f, item, &block) == 0x00050006 && slot_pick_calls == 1 && slot_pick_node == &final[0]
        && node_score_calls == 2);

    /* a score of -1 does not exceed the initial best; 0 does */
    node_score[0] = -1; node_score[4] = -1;
    assert(run_best(f, item, &block) == -1 && slot_pick_calls == 0 && memcmp(item, untouched, 12) == 0);
    node_score[4] = 0;
    assert(run_best(f, item, &block) == 0 && slot_pick_calls == 1 && slot_pick_node == &final[4]);
    assert(block.apBoost == -3 && block.dpBoost == -4);

    /* the count bounds the walk */
    count = 4;
    assert(run_best(f, item, &block) == -1 && node_make_calls == 1 && slot_pick_calls == 0 && count_calls == 5);
}

/* 0x11ef60: the 0x114d40 outcomes, and id 342 is skipped like a kind >> 5
   nonzero (341 and 343 are not). */
static void check_best_skip(PickBest f, int which)
{
    unsigned char item[12];
    struct DotrPickTarget target;
    struct DotrPickBlock block;
    int i;

    check_best(f, which);
    memset(&target, 0, sizeof(target));
    block.target = &target;
    memset(kind_by, 0, sizeof(kind_by));
    count = 5;
    for (i = 0; i < 5; i++) {
        node_score[i] = -1;
    }
    set_ids(20, 342, 22, 23, 24);
    assert(run_best(f, item, &block) == -1 && node_make_calls == 4 && node_score_calls == 4 && count_calls == 6);
    set_ids(20, 341, 343, 23, 24);
    assert(run_best(f, item, &block) == -1 && node_make_calls == 5 && node_score_calls == 5);
    set_ids(342, 342, 342, 342, 342);
    assert(run_best(f, item, &block) == -1 && node_make_calls == 0 && count_calls == 6);
}

/* 0x10e320 family: the 0x114d40 outcomes with each scorer (0x23c160 answering
   2 and 3, the boundary), one 0x23c160 call per scored slot. */
static void check_best_scorers(PickBest f, int which)
{
    int mode;

    for (mode = 2; mode <= 3; mode++) {
        score_mode = mode;
        score_mode_calls = 0;
        check_best(f, which);
        /* check_best ends on a walk of four slots, three of them scored */
        assert(node_score_calls == 3 && score_mode_calls > 3);
    }
    score_mode = 0;
    score_mode_calls = 0;
    check_best(f, which);
    assert(score_mode_calls > 3);
    score_mode = 3;
}

/* 0x124810: with a 0x26fbb0 word of 1500 << 16 (the boundary) every score is
   the 0x278f10 answer and the 0x114d40 outcomes hold; one below, the word is
   the score and 0x278f10 is not asked; a node id of 535 is skipped before the
   word is read (534 and 536 are not). */
static void check_best_word(PickBest f, int which)
{
    unsigned char item[12];
    struct DotrPickTarget target;
    struct DotrPickBlock block;

    limit_word = 1500 << 16;
    limit_calls = 0;
    check_best(f, which);
    assert(limit_calls > 3 && limit_id == 103);

    memset(&target, 0, sizeof(target));
    block.target = &target;
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_answers, 0, sizeof(afford_answers));
    count = 5;
    limit_word = (1499 << 16) | 7;
    limit_calls = 0;
    assert(run_best(f, item, &block) == ((1499 << 16) | 7) && node_score_calls == 0 && limit_calls == 5);
    assert(limit_id == 104 && afford_calls == 5 && slot_pick_calls == 0);
    afford_answers[1] = 1;
    limit_calls = 0;
    assert(run_best(f, item, &block) == ((1499 << 16) | 7) && slot_pick_calls == 1 && slot_pick_node == &final[1]);
    assert(block.apBoost == 1499 && block.dpBoost == 7 && node_score_calls == 0);

    final[1].id = 535;
    limit_calls = 0;
    assert(run_best(f, item, &block) == ((1499 << 16) | 7) && limit_calls == 4 && afford_calls == 4);
    assert(node_make_calls == 5 && slot_pick_calls == 1 && slot_pick_node == &final[2]);
    final[1].id = 534;
    final[2].id = 536;
    limit_calls = 0;
    assert(run_best(f, item, &block) == ((1499 << 16) | 7) && limit_calls == 5 && slot_pick_calls == 1);
}

/* 0x13f850's other node builder (0x23c160 below 3). */
struct DotrPickNode *func_00278c80_pick(int side, struct DotrPickNode *node, unsigned char slot, unsigned short id,
                                        int unknown, short *first, short *second)
{
    assert(side == 1 && node == 0 && unknown == 0 && *first == 0 && *second == 0);
    assert(slot < 5 && id == ids[slot] && score_mode < 3);
    alt_make_calls++;
    return &made[slot];
}

/* 0x13f850: at 0x23c160 == 4 the 0x114d40 outcomes; at 3 the same picks
   without the slot callee; at 2 the nodes come from 0x278c80. */
static void check_best_modes(PickBest f, int which)
{
    unsigned char item[12];
    struct DotrPickTarget target;
    struct DotrPickBlock block;

    score_mode = 4;
    score_mode_calls = alt_make_calls = 0;
    check_best(f, which);
    assert(score_mode_calls > 3 && alt_make_calls == 0);

    memset(&target, 0, sizeof(target));
    block.target = &target;
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_answers, 0, sizeof(afford_answers));
    afford_answers[1] = 1;
    node_score[0] = 5; node_score[1] = 4; node_score[2] = 3; node_score[3] = 2; node_score[4] = 1;
    count = 5;
    score_mode = 3;
    score_mode_calls = 0;
    assert(run_best(f, item, &block) == 5 && node_make_calls == 5 && alt_make_calls == 0);
    assert(slot_pick_calls == 0 && slot_copy_calls == 1 && memcmp(item + 6, final[1].slots, 5) == 0);
    assert(mark_calls == 1 && mark_id == 101 && block.dpBoost == 4 && score_mode_calls == 6);

    score_mode = 2;
    score_any = 1;
    score_mode_calls = 0;
    assert(run_best(f, item, &block) == 5 && node_make_calls == 0 && alt_make_calls == 5);
    assert(slot_pick_calls == 0 && slot_copy_calls == 1 && node_score_calls == 5 && score_mode_calls == 6);
    score_any = 0;
    score_mode = 3;
    score_mode_calls = 0;
}

/* 0x1564e0: only slots with a 0x26fb10 answer of 1 are built and scored; the
   low half of the score is compared (against the whole earlier best score,
   as the code does) and returned. */
static void check_best_low(PickBest f, int which)
{
    unsigned char item[12];
    unsigned char untouched[12];
    struct DotrPickTarget target;
    struct DotrPickBlock block;
    int i;

    memset(untouched, 0x11, sizeof(untouched));
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_answers, 0, sizeof(afford_answers));
    memset(&target, 0, sizeof(target));
    target.apWithFlags = 0xe000 + 3;
    target.dpWithFlags = (short)(0xe000 + 4);
    block.target = &target;
    set_ids(20, 21, 22, 23, 24);
    for (i = 0; i < 5; i++) {
        final[i].id = (unsigned short)(100 + i);
        memset(final[i].slots, 0x40 + i, 5);
    }

    /* a null block: -1 before any callee; a count of zero: -1 after the id read */
    count = 5;
    assert(run_best(f, item, 0) == -1 && fill_calls == 0 && count_calls == 0);
    count = 0;
    assert(run_best(f, item, &block) == -1 && fill_calls == 1 && count_calls == 1 && afford_calls == 0);

    /* every kind >> 5 nonzero: 0x26fb10 is not asked */
    count = 5;
    for (i = 20; i < 25; i++) {
        kind_by[i] = 0x20;
    }
    assert(run_best(f, item, &block) == -1 && afford_calls == 0 && node_make_calls == 0 && count_calls == 6);

    /* answers other than 1: asked for every slot, nothing built */
    for (i = 20; i < 25; i++) {
        kind_by[i] = 0x1f;
    }
    afford_answers[1] = 2;
    assert(run_best(f, item, &block) == -1 && afford_calls == 5 && afford_values[4] == 24);
    assert(node_make_calls == 0 && node_score_calls == 0 && memcmp(item, untouched, 12) == 0);

    /* slot 0 is picked; slot 3's low half 7 is not above the whole best
       score 0x50006, so it only raises the result */
    afford_answers[0] = 1; afford_answers[1] = 0; afford_answers[3] = 1;
    node_score[0] = 0x00050006; node_score[3] = 0x00010007;
    assert(run_best(f, item, &block) == 7 && node_make_calls == 2 && node_score_calls == 2);
    assert(slot_pick_calls == 1 && slot_pick_which == which && slot_pick_node == &final[0] && slot_pick_ids_ok);
    assert(slot_copy_calls == 1 && memcmp(item + 6, final[0].slots, 5) == 0 && item[5] == 0x11 && item[11] == 0x11);
    assert(mark_calls == 1 && mark_id == 100 && block.apBoost == 5 - 3 && block.dpBoost == 6 - 4);

    /* equal low halves: not picked, result unchanged; one above: picked */
    node_score[0] = 6; node_score[3] = 0x00090006;
    assert(run_best(f, item, &block) == 6 && slot_pick_calls == 1 && slot_pick_node == &final[0]);
    node_score[3] = 0x00090007;
    assert(run_best(f, item, &block) == 7 && slot_pick_calls == 2 && slot_pick_node == &final[3]);
    assert(mark_id == 103 && block.apBoost == 9 - 3 && block.dpBoost == 7 - 4);

    /* a low half of 0 exceeds the initial -1; -1 as a score has low half 0xffff */
    afford_answers[3] = 0;
    node_score[0] = 0x00030000;
    assert(run_best(f, item, &block) == 0 && slot_pick_calls == 1 && block.dpBoost == -4);
    node_score[0] = -1;
    assert(run_best(f, item, &block) == 0xffff && slot_pick_calls == 1);

    /* the count bounds the walk */
    afford_answers[0] = 0; afford_answers[4] = 1;
    node_score[4] = 9;
    assert(run_best(f, item, &block) == 9 && slot_pick_node == &final[4]);
    count = 4;
    assert(run_best(f, item, &block) == -1 && afford_calls == 4 && node_make_calls == 0 && count_calls == 5);
}

static void check_passes(PickFirst f)
{
    unsigned char item[12];
    unsigned char untouched[12];
    int n;

    memset(untouched, 0x11, sizeof(untouched));
    memset(kind_by, 0, sizeof(kind_by));
    set_ids(20, 21, 22, 23, 999);

    /* a null block: -1 after the id read, nothing counted, stored or forwarded */
    fill_calls = count_calls = mark_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, NULL, 22) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 0 && mark_calls == 0);
    assert(memcmp(item, untouched, 12) == 0);

    /* a count of zero: item[6..10] become 0xff and nothing beyond, -1 */
    count = 0;
    assert(run_first(f, item, 22) == -1 && count_calls == 3 && mark_calls == 0);
    for (n = 6; n < 11; n++) {
        assert(item[n] == 0xff);
    }
    assert(item[5] == 0x11 && item[11] == 0x11);

    /* first pass: the slot holding the id is taken before any kind is weighed */
    count = 5;
    kind_by[20] = 0x20;
    assert(run_first(f, item, 22) == 1 && item[6] == 2 && item[7] == 0xff && count_calls == 3);
    assert(mark_calls == 1 && mark_id == 22 && mark_block == (struct DotrStateBlock *)item);

    /* the id 999 never matches in the first pass; the second pass takes slot 0 */
    assert(run_first(f, item, 999) == 1 && item[6] == 0 && mark_id == 20 && count_calls == 7);

    /* second pass: kinds >> 5 of 0 and 2 are passed over, 1 and 3 are taken */
    kind_by[20] = 0x1f; kind_by[21] = 0x40; kind_by[22] = 0x5f; kind_by[23] = 0x60;
    assert(run_first(f, item, 24) == 1 && item[6] == 3 && mark_id == 23 && count_calls == 10);
    kind_by[23] = 0x3f;
    assert(run_first(f, item, 24) == 1 && item[6] == 3 && mark_id == 23);

    /* third pass: with only kinds 0 and 2 left the first non-zero kind is taken */
    kind_by[23] = 0;
    assert(run_first(f, item, 24) == 1 && item[6] == 1 && mark_id == 21 && count_calls == 14);
    assert(mark_calls == 1);

    /* no pass finds a slot: -1, item[6] stays 0xff */
    kind_by[21] = 0; kind_by[22] = 0x1f;
    assert(run_first(f, item, 24) == -1 && item[6] == 0xff && mark_calls == 0 && count_calls == 18);

    /* the count bounds every pass: one slot hides the id in slot 2 and the kind in slot 1 */
    kind_by[21] = 0x20;
    count = 1;
    assert(run_first(f, item, 22) == -1 && mark_calls == 0 && count_calls == 6);
    count = 2;
    assert(run_first(f, item, 22) == 1 && item[6] == 1 && mark_id == 21);
    count = 3;
    assert(run_first(f, item, 22) == 1 && item[6] == 2 && mark_id == 22);
    memset(kind_by, 0, sizeof(kind_by));
}

/* TaLogic013_GetSpecialComboPlay and TaLogic015_GetSpecialComboPlay (branch and loop coverage):
 * every callee is a stub. */
static int run_pairing(int (*f)(int, unsigned char *, struct DotrPickBlock *), unsigned char *item,
                       struct DotrPickBlock *block)
{
    fill_calls = count_calls = afford_calls = pair_calls = add_calls = mark_calls = 0;
    memset(item, 0x11, 12);
    memset(block, 0, sizeof(*block));
    block->apBoost = 100;
    block->dpBoost = -100;
    return f(1, item, block);
}

static void check_pairing(int (*f)(int, unsigned char *, struct DotrPickBlock *), int first, int second, int third)
{
    struct DotrPickBlock block;
    unsigned char item[12];

    memset(kind_by, 0, sizeof(kind_by));
    memset(pair_by, 0, sizeof(pair_by));
    memset(add_by, 0, sizeof(add_by));

    /* no slots at all: one count call, -1 */
    set_ids(first, second, 50, 51, 52);
    count = 0;
    assert(run_pairing(f, item, &block) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 1 && afford_calls == 0 && mark_calls == 0);
    assert(item[6] == 0x11 && item[7] == 0x11);

    /* the first id is missing: -1 after the first walk */
    count = 5;
    set_ids(second, 50, 51, 52, 53);
    assert(run_pairing(f, item, &block) == -1);
    assert(count_calls == 6 && afford_calls == 0 && mark_calls == 0 && item[6] == 0x11);

    /* the last slot with the first id is kept; the second id is missing: -1 */
    set_ids(first, 50, 51, first, 52);
    assert(run_pairing(f, item, &block) == -1);
    assert(item[6] == 3 && item[7] == 0x11 && count_calls == 12 && afford_calls == 0 && mark_calls == 0);

    /* the second id with an answer other than 1: -1 */
    set_ids(first, second, 51, 52, 53);
    afford_answers[0] = 0;
    assert(run_pairing(f, item, &block) == -1);
    assert(item[6] == 0 && item[7] == 0x11 && afford_calls == 1 && afford_values[0] == second && mark_calls == 0);

    /* both found (the last answering slot of the second id is kept); 795 and 796
     * are passed over although they qualify, slot 4 is appended */
    set_ids(first, second, 795, 796, second);
    afford_answers[0] = 1, afford_answers[1] = 1;
    kind_by[795] = kind_by[796] = 0x40;
    kind_by[50] = 0x5f, kind_by[51] = 0x40, kind_by[52] = 0x20, kind_by[53] = 0x40;
    assert(run_pairing(f, item, &block) == 1);
    assert(item[6] == 0 && item[7] == 4 && item[8] == 0x11 && afford_calls == 2);
    assert(mark_calls == 1 && mark_id == third && mark_block == (struct DotrStateBlock *)&block);
    assert(pair_calls == 3 && pair_first[0] == third && add_calls == 0);
    assert(block.apBoost == 100 && block.dpBoost == -100 && count_calls == 18);

    /* a negative pair answer and a kind >> 5 other than 2 are passed over; the
     * two qualifying slots are appended and their low halfwords added twice */
    set_ids(first, 50, 51, 52, 53);
    afford_answers[0] = 1;
    set_ids(second, first, 51, 52, 53);
    pair_by[51] = -1;
    add_by[50] = 0x10000, add_by[53] = 0x20007;
    assert(run_pairing(f, item, &block) == 1);
    assert(item[6] == 1 && item[7] == 0 && item[8] == 4 && item[9] == 0x11 && item[5] == 0x11);
    assert(pair_calls == 3 && add_calls == 2 && block.apBoost == 107 && block.dpBoost == -93);
    set_ids(second, first, 50, 52, 53);
    pair_by[51] = 0;
    add_by[50] = -3;
    assert(run_pairing(f, item, &block) == 1);
    assert(item[8] == 2 && item[9] == 4 && item[10] == 0x11 && add_calls == 4);
    assert(block.apBoost == 104 && block.dpBoost == -96);
    memset(kind_by, 0, sizeof(kind_by));
    memset(add_by, 0, sizeof(add_by));
}

static void check_follow(int (*f)(int, unsigned char *, struct DotrPickBlock *))
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    block.cardNo = 21;
    block.kind = 0xe7;
    memset(kind_by, 0, sizeof(kind_by));
    memset(pair_by, 0, sizeof(pair_by));

    /* a null block: item[6..10] are already 0xff, the ids are read, -1 */
    set_ids(20, 21, 22, 21, 24);
    count = 5;
    fill_calls = count_calls = afford_calls = pair_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, NULL) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 0 && afford_calls == 0);
    for (n = 6; n < 11; n++) {
        assert(item[n] == 0xff);
    }
    assert(item[5] == 0x11 && item[11] == 0x11);

    /* the last slot holding the block's id with an answer of 1 goes to item[6];
     * a kind >> 5 other than 0 ends there */
    kind_by[21] = 0x40;
    afford_answers[0] = 0, afford_answers[1] = 1;
    fill_calls = count_calls = afford_calls = pair_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, &block) == 7);
    assert(afford_calls == 2 && afford_values[0] == 21 && afford_values[1] == 21);
    assert(item[6] == 3 && item[7] == 0xff && item[10] == 0xff && pair_calls == 0);
    assert(count_calls == 6);

    /* kind >> 5 of 0: a negative pair answer, the slot itself, 795 and 796 are
     * passed over, the last slot is appended */
    kind_by[21] = 0x1f;
    kind_by[24] = kind_by[795] = kind_by[796] = 0x40;
    set_ids(20, 21, 795, 796, 24);
    pair_by[20] = -1;
    afford_answers[0] = 1;
    fill_calls = count_calls = afford_calls = pair_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, &block) == 7);
    assert(afford_calls == 1 && item[6] == 1 && item[7] == 4 && item[8] == 0xff);
    assert(pair_calls == 4 && pair_first[0] == 21 && pair_first[3] == 21);

    /* kinds 1 and 3 are passed over, kind 2 with a pair answer of 0 or more is
     * appended in slot order */
    set_ids(21, 30, 31, 32, 33);
    kind_by[30] = 0x40, kind_by[31] = 0x20, kind_by[32] = 0x5f, kind_by[33] = 0x60;
    pair_by[32] = 5;
    block.kind = 0x1f;
    afford_answers[0] = 1;
    fill_calls = count_calls = afford_calls = pair_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, &block) == 0);
    assert(item[6] == 0 && item[7] == 1 && item[8] == 3 && item[9] == 0xff && item[10] == 0xff);
    assert(pair_calls == 4 && item[11] == 0x11);
    memset(kind_by, 0, sizeof(kind_by));
    memset(pair_by, 0, sizeof(pair_by));
}

/* the third pass of 0x11ed50/0x13f640 also passes over one id */
static void check_passes_except(PickFirst f, int skipped, int other)
{
    unsigned char item[12];

    check_passes(f);
    memset(kind_by, 0, sizeof(kind_by));
    kind_by[skipped] = kind_by[other] = kind_by[21] = 0x40;
    count = 3;
    set_ids(skipped, 20, 21, 22, 23);
    assert(run_first(f, item, 24) == 1 && item[6] == 2 && mark_id == 21 && mark_calls == 1);
    set_ids(other, 20, 21, 22, 23);
    assert(run_first(f, item, 24) == 1 && item[6] == 0 && mark_id == other);
    set_ids(skipped, 20, skipped, 22, 23);
    assert(run_first(f, item, 24) == -1 && item[6] == 0xff && mark_calls == 0);
    memset(kind_by, 0, sizeof(kind_by));
}

/* TaLogic_SetSpecialComboQue__00124c90 (branch and loop coverage). */
static void check_kinds(int (*f)(int, unsigned char *, struct DotrPickBlock *, unsigned char))
{
    static const int kinds[5] = {30, 31, 32, 33, 34};
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    block.cardNo = 21;
    memset(kind_by, 0, sizeof(kind_by));
    memset(afford_answers, 0, sizeof(afford_answers));
    set_ids(20, 21, 22, 21, 24);
    memcpy(order_ids, kinds, sizeof(kinds));
    count = 5;

    /* a null block: item[6..10] set, the ids read, -1 */
    fill_calls = count_calls = afford_calls = order_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, NULL, 0) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 0 && afford_calls == 0 && order_calls == 0);
    for (n = 6; n < 11; n++) {
        assert(item[n] == 0xff);
    }
    assert(item[5] == 0x11 && item[11] == 0x11);

    /* mode 0: an empty count stores nothing */
    count = 0;
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 0) == 0);
    assert(count_calls == 1 && afford_calls == 0 && item[6] == 0xff && order_calls == 0);
    /* mode 0: slots holding the block's id are asked; an answer other than 1 is passed over */
    count = 5;
    afford_answers[0] = 1, afford_answers[1] = 2;
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 0) == 0);
    assert(count_calls == 6 && afford_calls == 2 && afford_values[0] == 21 && afford_values[1] == 21);
    assert(item[6] == 1 && item[7] == 0xff && item[8] == 0xff && order_calls == 0);
    /* the last such slot wins */
    afford_answers[1] = 1;
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 0) == 0);
    assert(item[6] == 3 && afford_calls == 2);
    /* the count bounds the walk */
    count = 3;
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 0) == 0);
    assert(item[6] == 1 && afford_calls == 1 && count_calls == 4);

    /* another mode (0x100 is 0 in a byte): the reordered ids are walked three times */
    count = 0;
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 2) == 0);
    assert(order_calls == 1 && order_side == 1 && count_calls == 3 && afford_calls == 0);
    assert(item[6] == 0xff && item[7] == 0xff && item[8] == 0xff);
    count = 5;
    kind_by[30] = 0, kind_by[31] = 18, kind_by[32] = 15, kind_by[33] = 0x20, kind_by[34] = 0;
    for (n = 0; n < 4; n++) {
        afford_answers[n] = 1;
    }
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 1) == 0);
    assert(order_calls == 1 && count_calls == 18 && afford_calls == 4);
    assert(afford_values[0] == 30 && afford_values[1] == 34 && afford_values[2] == 31 && afford_values[3] == 32);
    assert(item[6] == 4 && item[7] == 1 && item[8] == 2 && item[9] == 0xff && item[10] == 0xff);
    /* answers other than 1 leave the byte; kinds 0x32 and 0x2f (kind >> 5 of 1) are passed over */
    kind_by[33] = 0x32, kind_by[34] = 0x2f;
    afford_answers[0] = 1, afford_answers[1] = 0, afford_answers[2] = 2;
    fill_calls = count_calls = afford_calls = order_calls = 0;
    assert(f(1, item, &block, 0xff) == 0);
    assert(afford_calls == 3 && item[6] == 0 && item[7] == 0xff && item[8] == 0xff);
}

/* `fixed`: non-zero for the copy that answers 1 whatever the block's +0x3e byte holds */
static void check_append(int (*f)(int, unsigned char *, struct DotrPickBlock *), int fixed)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    block.cardNo = 21;
    block.kind = 0x65;
    memset(kind_by, 0, sizeof(kind_by));
    memset(pair_by, 0, sizeof(pair_by));
    set_ids(20, 21, 22, 23, 24);
    count = 5;

    /* a null block: item[6..10] set, the ids read, -1 */
    fill_calls = count_calls = pair_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, NULL) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 0 && pair_calls == 0);
    for (n = 6; n < 11; n++) {
        assert(item[n] == 0xff);
    }
    assert(item[5] == 0x11 && item[11] == 0x11);

    /* the first id's kind has a nonzero >> 5: only item[6] is set */
    kind_by[21] = 0x20;
    fill_calls = count_calls = pair_calls = 0;
    assert(f(1, item, &block) == (fixed ? 1 : 3));
    assert(count_calls == 6 && pair_calls == 0 && item[6] == 1 && item[7] == 0xff);
    /* kind >> 5 of 0: slots with a pair answer >= 0 and kind >> 5 of 2 follow; the first slot is skipped */
    kind_by[21] = 0x1f;
    kind_by[20] = 0x40, kind_by[22] = 0x40, kind_by[23] = 0x20, kind_by[24] = 0x5f;
    pair_by[22] = -1;
    fill_calls = count_calls = pair_calls = 0;
    assert(f(1, item, &block) == (fixed ? 1 : 3));
    assert(count_calls == 12 && pair_calls == 4 && pair_first[0] == 21 && pair_first[3] == 21);
    assert(item[6] == 1 && item[7] == 0 && item[8] == 4 && item[9] == 0xff && item[10] == 0xff);
    /* kind 0x60 (>> 5 of 3) is passed over; the count bounds both walks */
    kind_by[20] = 0x60;
    pair_by[22] = 0;
    count = 3;
    fill_calls = count_calls = pair_calls = 0;
    assert(f(1, item, &block) == (fixed ? 1 : 3));
    assert(count_calls == 8 && pair_calls == 2);
    assert(item[6] == 1 && item[7] == 2 && item[8] == 0xff);
    /* ids 795 and 796 are passed over; the last slot holding the block's id is the first */
    set_ids(795, 21, 796, 21, 24);
    kind_by[795] = 0x40, kind_by[796] = 0x40;
    count = 5;
    block.kind = 0x1f;
    fill_calls = count_calls = pair_calls = 0;
    assert(f(1, item, &block) == (fixed ? 1 : 0));
    assert(pair_calls == 4 && item[6] == 3 && item[7] == 4 && item[8] == 0xff);
    kind_by[795] = 0, kind_by[796] = 0;
}

static int words_calls, word_answer, word_calls, kind_answer, kind_calls, kind_a, kind_b;
static unsigned short *words_seen;

void TaUtil_GetUnitLst(int kind, int mode, unsigned short *out)
{
    int n;

    assert(kind == 1 && mode == 2);
    for (n = 0; n < 22; n++) {
        assert(out[n] == 0xffff);
    }
    words_seen = out;
    words_calls++;
}

int TaLogic_GetMaxApUnit(unsigned short *words)
{
    assert(words == words_seen);
    word_calls++;
    return word_answer;
}

int SzDuel_GetUnitKind(int a, int b)
{
    kind_a = a;
    kind_b = b;
    kind_calls++;
    return kind_answer;
}

static void check_kind_id(int (*f)(int, unsigned char *, struct DotrPickBlock *))
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    set_ids(721, 724, 722, 724, 721);
    count = 5;

    /* a null block: the words and ids are read, item is left alone, -1 */
    fill_calls = count_calls = words_calls = word_calls = kind_calls = 0;
    memset(item, 0x11, 12);
    assert(f(1, item, NULL) == -1);
    assert(words_calls == 1 && fill_calls == 1 && fill_side == 1 && word_calls == 0 && count_calls == 0);
    for (n = 0; n < 12; n++) {
        assert(item[n] == 0x11);
    }
    /* a negative word: item[6..10] set, -1 */
    word_answer = -1;
    fill_calls = count_calls = words_calls = word_calls = kind_calls = 0;
    assert(f(1, item, &block) == -1);
    assert(words_calls == 1 && word_calls == 1 && kind_calls == 0 && count_calls == 0);
    for (n = 6; n < 11; n++) {
        assert(item[n] == 0xff);
    }
    assert(item[5] == 0x11 && item[11] == 0x11);
    /* word 0 is accepted; a kind outside 18, 2 and 14 answers -1 */
    word_answer = 0;
    kind_answer = 3;
    fill_calls = count_calls = words_calls = word_calls = kind_calls = 0;
    assert(f(1, item, &block) == -1);
    assert(kind_calls == 1 && kind_a == 0 && kind_b == 0 && count_calls == 0 && item[6] == 0xff);
    /* kind 18 (low five bits): id 724, last slot holding it */
    word_answer = 0x1234;
    kind_answer = 0x20 | 18;
    fill_calls = count_calls = words_calls = word_calls = kind_calls = 0;
    assert(f(1, item, &block) == 724);
    assert(kind_calls == 1 && kind_a == 0x12 && kind_b == 0x34 && count_calls == 6 && item[6] == 3);
    assert(item[7] == 0xff);
    /* kind 2: id 721 */
    kind_answer = 2;
    assert(f(1, item, &block) == 721);
    assert(item[6] == 4);
    /* kind 14: id 722 */
    kind_answer = 14;
    assert(f(1, item, &block) == 722);
    assert(item[6] == 2);
    /* the count bounds the walk */
    kind_answer = 2;
    count = 4;
    count_calls = 0;
    assert(f(1, item, &block) == 721);
    assert(item[6] == 0 && count_calls == 5);
    /* an empty count: the id is still returned */
    count = 0;
    count_calls = 0;
    assert(f(1, item, &block) == 721);
    assert(item[6] == 0xff && count_calls == 1);
}

static int total_answer, total_calls;

int func_001e02f0_pick(struct DotrPickBlock *block, int unknown)
{
    assert(block != NULL && unknown == 0);
    total_calls++;
    return total_answer;
}

static void check_budget(int (*f)(int, unsigned char *, struct DotrPickBlock *))
{
    struct DotrPickBlock block;
    unsigned char item[16];
    int n;

    memset(&block, 0, sizeof(block));
    block.cardNo = 21;
    block.apBoost = 1000;
    block.dpBoost = 2000;
    memset(kind_by, 0, sizeof(kind_by));
    memset(pair_by, 0, sizeof(pair_by));
    memset(add_by, 0, sizeof(add_by));
    set_ids(20, 21, 22, 23, 24);
    count = 5;

    /* a null block: nothing is read or written */
    fill_calls = count_calls = pair_calls = add_calls = total_calls = 0;
    memset(item, 0x11, sizeof(item));
    assert(f(1, item, NULL) == -1);
    assert(fill_calls == 0 && count_calls == 0);
    for (n = 0; n < 16; n++) {
        assert(item[n] == 0x11);
    }
    /* an empty count: both walks are skipped */
    count = 0;
    assert(f(1, item, &block) == 1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 2 && pair_calls == 0 && item[6] == 0x11);
    /* the block id's kind has a nonzero >> 5: item[6] is left; no slot has kind >> 5 of 2 */
    count = 5;
    kind_by[21] = 0x20;
    fill_calls = count_calls = pair_calls = add_calls = total_calls = 0;
    assert(f(1, item, &block) == 1);
    assert(count_calls == 12 && pair_calls == 5 && pair_first[0] == 21 && pair_first[4] == 21);
    assert(item[6] == 0x11 && item[7] == 0x11 && total_calls == 0);
    /* kind >> 5 of zero sets item[6]; slots with a pair answer >= 0 and kind >> 5 of 2 are appended */
    kind_by[21] = 0x1f;
    kind_by[20] = 0x40, kind_by[22] = 0x40, kind_by[23] = 0x60, kind_by[24] = 0x5f;
    pair_by[22] = -1;
    add_by[20] = 0x10064, add_by[24] = 7;
    total_answer = 1392 - 0x10064; /* the whole sum is compared; the low halfword is added */
    fill_calls = count_calls = pair_calls = add_calls = total_calls = 0;
    assert(f(1, item, &block) == 1);
    assert(item[6] == 1 && item[7] == 0 && item[8] == 4 && item[9] == 0x11);
    assert(block.apBoost == 1107 && block.dpBoost == 2107);
    assert(total_calls == 2 && add_calls == 6 && count_calls == 12);
    /* a sum of 1400 ends the walk at that slot */
    add_by[20] = 100, add_by[24] = 7;
    total_answer = 1300;
    fill_calls = count_calls = pair_calls = add_calls = total_calls = 0;
    memset(item, 0x11, sizeof(item));
    assert(f(1, item, &block) == 1);
    assert(item[6] == 1 && item[7] == 0x11 && block.apBoost == 1107 && block.dpBoost == 2107);
    assert(pair_calls == 1 && total_calls == 1 && add_calls == 1 && count_calls == 9);
    /* 1399 passes */
    total_answer = 1299;
    add_by[24] = 200;
    fill_calls = count_calls = pair_calls = add_calls = total_calls = 0;
    assert(f(1, item, &block) == 1);
    assert(item[7] == 0 && item[8] == 0x11 && block.apBoost == 1207 && block.dpBoost == 2207);
    assert(pair_calls == 5 && total_calls == 2 && add_calls == 4);
    /* ids 795 and 796 are passed over; the last slot holding the block id is taken */
    set_ids(795, 21, 796, 21, 24);
    kind_by[795] = 0x40, kind_by[796] = 0x40;
    add_by[24] = 1;
    fill_calls = count_calls = pair_calls = add_calls = total_calls = 0;
    memset(item, 0x11, sizeof(item));
    assert(f(1, item, &block) == 1);
    assert(item[6] == 3 && item[7] == 4 && item[8] == 0x11 && total_calls == 1 && add_calls == 3);
    assert(block.apBoost == 1208 && block.dpBoost == 2208);
    kind_by[795] = 0, kind_by[796] = 0;
}

/* TaLogic019_GetSpecialComboPlay (branch and loop coverage). */
static void check_two_ids(int (*f)(int, unsigned char *, struct DotrPickBlock *))
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    /* no slot at all, and no 790: -1 before the second walk, item untouched */
    for (n = 0; n < 2; n++) {
        set_ids(297, 791, 789, 297, 297);
        count = n ? 5 : 0;
        memset(item, 0x11, 12);
        fill_calls = count_calls = afford_calls = mark_calls = 0;
        assert(f(1, item, &block) == -1);
        assert(fill_calls == 1 && fill_side == 1 && count_calls == (n ? 6 : 1));
        assert(afford_calls == 0 && mark_calls == 0 && item[6] == 0x11 && item[7] == 0x11);
    }
    /* 790 twice (the last slot wins) but no 297: -1 with item[6] stored */
    set_ids(790, 296, 790, 298, 0);
    count = 5;
    memset(item, 0x11, 12);
    fill_calls = count_calls = afford_calls = mark_calls = 0;
    assert(f(1, item, &block) == -1);
    assert(count_calls == 12 && afford_calls == 0 && mark_calls == 0 && item[6] == 2 && item[7] == 0x11);
    /* 297 present but 0x26fb10 answers 0 and 2: -1 */
    set_ids(297, 790, 297, 0, 0);
    count = 3;
    memset(item, 0x11, 12);
    afford_answers[0] = 0;
    afford_answers[1] = 2;
    fill_calls = count_calls = afford_calls = mark_calls = 0;
    assert(f(1, item, &block) == -1);
    assert(afford_calls == 2 && afford_values[0] == 297 && afford_values[1] == 297);
    assert(mark_calls == 0 && item[6] == 1 && item[7] == 0x11);
    /* two affordable 297 slots: the last one is stored, the block is marked; slots past the count
     * are not read */
    set_ids(297, 790, 297, 297, 790);
    count = 3;
    memset(item, 0x11, 12);
    afford_answers[0] = 1;
    afford_answers[1] = 1;
    fill_calls = count_calls = afford_calls = mark_calls = 0;
    assert(f(1, item, &block) == 1);
    assert(afford_calls == 2 && mark_calls == 1 && mark_block == (struct DotrStateBlock *)&block && mark_id == 297);
    assert(item[6] == 1 && item[7] == 2 && count_calls == 8);
    /* only the first 297 slot is affordable */
    afford_answers[0] = 1;
    afford_answers[1] = 0;
    memset(item, 0x11, 12);
    fill_calls = count_calls = afford_calls = mark_calls = 0;
    assert(f(1, item, &block) == 1 && item[7] == 0 && mark_calls == 1);
    for (n = 0; n < 12; n++) {
        assert(n == 6 || n == 7 || item[n] == 0x11);
    }
}

static int class_by[8], class_calls;
int SzDuel_Kind2Type(int value)
{
    assert(value >= 0 && value < 8);
    class_calls++;
    return class_by[value];
}

static int listed(int *list, unsigned char *item, struct DotrPickBlock *block, int n)
{
    count = n;
    memset(item, 0x11, 12);
    count_calls = afford_calls = mark_calls = class_calls = 0;
    return TaLogic007_GetCurseCardPlay(1, list, block, item);
}

/* TaLogic007_GetCurseCardPlay (branch and loop coverage). */
static void check_listed(void)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int list[5] = { 500, 795, 501, 684, 311 };
    int n;

    memset(&block, 0, sizeof(block));
    memset(class_by, 0, sizeof(class_by));
    memset(kind_by, 0, sizeof(kind_by));
    kind_by[500] = 2 << 5 | 3;
    kind_by[501] = 5 << 5 | 1;
    kind_by[795] = 1 << 5;
    kind_by[684] = 6 << 5;
    kind_by[311] = 7 << 5 | 9;
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 0;
    }
    /* empty list: neither walk runs */
    assert(listed(list, item, &block, 0) == -1 && count_calls == 2 && class_calls == 0 && afford_calls == 0);
    /* first walk: class 2 at the first slot and at a later one (classes 1 and 3 do not count) */
    class_by[2] = 2;
    assert(listed(list, item, &block, 5) == 2 && item[6] == 0 && mark_calls == 1 && mark_id == 500);
    assert(mark_block == (struct DotrStateBlock *)&block && class_calls == 1 && afford_calls == 0);
    class_by[2] = 1;
    class_by[1] = 3;
    class_by[5] = 2;
    assert(listed(list, item, &block, 5) == 5 && item[6] == 2 && mark_id == 501 && class_calls == 3);
    /* past the count the class-2 slot is not reached; nothing affordable: -1 */
    assert(listed(list, item, &block, 2) == -1 && class_calls == 2 && afford_calls == 2 && mark_calls == 0);
    assert(afford_values[0] == 500 && afford_values[1] == 795 && item[6] == 0x11);
    /* second walk: affordable but not one of the three ids, then each of the three */
    class_by[5] = 0;
    afford_answers[0] = 1;
    afford_answers[1] = 2;
    assert(listed(list, item, &block, 2) == -1 && mark_calls == 0);
    afford_answers[1] = 1;
    assert(listed(list, item, &block, 5) == 1 && item[6] == 1 && mark_id == 795 && afford_calls == 2);
    afford_answers[1] = 0;
    afford_answers[2] = 1;
    afford_answers[3] = 1;
    assert(listed(list, item, &block, 5) == 6 && item[6] == 3 && mark_id == 684 && afford_calls == 4);
    afford_answers[3] = 0;
    afford_answers[4] = 1;
    assert(listed(list, item, &block, 5) == 7 && item[6] == 4 && mark_id == 311 && afford_calls == 5);
    assert(mark_calls == 1 && class_calls == 5);
    for (n = 0; n < 12; n++) {
        assert(n == 6 || item[n] == 0x11);
    }
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 0;
    }
}

static int five(unsigned char *item, struct DotrStateBlock *block, int n)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = afford_calls = 0;
    return TaLogic001_GetProtectCardPlay(1, item, block);
}

/* TaLogic001_GetProtectCardPlay (branch and loop coverage). */
static void check_five_ids(void)
{
    static const int wanted[5] = { 823, 821, 824, 828, 825 };
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(kind_by, 0, sizeof(kind_by));
    for (n = 0; n < 5; n++) {
        kind_by[wanted[n]] = (n + 1) << 5 | 7;
    }
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 1;
    }
    /* empty list: five fills, five counts, nothing else */
    set_ids(823, 821, 824, 828, 825);
    assert(five(item, (struct DotrStateBlock *)&block, 0) == -1 && fill_calls == 5 && count_calls == 5 && afford_calls == 0);
    assert(mark_calls == 0 && item[6] == 0x11 && fill_side == 1);
    /* none of the five ids listed */
    set_ids(1, 2, 3, 4, 826);
    assert(five(item, (struct DotrStateBlock *)&block, 5) == -1 && afford_calls == 0 && mark_calls == 0 && fill_calls == 5);
    /* each id alone, at each slot */
    for (n = 0; n < 5; n++) {
        set_ids(1, 2, 3, 4, 5);
        ids[n] = wanted[n];
        assert(five(item, (struct DotrStateBlock *)&block, 5) == n + 1 && item[6] == n && mark_calls == 1 && mark_id == wanted[n]);
        assert(mark_block == (struct DotrStateBlock *)&block && afford_calls == 1 && afford_values[0] == wanted[n] && fill_calls == n + 1);
        /* past the count it is not reached */
        assert(five(item, (struct DotrStateBlock *)&block, n) == -1 && mark_calls == 0 && item[6] == 0x11);
    }
    /* order of the searches, not of the list, decides */
    set_ids(825, 828, 824, 821, 823);
    assert(five(item, (struct DotrStateBlock *)&block, 5) == 1 && item[6] == 4 && mark_id == 823 && fill_calls == 1);
    /* a listed id that is not affordable (0 or 2) is passed over for the next search */
    afford_answers[0] = 0;
    afford_answers[1] = 2;
    assert(five(item, (struct DotrStateBlock *)&block, 5) == 3 && item[6] == 2 && mark_id == 824 && afford_calls == 3 && fill_calls == 3);
    assert(afford_values[0] == 823 && afford_values[1] == 821 && afford_values[2] == 824);
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 0;
    }
    assert(five(item, (struct DotrStateBlock *)&block, 5) == -1 && afford_calls == 5 && mark_calls == 0);
    for (n = 0; n < 12; n++) {
        assert(item[n] == 0x11);
    }
}

static unsigned char flag_by[1024], busy_by[1024];
static int flag_calls, sort_calls, sort_lo, sort_hi, sort_key, sort_reverse, sort_table[5];
static unsigned short *sort_values;

int TaLogic017_IsExzParts(int id)
{
    flag_calls++;
    return flag_by[id];
}

int TaLogic017_IsRefuseEmptyCombo(int id) { return busy_by[id]; }

void TaLogic017_SortTempArray(unsigned short *values, int lo, int hi, int key, int *table)
{
    unsigned short tmp;

    sort_calls++;
    sort_values = values;
    sort_lo = lo;
    sort_hi = hi;
    sort_key = key;
    memcpy(sort_table, table, sizeof(sort_table));
    if (sort_reverse) {
        for (; lo < hi; lo++, hi--) {
            tmp = values[lo];
            values[lo] = values[hi];
            values[hi] = tmp;
        }
    }
}

static int ordered(unsigned char *item, struct DotrPickTotals *totals, int n)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = pair_calls = add_calls = flag_calls = sort_calls = 0;
    return TaLogic017_PlayExzCombo(1, item, totals);
}

/* TaLogic017_PlayExzCombo (branch and loop coverage). */
static void check_ordered(void)
{
    struct DotrPickTotals totals;
    unsigned char item[12];
    int n;

    memset(&totals, 0, sizeof(totals));
    memset(kind_by, 0, sizeof(kind_by));
    memset(flag_by, 0, sizeof(flag_by));
    memset(busy_by, 0, sizeof(busy_by));
    for (n = 0; n < 1024; n++) {
        pair_by[n] = -1;
        add_by[n] = 0;
    }
    sort_reverse = 0;
    set_ids(10, 11, 12, 13, 14);
    /* empty list, nothing flagged, flagged slot past the count: -1 and item untouched */
    assert(ordered(item, &totals, 0) == -1 && fill_calls == 1 && fill_side == 1 && flag_calls == 0);
    assert(ordered(item, &totals, 5) == -1 && flag_calls == 5 && mark_calls == 0 && sort_calls == 0);
    flag_by[14] = 1;
    assert(ordered(item, &totals, 4) == -1 && flag_calls == 4 && mark_calls == 0);
    flag_by[14] = 2;
    assert(ordered(item, &totals, 5) == -1 && flag_calls == 5);
    for (n = 0; n < 12; n++) {
        assert(item[n] == 0x11);
    }
    /* only the first flagged slot is taken; unpaired; the others come first in sorted order */
    flag_by[14] = 1;
    flag_by[12] = 1;
    busy_by[12] = 1;
    assert(ordered(item, &totals, 5) == 1 && flag_calls == 3 && mark_calls == 1 && mark_id == 12);
    assert(mark_block == (struct DotrStateBlock *)&totals && pair_calls == 5 && pair_first[0] == 12 && pair_first[4] == 12);
    assert(sort_calls == 1 && sort_lo == 0 && sort_hi == 3 && sort_key == 1 && memcmp(sort_table, ids, sizeof(ids)) == 0);
    assert(item[6] == 0 && item[7] == 1 && item[8] == 3 && item[9] == 4 && item[10] == 2);
    assert(add_calls == 0 && totals.total_38 == 0 && totals.total_3a == 0);
    /* the copy uses the list as the sort left it */
    sort_reverse = 1;
    assert(ordered(item, &totals, 5) == 1);
    assert(item[6] == 4 && item[7] == 3 && item[8] == 1 && item[9] == 0 && item[10] == 2);
    sort_reverse = 0;
    for (n = 0; n < 6; n++) {
        assert(item[n] == 0x11);
    }
    assert(item[11] == 0x11);
    /* pairing: result >= 0, kind exactly 2, not 795 or 796 */
    set_ids(10, 795, 12, 796, 14);
    memset(flag_by, 0, sizeof(flag_by));
    flag_by[10] = 1;
    busy_by[10] = 1;
    pair_by[795] = pair_by[12] = pair_by[796] = pair_by[14] = 0;
    kind_by[795] = kind_by[796] = 2 << 5;
    kind_by[12] = 2 << 5 | 3;
    kind_by[14] = 1 << 5;
    add_by[12] = 7;
    totals.total_38 = 100;
    totals.total_3a = -3;
    assert(ordered(item, &totals, 5) == 1 && mark_id == 10 && add_calls == 2);
    assert(totals.total_38 == 107 && totals.total_3a == 4 && sort_hi == 2);
    assert(item[6] == 1 && item[7] == 3 && item[8] == 4 && item[9] == 0 && item[10] == 2);
    kind_by[14] = 3 << 5;
    pair_by[12] = -1;
    assert(ordered(item, &totals, 5) == 1 && add_calls == 0 && totals.total_38 == 107 && sort_hi == 2);
    assert(item[6] == 1 && item[7] == 3 && item[8] == 4 && item[9] == 0xff && item[10] == 0);
    /* every slot reported busy: the unfilled list entries keep their 0xff */
    pair_by[12] = 0;
    for (n = 0; n < 1024; n++) {
        busy_by[n] = 1;
    }
    assert(ordered(item, &totals, 5) == 1 && sort_lo == 0 && sort_hi == -1);
    assert(item[6] == 0xff && item[7] == 0xff && item[8] == 0xff && item[9] == 0 && item[10] == 2);
    /* a shorter count bounds every walk */
    assert(ordered(item, &totals, 2) == 1 && pair_calls == 2 && add_calls == 0);
    assert(item[6] == 0xff && item[7] == 0 && item[8] == 0x11);
    for (n = 0; n < 1024; n++) {
        pair_by[n] = 0;
        add_by[n] = 0;
    }
    memset(kind_by, 0, sizeof(kind_by));
}

static int owned_answers[8], owned_calls, owned_values[8];
int TaHand_IsCardSummon(int index, int value)
{
    assert(index == 1 && owned_calls < 8);
    owned_values[owned_calls] = value;
    return owned_answers[owned_calls++];
}

static int after800(unsigned char *item, struct DotrPickBlock *block, int n)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = pair_calls = owned_calls = 0;
    return TaLogic011_GetSpecComboPlay(1, item, block);
}

/* TaLogic011_GetSpecComboPlay (branch and loop coverage). */
static void check_after800(void)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    memset(kind_by, 0x20, sizeof(kind_by));
    pair_by[800] = 0;
    for (n = 0; n < 8; n++) {
        owned_answers[n] = 2;
    }
    /* empty list: -1 after one count read */
    set_ids(500, 800, 501, 502, 800);
    assert(after800(item, &block, 0) == -1 && fill_calls == 1 && fill_side == 1 && count_calls == 1);
    assert(mark_calls == 0 && item[7] == 0x11);
    /* no slot holds 800 below the count: -1, nothing marked, no second walk */
    assert(after800(item, &block, 1) == -1 && count_calls == 2 && mark_calls == 0 && item[7] == 0x11);
    /* every 800 slot is stored and marked; all kinds have >> 5 nonzero: -1 */
    assert(after800(item, &block, 5) == -1 && item[7] == 4 && mark_calls == 2 && mark_id == 800);
    assert(mark_block == (struct DotrStateBlock *)&block && pair_calls == 0 && count_calls == 12);
    /* kinds pass, a negative 0x248f60 answer: -1 without asking 0x26fb60 */
    memset(kind_by, 0x1f, sizeof(kind_by));
    pair_by[800] = -1;
    assert(after800(item, &block, 5) == -1 && pair_calls == 5 && pair_first[0] == 500 && pair_first[4] == 800);
    assert(owned_calls == 0 && item[7] == 4);
    /* answer 0 is accepted; 0x26fb60 answers other than 1: -1 */
    pair_by[800] = 0;
    assert(after800(item, &block, 5) == -1 && owned_calls == 5 && owned_values[0] == 500 && owned_values[2] == 501);
    assert(mark_calls == 2);
    /* 795 and 796 are passed over; the next id (800 itself) is taken */
    for (n = 0; n < 8; n++) {
        owned_answers[n] = 1;
    }
    set_ids(795, 796, 800, 501, 502);
    assert(after800(item, &block, 5) == 0 && item[7] == 2 && mark_calls == 2 && mark_id == 800 && owned_calls == 3);
    for (n = 0; n < 12; n++) {
        assert(n == 7 || item[n] == 0x11);
    }
    /* the count bounds both walks */
    assert(after800(item, &block, 2) == -1 && count_calls == 3 && mark_calls == 0);
    /* a kind with >> 5 nonzero is passed over without the later tests */
    kind_by[501] = 0x20;
    set_ids(501, 800, 502, 503, 504);
    assert(after800(item, &block, 5) == 0 && item[7] == 1 && mark_calls == 2 && pair_calls == 1 && pair_first[0] == 800);
    /* first slot taken */
    kind_by[501] = 0x1f;
    assert(after800(item, &block, 5) == 0 && item[7] == 0 && mark_id == 501 && owned_calls == 1);
    memset(kind_by, 0, sizeof(kind_by));
}

static int by_mode(unsigned char *item, struct DotrPickBlock *block, int n, int mode)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = pair_calls = afford_calls = 0;
    return TaLogic004_GetSpecialComboPlay(1, item, block, mode);
}

/* TaLogic004_GetSpecialComboPlay (branch and loop coverage). */
static void check_by_mode(void)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    memset(kind_by, 0x20, sizeof(kind_by));
    pair_by[799] = 0;
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 2;
    }
    set_ids(799, 500, 799, 795, 796);
    /* a mode other than 0 and 1: -1 after the list is read, no count read */
    assert(by_mode(item, &block, 5, 2) == -1 && fill_calls == 1 && fill_side == 1 && count_calls == 0);
    assert(by_mode(item, &block, 5, 0xff) == -1 && count_calls == 0);
    /* mode 0, empty list and no 799 below the count: -1 */
    assert(by_mode(item, &block, 0, 0) == -1 && count_calls == 1 && item[6] == 0x11);
    set_ids(500, 799, 501, 795, 796);
    assert(by_mode(item, &block, 1, 0) == -1 && count_calls == 2 && item[6] == 0x11);
    /* the last 799 slot is stored; every kind has >> 5 nonzero: -1, nothing marked */
    set_ids(799, 500, 799, 795, 796);
    assert(by_mode(item, &block, 5, 0) == -1 && item[6] == 2 && item[7] == 0x11 && count_calls == 12);
    assert(pair_calls == 0 && mark_calls == 0);
    /* kinds pass, a negative 0x248f60 answer: -1 without asking 0x26fb10 */
    memset(kind_by, 0x1f, sizeof(kind_by));
    pair_by[799] = -1;
    assert(by_mode(item, &block, 5, 0) == -1 && pair_calls == 5 && pair_first[1] == 500 && afford_calls == 0);
    /* answer 0 is accepted; 0x26fb10 answers other than 1: -1 */
    pair_by[799] = 0;
    assert(by_mode(item, &block, 5, 0) == -1 && afford_calls == 5 && afford_values[1] == 500 && mark_calls == 0);
    /* 795 and 796 are passed over: nothing left */
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 1;
    }
    kind_by[799] = 0x20, kind_by[500] = 0x20;
    assert(by_mode(item, &block, 5, 0) == -1 && afford_calls == 2 && afford_values[0] == 795 && mark_calls == 0);
    /* the first id passing every test is stored, marked and answered */
    kind_by[500] = 0x1f;
    assert(by_mode(item, &block, 5, 0) == 0 && item[6] == 2 && item[7] == 1 && mark_calls == 1 && mark_id == 500);
    assert(mark_block == (struct DotrStateBlock *)&block && afford_calls == 1);
    for (n = 0; n < 12; n++) {
        assert(n == 6 || n == 7 || item[n] == 0x11);
    }
    /* mode 1: no 514, a 514 the side cannot afford, then one it can */
    assert(by_mode(item, &block, 5, 1) == -1 && count_calls == 6 && afford_calls == 0 && item[6] == 0x11);
    set_ids(500, 514, 501, 514, 514);
    kind_by[514] = 3 << 5 | 1;
    afford_answers[0] = 0, afford_answers[1] = 2;
    assert(by_mode(item, &block, 4, 1) == -1 && afford_calls == 2 && afford_values[0] == 514 && mark_calls == 0);
    assert(by_mode(item, &block, 5, 1) == 3 && afford_calls == 3 && item[6] == 4 && mark_calls == 1 && mark_id == 514);
    assert(item[7] == 0x11);
    afford_answers[0] = 1;
    assert(by_mode(item, &block, 5, 1) == 3 && item[6] == 1 && afford_calls == 1);
    assert(by_mode(item, &block, 0, 1) == -1 && count_calls == 1);
    memset(kind_by, 0, sizeof(kind_by));
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 0;
    }
}

static int after798(unsigned char *item, struct DotrPickBlock *block, int n, int mode)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = pair_calls = owned_calls = item_calls = score_calls = 0;
    grid_i = 0;
    grid_entry[0] = 0;
    entry_by[0] = 0x100;
    return TaLogic002_GetSpecialComboPlay(1, item, block, mode);
}

/* TaLogic002_GetSpecialComboPlay (branch and loop coverage). */
static void check_after798(void)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    memset(kind_by, 0x20, sizeof(kind_by));
    pair_by[798] = 0;
    for (n = 0; n < 8; n++) {
        owned_answers[n] = 2;
    }
    set_ids(798, 500, 798, 501, 502);
    grid_null[0] = 1;
    cell_answer = 1;
    /* a non-zero mode: -1 after the list is read, before the cell is asked */
    assert(after798(item, &block, 5, 1) == -1 && fill_calls == 1 && fill_side == 1 && item_calls == 0);
    /* a non-null SzDuel_GetMapUnit(0, 0) answer: -1; a SzDuel_GetUnitLoc(side, 0) answer other than 1: -1 */
    grid_null[0] = 0;
    assert(after798(item, &block, 5, 0) == -1 && item_calls == 1 && count_calls == 0);
    grid_null[0] = 1;
    cell_answer = 2;
    assert(after798(item, &block, 5, 0) == -1 && item_calls == 1 && count_calls == 0);
    cell_answer = 1;
    /* empty list, and no 798 below the count: -1 */
    assert(after798(item, &block, 0, 0) == -1 && count_calls == 1 && item[6] == 0x11);
    set_ids(500, 798, 501, 502, 503);
    assert(after798(item, &block, 1, 0) == -1 && count_calls == 2 && item[6] == 0x11);
    /* the last 798 slot is stored; every kind has >> 5 nonzero: -1 */
    set_ids(798, 500, 798, 501, 502);
    assert(after798(item, &block, 5, 0) == -1 && item[6] == 2 && item[7] == 0x11 && count_calls == 12);
    assert(pair_calls == 0 && mark_calls == 0);
    /* kinds pass, a negative 0x248f60 answer: -1 without asking 0x26fb60 */
    memset(kind_by, 0x1f, sizeof(kind_by));
    pair_by[798] = -1;
    assert(after798(item, &block, 5, 0) == -1 && pair_calls == 5 && pair_first[1] == 500 && owned_calls == 0);
    /* answer 0 is accepted; 0x26fb60 answers other than 1: -1 */
    pair_by[798] = 0;
    assert(after798(item, &block, 5, 0) == -1 && owned_calls == 5 && owned_values[1] == 500 && mark_calls == 0);
    /* the first id passing every test is stored, marked and answered */
    kind_by[798] = 0x20;
    owned_answers[0] = 1, owned_answers[1] = 1;
    assert(after798(item, &block, 5, 0) == 0 && item[6] == 2 && item[7] == 1 && mark_calls == 1 && mark_id == 500);
    assert(mark_block == (struct DotrStateBlock *)&block && owned_calls == 1);
    owned_answers[0] = 0;
    assert(after798(item, &block, 5, 0) == 0 && item[7] == 3 && mark_id == 501 && owned_calls == 2);
    for (n = 0; n < 12; n++) {
        assert(n == 6 || n == 7 || item[n] == 0x11);
    }
    memset(kind_by, 0, sizeof(kind_by));
}

static int after_mode(unsigned char *item, struct DotrPickBlock *block, int n, int mode)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = pair_calls = afford_calls = 0;
    return TaLogic_SetSpecialComboQue__0011f3c0(1, item, block, mode);
}

static void check_preset_only(const unsigned char *item)
{
    int n;

    for (n = 0; n < 12; n++) {
        assert(item[n] == (n >= 6 && n <= 10 ? 0xff : 0x11));
    }
}

/* TaLogic_SetSpecialComboQue__0011f3c0 (branch and loop coverage); 0x11eb10 below it is the real function. */
static void check_after_mode(void)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    memset(kind_by, 0x40, sizeof(kind_by));
    kind_by[500] = 0x1f;
    pair_by[799] = 0;
    for (n = 0; n < 16; n++) {
        afford_answers[n] = 1;
    }
    set_ids(500, 501, 502, 503, 504);
    /* null block: -1 before anything is stored or read */
    assert(after_mode(item, NULL, 5, 0) == -1 && fill_calls == 0);
    for (n = 0; n < 12; n++) {
        assert(item[n] == 0x11);
    }
    /* a mode other than 0 and 1: the preset and one list read only */
    assert(after_mode(item, &block, 5, 2) == 0 && fill_calls == 1 && fill_side == 1 && count_calls == 0);
    check_preset_only(item);
    /* mode 1: 0x11eb10 runs its mode-0 walk (no 799: six count reads) and nothing follows */
    assert(after_mode(item, &block, 5, 1) == 0 && fill_calls == 2 && count_calls == 6 && pair_calls == 0);
    check_preset_only(item);
    /* mode 0, empty list: one count read each */
    assert(after_mode(item, &block, 0, 0) == 0 && fill_calls == 2 && count_calls == 2 && pair_calls == 0);
    check_preset_only(item);
    /* 0x11eb10 takes slots 2 and 1; of the others 795 and 796 are passed over */
    set_ids(799, 500, 799, 795, 796);
    assert(after_mode(item, &block, 5, 0) == 0 && item[6] == 2 && item[7] == 1 && item[8] == 0);
    assert(mark_calls == 1 && pair_calls == 4 && pair_first[0] == 500);
    assert(pair_first[1] == 488 && pair_first[2] == 488 && pair_first[3] == 488);
    assert(item[9] == 0xff && item[10] == 0xff && item[5] == 0x11 && item[11] == 0x11);
    /* the last passing slot wins */
    set_ids(799, 500, 799, 501, 502);
    assert(after_mode(item, &block, 5, 0) == 0 && item[6] == 2 && item[7] == 1 && item[8] == 4);
    /* the count bounds the walk */
    assert(after_mode(item, &block, 4, 0) == 0 && item[8] == 3);
    /* a negative 0x248f60 answer, then a kind whose >> 5 is 1 and one whose >> 5 is 3 */
    pair_by[502] = -1;
    assert(after_mode(item, &block, 5, 0) == 0 && item[8] == 3);
    kind_by[501] = 0x20;
    assert(after_mode(item, &block, 5, 0) == 0 && item[8] == 0);
    kind_by[799] = 0x60;
    assert(after_mode(item, &block, 5, 0) == 0 && item[6] == 2 && item[7] == 1 && item[8] == 0xff);
    pair_by[502] = 0;
    memset(kind_by, 0, sizeof(kind_by));
}

static int after_427(unsigned char *item, struct DotrPickBlock *block, int n, int mode)
{
    count = n;
    memset(item, 0x11, 12);
    fill_calls = count_calls = mark_calls = pair_calls = owned_calls = item_calls = score_calls = 0;
    score_mode_calls = 0;
    grid_i = 0;
    grid_entry[0] = 0;
    entry_by[0] = 0x100;
    return TaLogic_SetSpecialComboQue__0010e540(1, item, block, mode);
}

/* TaLogic_SetSpecialComboQue__0010e540 (branch and loop coverage); 0x10f290 below it is the real function. */
static void check_after_427(void)
{
    struct DotrPickBlock block;
    unsigned char item[12];
    int n;

    memset(&block, 0, sizeof(block));
    memset(kind_by, 0x40, sizeof(kind_by));
    kind_by[500] = 0x1f;
    pair_by[798] = 0;
    for (n = 0; n < 8; n++) {
        owned_answers[n] = 1;
    }
    grid_null[0] = 1;
    cell_answer = 1;
    score_mode = 4;
    set_ids(798, 500, 798, 501, 502);
    /* null block: -1 before anything is stored or read */
    assert(after_427(item, NULL, 5, 0) == -1 && fill_calls == 0);
    for (n = 0; n < 12; n++) {
        assert(item[n] == 0x11);
    }
    /* a non-zero mode: the preset and one list read only */
    assert(after_427(item, &block, 5, 1) == 0 && fill_calls == 1 && fill_side == 1 && item_calls == 0);
    assert(score_mode_calls == 0);
    check_preset_only(item);
    /* 0x23c160 answers 3: 0x10f290 has taken slots 2 and 1 and nothing follows */
    score_mode = 3;
    assert(after_427(item, &block, 5, 0) == 0 && fill_calls == 2 && score_mode_calls == 1 && pair_calls == 1);
    assert(item[6] == 2 && item[7] == 1 && item[8] == 0xff && mark_calls == 1);
    /* answer 4: every other passing slot is stored in turn from item[8] */
    score_mode = 4;
    assert(after_427(item, &block, 5, 0) == 0 && item[6] == 2 && item[7] == 1);
    assert(item[8] == 0 && item[9] == 3 && item[10] == 4 && item[5] == 0x11 && item[11] == 0x11);
    assert(pair_calls == 4 && pair_first[0] == 500 && pair_first[1] == 427 && pair_first[3] == 427);
    /* empty list: nothing stored after the preset */
    assert(after_427(item, &block, 0, 0) == 0 && pair_calls == 0 && score_mode_calls == 1);
    check_preset_only(item);
    /* the count bounds the walk */
    assert(after_427(item, &block, 4, 0) == 0 && item[8] == 0 && item[9] == 3 && item[10] == 0xff);
    /* a negative 0x248f60 answer; a kind whose >> 5 is 1 */
    pair_by[501] = -1;
    assert(after_427(item, &block, 5, 0) == 0 && item[8] == 0 && item[9] == 4 && item[10] == 0xff);
    pair_by[501] = 0;
    kind_by[502] = 0x20;
    assert(after_427(item, &block, 5, 0) == 0 && item[8] == 0 && item[9] == 3 && item[10] == 0xff);
    /* 795 and 796 are passed over */
    set_ids(798, 500, 798, 795, 796);
    assert(after_427(item, &block, 5, 0) == 0 && item[8] == 0 && item[9] == 0xff && pair_calls == 4);
    score_mode = 3;
    memset(kind_by, 0, sizeof(kind_by));
}

/* TaLogic002_GetSpellPlay (branch and loop coverage): four passes over the ids, on the real 0x10f940
 * (answer_result -1 makes it 0; 0 with an empty list makes it 1). */
static void check_kind_passes(void)
{
    static char room[8];
    struct DotrStateBlock *block = (struct DotrStateBlock *)room;
    unsigned char item[12];
    int n;

    memset(kind_by, 0, sizeof(kind_by));
    set_ids(20, 21, 22, 23, 24);
    count = 5;
    list_count = 0;

    /* a null block: the ids are read, nothing is stored, -1 */
    fill_calls = count_calls = mark_calls = 0;
    memset(item, 0x11, 12);
    assert(TaLogic002_GetSpellPlay(1, item, NULL) == -1);
    assert(fill_calls == 1 && fill_side == 1 && count_calls == 0 && item[6] == 0x11 && item[10] == 0x11);

    /* an empty count: item[6..10] set, four count reads, -1 */
    count = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == -1);
    for (n = 6; n < 11; n++) {
        assert(item[n] == 0xff);
    }
    assert(item[5] == 0x11 && item[11] == 0x11 && count_calls == 4 && mark_calls == 0);

    /* pass 1: kind 5 and TaLogic002_IsDoCerem(side, id) == 1 */
    count = 5;
    kind_by[20] = 0x40, kind_by[21] = 0x20, kind_by[22] = 0x80, kind_by[23] = 0x60, kind_by[24] = 0xa0;
    answer_result = 0; record_index = -1; answer_value = -1;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1);
    assert(item[6] == 4 && item[7] == 0xff && mark_calls == 1 && mark_id == 24 && mark_block == block);
    assert(record_index == 24 && answer_value == 1);
    /* 0x10f940 answering 0: pass 2 takes the first kind 4 or kind 3 */
    answer_result = -1; mark_calls = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1 && item[6] == 2 && mark_id == 22 && mark_calls == 1);
    kind_by[22] = 0x60;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1 && item[6] == 2 && mark_id == 22);
    kind_by[22] = 0x40;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1 && item[6] == 3 && mark_id == 23);
    /* pass 3: the first kind that is neither 0 nor 2 (a refused kind 5 counts) */
    kind_by[23] = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1 && item[6] == 1 && mark_id == 21);
    kind_by[21] = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1 && item[6] == 4 && mark_id == 24);
    /* pass 4: the first kind other than 0 */
    kind_by[24] = 0; kind_by[20] = 0;
    count_calls = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == 1 && item[6] == 2 && mark_id == 22 && count_calls == 3 * 6 + 3);
    /* nothing: -1 with item[6] left 0xff */
    kind_by[22] = 0; mark_calls = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == -1 && item[6] == 0xff && mark_calls == 0);
    /* the count bounds every pass */
    kind_by[24] = 0x60; count = 4; count_calls = 0;
    assert(TaLogic002_GetSpellPlay(1, item, block) == -1 && count_calls == 20 && mark_calls == 0);
    memset(kind_by, 0, sizeof(kind_by));
    answer_result = 0;
}

static int low_score_by[8], low_score_calls;
int SzDuel_GetUnitSummonOrder(int a, int b)
{
    assert(a == grid_cell[grid_i] >> 8 && b == (grid_cell[grid_i] & 0xff));
    low_score_calls++;
    return low_score_by[grid_i];
}

static int run_lowest(int who)
{
    entry_calls = cell_calls = key_calls = low_score_calls = 0;
    word_calls = 1; /* past the stub's first-call guard */
    return Ta_GetExzodiaPartsUnit(2, who);
}

/* 0x268600 branch coverage: negative cell, kind mismatch, score at and below the bound, order, both
 * loop ends. */
static void check_lowest_cell(void)
{
    int i;
    grid_mode = 1;
    for (i = 0; i < 8; i++) {
        grid_entry[i] = ((i + 1) << 8) | (i + 2);
        grid_cell[i] = -1;
        grid_word_by[i] = 0;
        low_score_by[i] = 0;
    }
    /* every cell negative: nothing else asked */
    assert(run_lowest(1) == -1 && entry_calls == 8 && cell_calls == 8 && key_calls == 0 && low_score_calls == 0);
    /* first position, kind equal, score 254 (just below the starting bound) */
    grid_cell[0] = (3 << 8) | 5; grid_word_by[5] = 101; low_score_by[0] = 254;
    assert(run_lowest(1) == ((3 << 8) | 5) && key_calls == 1 && low_score_calls == 1);
    /* score 255 is not below the bound */
    low_score_by[0] = 255;
    assert(run_lowest(1) == -1 && low_score_calls == 1);
    /* kind differs: not scored */
    low_score_by[0] = 10;
    assert(run_lowest(2) == -1 && key_calls == 1 && low_score_calls == 0);
    /* a later equal score does not replace, a later lower one does; last position included */
    grid_cell[3] = (4 << 8) | 6; grid_word_by[6] = 101; low_score_by[3] = 10;
    assert(run_lowest(1) == ((3 << 8) | 5) && key_calls == 2 && low_score_calls == 2);
    grid_cell[7] = (1 << 8) | 2; grid_word_by[2] = 101; low_score_by[7] = 9;
    assert(run_lowest(1) == ((1 << 8) | 2) && key_calls == 3 && low_score_calls == 3 && entry_calls == 8);
    /* cell 0 (not negative) is a cell like any other */
    grid_cell[7] = 0; grid_word_by[0] = 101; low_score_by[7] = 0;
    assert(run_lowest(1) == 0 && low_score_calls == 3);
    grid_mode = 0;
    word_calls = 0;
}

/* 0x1307e0: limit-3000 walk with three cell-word vetoes. */
static void check_limit_words(void)
{
    int i;
    cellword_walk = 1;
    memset(cellword_by, 0, sizeof(cellword_by));
    memset(grid_bit_flip, 0, sizeof(grid_bit_flip));
    check_limit(TaLogic007_IsDoCerem, 3000, 1);
    answer_result = 7;
    /* word 298 or 326 on any cell: 0 at that cell */
    for (i = 0; i < 3; i++) {
        cellword_by[i] = 298;
        assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 0, 0) == 0 && score_calls == i + 1);
        cellword_by[i] = 326;
        assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 0, 0) == 0 && score_calls == i + 1);
        cellword_by[i] = 0;
    }
    /* neighbours of the constants pass */
    cellword_by[0] = 297; cellword_by[1] = 299; cellword_by[2] = 325;
    assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 0, 0) == 1 && score_calls == 3);
    cellword_by[0] = 327; cellword_by[1] = 304; cellword_by[2] = 306;
    assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 0, 0) == 1 && score_calls == 3);
    memset(cellword_by, 0, sizeof(cellword_by));
    /* word 305: passes with bit 4 set, fails with it clear */
    cellword_by[1] = 305; grid_i = 0; grid_cell[0] = entry_by[1];
    memset(grid_bit_calls, 0, sizeof(grid_bit_calls));
    assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 0, 0) == 1 && score_calls == 3 && grid_bit_calls[4] == 1);
    grid_bit_flip[4][entry_by[1] & 0xff] = 1;
    assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 0, 0) == 0 && score_calls == 2 && grid_bit_calls[4] == 2);
    /* the score limit is tested before the word */
    assert(run_limit(TaLogic007_IsDoCerem, 3, 0, 3000, 0) == 0 && score_calls == 2 && grid_bit_calls[4] == 2);
    grid_bit_flip[4][entry_by[1] & 0xff] = 0;
    cellword_by[1] = 0;
    cellword_walk = 0;
}

/* 0x124040: one assertion per stage outcome and slot bound. */
static int spell_storage[16]; /* the block is opaque here: only its address is forwarded */
#define spell_block (*(struct DotrStateBlock *)spell_storage)
static unsigned char spell_item[8];

static void reset_spell(int id)
{
    int i;

    for (i = 0; i < 5; i++) {
        ids[i] = i + 1;
    }
    ids[0] = id;
    count = 5;
    memset(spell_ctrl, 0xff, sizeof(spell_ctrl));
    memset(spell_attr, 0, sizeof(spell_attr));
    memset(spell_curse, 0, sizeof(spell_curse));
    memset(spell_card, 0, sizeof(spell_card));
    answer_result = -1; /* TaLogic005_IsDoCerem answers 0 */
}

static int run_spell(int side)
{
    fill_side = -1; mark_calls = 0; mark_id = -1; mark_block = NULL;
    memset(spell_item, 0xee, sizeof(spell_item));
    return TaLogic005_GetSpecialSpellPlay(side, spell_item, &spell_block);
}

static int spell_took(int r, int index, int id)
{
    return r == kind_by[id] >> 5 && spell_item[6] == index && mark_calls == 1 && mark_id == id &&
           mark_block == &spell_block && spell_item[5] == 0xee && spell_item[7] == 0xee;
}

static void check_special_spell(void)
{
    spell_mode = 1;
    pair_record.unknown_0 = 0x111; pair_record.unknown_4 = 0x222;
    kind_by[829] = 0x65; kind_by[844] = 0x41; kind_by[818] = 0x9f; kind_by[742] = 0x20;
    kind_by[796] = 0xc3; kind_by[820] = 0x1f;
    /* nothing wanted in the hand: -1, nothing stored */
    reset_spell(1);
    assert(run_spell(1) == -1 && mark_calls == 0 && spell_item[6] == 0xee && fill_side == 1);
    /* 829 is taken first, wherever it is */
    reset_spell(818); ids[2] = 829;
    assert(spell_took(run_spell(1), 2, 829));
    /* count bound: an entry past the count is not read; an empty hand gives -1 */
    reset_spell(1); ids[4] = 829; count = 4;
    assert(run_spell(1) == -1 && mark_calls == 0);
    count = 5;
    assert(spell_took(run_spell(1), 4, 829));
    count = 0;
    assert(run_spell(1) == -1);
    /* 844 only when TaLogic005_IsDoCerem(side, 844) is 1 */
    reset_spell(1); ids[1] = 844;
    assert(run_spell(1) == -1 && record_index == 844 && answer_value == 1);
    answer_result = 0; list_count = 0;
    assert(spell_took(run_spell(1), 1, 844));
    /* 818 unconditionally, after 844 */
    reset_spell(1); ids[3] = 818;
    assert(spell_took(run_spell(1), 3, 818));
    ids[4] = 844; answer_result = 0; list_count = 0;
    assert(spell_took(run_spell(1), 4, 844));
    /* 742 needs three controlled attribute-4 units in slots 0-5 of both sides */
    reset_spell(742);
    spell_ctrl[1][0] = 1; spell_attr[1][0] = 4;
    spell_ctrl[0][5] = 1; spell_attr[0][5] = 4;
    spell_attr[1][1] = 4;                         /* not controlled: not counted */
    spell_ctrl[1][2] = 1; spell_attr[1][2] = 3;   /* other attribute: not counted */
    assert(run_spell(1) == -1);
    spell_ctrl[1][5] = 1; spell_attr[1][5] = 4;
    assert(spell_took(run_spell(1), 0, 742));
    /* 796 needs a controlled unit with curse rest 255 in slots 0-10 */
    reset_spell(796);
    spell_curse[1][3] = 255;                      /* not controlled */
    spell_ctrl[0][4] = 1; spell_curse[0][4] = 254;
    assert(run_spell(1) == -1);
    spell_ctrl[0][10] = 1; spell_curse[0][10] = 255;
    assert(spell_took(run_spell(1), 0, 796));
    reset_spell(796); spell_ctrl[1][0] = 1; spell_curse[1][0] = 255;
    assert(spell_took(run_spell(1), 0, 796));
    /* 820 needs a controlled card 829 in slots 6-10; 796 goes first when both hold */
    reset_spell(820);
    spell_card[1][6] = 829;                       /* not controlled */
    spell_ctrl[1][7] = 1; spell_card[1][7] = 828;
    assert(run_spell(1) == -1);
    spell_ctrl[1][6] = 1;
    assert(spell_took(run_spell(1), 0, 820));
    reset_spell(820); spell_ctrl[0][10] = 1; spell_card[0][10] = 829;
    assert(spell_took(run_spell(1), 0, 820));
    ids[1] = 796; spell_curse[0][10] = 255;
    assert(spell_took(run_spell(1), 1, 796));
    /* side 0: the other side asked is 1, and control is compared with 0 */
    reset_spell(796); spell_ctrl[1][9] = 0; spell_curse[1][9] = 255;
    assert(spell_took(run_spell(0), 0, 796) && fill_side == 0);
    spell_ctrl[1][9] = 1;
    assert(run_spell(0) == -1);
    spell_mode = 0;
}

int main(void)
{
    check_kind_passes();
    check_passes_except(TaLogic004_GetSpellPlay, 799, 800);
    check_passes_except(TaLogic011_GetSpellPlay, 800, 799);
    check_pairing(TaLogic013_GetSpecialComboPlay, 797, 272, 273);
    check_pairing(TaLogic015_GetSpecialComboPlay, 799, 7, 501);
    check_follow(TaLogic007_SetSpecialComboQue);
    check_follow(TaLogic012_SetSpecialComboQue);
    check_given(TaLogic007_GetSpecCardPlay);
    check_given(TaLogic011_GetSpecCardPlay);
    check_given(TaLogic012_GetSpecCardPlay);
    check_triple(TaLogic001_IsDoCerem);
    check_triple(TaLogic020_IsDoCerem);
    check_triple(TaLogic008_IsDoCerem);
    check_limit(TaLogic009_IsDoCerem, 2600, 1);
    check_limit(TaLogic010_IsDoCerem, 3000, 1);
    check_limit(TaLogic013_IsDoCerem, 2400, 1);
    check_limit(TaLogic014_IsDoCerem, 2500, 1);
    check_limit(TaLogic019_IsDoCerem, 3000, 1);
    check_limit_zero(TaLogic018_IsDoCerem);
    check_limit_kinds(TaLogic002_IsDoCerem);
    check_limit(TaLogic005_IsDoCerem, 2600, 1);
    check_limit(TaLogic006_IsDoCerem, 2000, 1);
    check_limit(TaLogic012_IsDoCerem, 2500, 1);
    check_record_limit(TaLogicCom_IsDoCerem__00112850);
    check_record_limit(TaLogicCom_IsDoCerem__0016ce00);
    check_row(TaLogic019_IsEmptyCntrGuard);
    check_row(TaLogic022_IsEmptyCntrGuard);
    check_grid(TaLogic_IsExzodia__00112a40);
    check_grid(TaLogic_IsExzodia__0016cff0);
    check_grid(TaLogic017_IsExzodia);
    check_grid_bits(Ta_IsExzodia);
    check_mask(TaLogic005_InitRogicControl);
    check_mask(TaLogic006_InitRogicControl);
    check_mask_word(TaLogic003_InitRogicControl);
    check_mask_word(TaLogic004_InitRogicControl);
    static const PickFirst firsts[] = {TaLogic001_GetSpecCardPlay, TaLogic005_GetSpecialCardPlay, TaLogic006_GetSpecCardPlay, TaLogic008_GetSpecCardPlay, TaLogic009_GetSpecCardPlay,
                                       TaLogic010_GetSpecCardPlay, TaLogic013_GetSpecCardPlay, TaLogic014_GetSpecCardPlay, TaLogic015_GetSpecCardPlay, TaLogic018_GetSpecCardPlay,
                                       TaLogic019_GetSpecCardPlay, TaLogic020_GetSpecCardPlay, TaLogic021_GetSpecCardPlay, TaLogic022_GetSpecCardPlay};
    static const PickFill fills[] = {TaLogic022_SetSpecialComboQue, TaLogic019_SetSpecialComboQue, TaLogic017_SetSpecialComboQue, TaLogic014_SetSpecialComboQue,
                                     TaLogic015_SetSpecialComboQue, TaLogic018_SetSpecialComboQue, TaLogic013_SetSpecialComboQue};
    static const PickFill plain[] = {TaLogic009_SetSpecialComboQue, TaLogic010_SetSpecialComboQue, TaLogic021_SetSpecialComboQue, TaLogic008_SetSpecialComboQue,
                                     TaLogic020_SetSpecialComboQue, TaLogic006_SetSpecialComboQue};
    size_t i;

    for (i = 0; i < sizeof(fills) / sizeof(fills[0]); i++) {
        check_fill(fills[i], 1);
    }
    for (i = 0; i < sizeof(plain) / sizeof(plain[0]); i++) {
        check_fill(plain[i], 0);
    }
    for (i = 0; i < sizeof(firsts) / sizeof(firsts[0]); i++) {
        check_first(firsts[i]);
    }
    {
        static const PickFirst passes[] = {TaLogic001_GetSpellPlay, TaLogic003_GetSpellPlay, TaLogic005_GetSpellPlay, TaLogic006_GetSpellPlay,
                                           TaLogic007_GetSpellPlay, TaLogic008_GetSpellPlay, TaLogic009_GetSpellPlay, TaLogic010_GetSpellPlay,
                                           TaLogic012_GetSpellPlay, TaLogic013_GetSpellPlay, TaLogic014_GetSpellPlay, TaLogic015_GetSpellPlay,
                                           TaLogic017_GetSpellPlay, TaLogic018_GetSpellPlay, TaLogic019_GetSpellPlay, TaLogic020_GetSpellPlay,
                                           TaLogic021_GetSpellPlay, TaLogic022_GetSpellPlay};
        for (i = 0; i < sizeof(passes) / sizeof(passes[0]); i++) {
            check_passes(passes[i]);
        }
    }
    {
        static const PickBest bests[] = {TaLogic001_SetComboQue, TaLogic006_SetComboQue, TaLogic007_SetComboQue, TaLogic008_SetComboQue,
                                         TaLogic009_SetComboQue, TaLogic010_SetComboQue, TaLogic012_SetComboQue, TaLogic013_SetComboQue,
                                         TaLogic014_SetComboQue, TaLogic019_SetComboQue, TaLogic020_SetComboQue, TaLogic021_SetComboQue,
                                         TaLogic022_SetComboQue};
        for (i = 0; i < sizeof(bests) / sizeof(bests[0]); i++) {
            check_best(bests[i], (int)i);
        }
        assert(score_mode_calls == 0);
        check_best_skip(TaLogic_SetComboQue__0011ef60, 15);
        check_best_scorers(TaLogic_SetComboQue__0010e320, 13);
        check_best_scorers(TaLogic_SetComboQue__0011a0d0, 14);
        check_best_word(TaLogic005_SetComboQue, 16);
        check_best_modes(TaLogic011_SetComboQue, 17);
        check_best_low(TaLogic017_SetComboQue, 18);
        check_best_afford(TaLogic015_SetComboQue, 19);
    }
    check_kinds(TaLogic_SetSpecialComboQue__00124c90);
    check_append(TaLogic011_SetSpecialComboQue, 0);
    check_append(TaLogic001_SetSpecialComboQue, 1);
    check_kind_id(TaLogic003_IsSpellPlay);
    check_budget(TaLogic018_SetComboQue);
    check_two_ids(TaLogic019_GetSpecialComboPlay);
    check_listed();
    check_after800();
    check_by_mode();
    check_after798();
    check_after_mode();
    check_after_427();
    check_lowest_cell();
    check_limit_words();
    check_five_ids();
    check_ordered();
    check_special_spell();
    puts("pick_copies: ok");
    return 0;
}

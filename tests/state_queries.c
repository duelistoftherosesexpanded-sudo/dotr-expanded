/* Synthetic tests of the state queries. The record and entry routines
 * are tested in their own groups and stubbed here. */
#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/state_queries.h"
#include "dotr/ee/draw_state.h"

static struct DotrStateEntry20 entry;
static int need_args[2], need_result, have_argument, have_result, counts[2];

static int need_calls, need_by_id_on, need_by_id[8];
int TaHand_GetCardSummonCost2(int value, int index)
{
    need_args[0] = value;
    need_args[1] = index;
    need_calls++;
    if (need_by_id_on && value >= 100 && value < 108) return need_by_id[value - 100];
    return need_result;
}

int SzDuel_GetSummonPower(int index)
{
    have_argument = index;
    return have_result;
}

static struct DotrStateEntry20 node_entries[8];

/* Ids 100-107 select per-node entries for the tree search of 0x278e40. */
struct DotrStateEntry20 *SzDuel_GetCardConstInfo(int id)
{
    if (id >= 100 && id < 108) {
        return &node_entries[id - 100];
    }
    return id == 40 ? &entry : 0;
}

int SzDuel_GetHandCount(int index) { return counts[0] + index; }
int SzDuel_GetDeckCount(int index) { return counts[1] - index; }

static struct DotrStateItem item_fixture;
/* Per-slot tables for the 0x266c70 family: when query_mode is set the stubs answer from them. */
static int query_mode, q_present[2][11], q_bits[2][11], q_kind[2][11];
static struct DotrStateEntry8 q_entries[2][11];
static int order[16], order_count, extra_args[3];
static struct DotrStateItem *lookup_result = &item_fixture;
static int clamp_bias = 100000;
static struct DotrStateItem *extra_item;

static void mark(int id)
{
    assert(order_count < 16);
    order[order_count++] = id;
}

struct DotrStateItem *SzDuel_GetUnit(int a, int b)
{
    if (query_mode) {
        return q_present[a][b] ? &item_fixture : 0;
    }
    mark(1);
    assert(a == 3 && b == 4);
    return lookup_result;
}

int func_001df540_word(int a, int b)
{
    return q_present[a][b];
}

int Ta_GetParamOfs(struct DotrStateItem *item, int c, int d, int flag)
{
    mark(2);
    extra_item = item;
    extra_args[0] = c;
    extra_args[1] = d;
    extra_args[2] = flag;
    return 1000;
}

int GetParCap(int value)
{
    mark(3);
    return value + clamp_bias;
}

int SzDuel_GetUnitAP(int a, int b, int flag) { mark(4); assert(a == 3 && b == 4 && flag == 0); return 20; }
int SzDuel_GetUnitDP(int a, int b, int flag) { mark(5); assert(a == 3 && b == 4 && flag == 0); return 30; }
int SzDuel_GetUnitAP2(struct DotrStateItem *item, int flag) { mark(6); assert(item == &item_fixture && flag == 0); return 40; }
int SzDuel_GetUnitDP2(struct DotrStateItem *item, int flag) { mark(7); assert(item == &item_fixture && flag == 0); return 50; }

static void check_sums(void)
{
    /* Coordinate forms explicitly sequence lookup, extra term, base term,
     * and the optional clamp in the same order as the original. */
    order_count = 0;
    assert(TaUtil_GetUnitAP(3, 4, 5, 6) == 101020 && order_count == 4 && order[3] == 3);
    assert(order[0] == 1 && order[1] == 2 && order[2] == 4);
    assert(extra_item == &item_fixture && extra_args[0] == 5 && extra_args[1] == 6 && extra_args[2] == 1);
    order_count = 0;
    assert(TaUtil_GetUnitAP3(3, 4, 5, 6) == 1020 && order_count == 3);
    assert(order[0] == 1 && order[1] == 2 && order[2] == 4);
    assert(extra_item == &item_fixture && extra_args[0] == 5 && extra_args[1] == 6 && extra_args[2] == 1);
    order_count = 0;
    assert(TaUtil_GetUnitDP(3, 4, 7, 8) == 101030 && order_count == 4 && order[3] == 3);
    assert(order[0] == 1 && order[1] == 2 && order[2] == 5);
    assert(extra_item == &item_fixture && extra_args[0] == 7 && extra_args[1] == 8 && extra_args[2] == 0);
    order_count = 0;
    assert(TaUtil_GetUnitDP3(3, 4, 7, 8) == 1030 && order_count == 3);
    assert(order[0] == 1 && order[1] == 2 && order[2] == 5);
    assert(extra_item == &item_fixture && extra_args[0] == 7 && extra_args[1] == 8 && extra_args[2] == 0);
    /* Item forms: base term first, then the extra term and the clamp. */
    order_count = 0;
    assert(TaUtil_GetUnitAP2(&item_fixture, 5, 6) == 101040 && order_count == 3);
    assert(order[0] == 6 && order[1] == 2 && order[2] == 3 && extra_args[2] == 1);
    order_count = 0;
    assert(TaUtil_GetUnitDP2(&item_fixture, 5, 6) == 101050 && order_count == 3);
    assert(order[0] == 7 && order[1] == 2 && order[2] == 3 && extra_args[2] == 0);
}

static struct DotrStateEntry8 slot_entries[5];
static int slot_present[5], slot_calls, range_result, item_query_result;
static struct DotrStateItem *item_query_argument;

struct DotrStateEntry8 *SzDuel_GetUnitEffect(int a, int b, int n)
{
    if (query_mode) {
        assert(n == 2);
        return q_kind[a][b] < 0 ? 0 : &q_entries[a][b];
    }
    assert(a == 3 && b == 4 && n >= 0 && n < 5);
    slot_calls++;
    return slot_present[n] ? &slot_entries[n] : 0;
}

static int range_calls, range_last[2];

int SzDuel_GetUnitKind(int a, int b)
{
    range_calls++;
    range_last[0] = a;
    range_last[1] = b;
    /* 0x24df00 also queries (3, 0) before rejecting a zero position. */
    assert(query_mode || (a == 3 && (b == 4 || b == 0)));
    return range_result;
}

/* Host stand-in for the one-argument call of 0x250a60. */
int func_00250a60_item(struct DotrStateItem *item)
{
    item_query_argument = item;
    return item_query_result;
}

static int grid_word, limit_result, limit_calls, store_calls, store_value, flag_calls, flag_value;
static int list_count, list_ids[8], pair_count[2], pair_ids[2][8];

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == 3 && b == 4);
    return grid_word;
}

int SzDuel_GetUnitCurseRest(int a, int b)
{
    assert(a == 3 && b == 4 && store_calls == 0);
    limit_calls++;
    return limit_result;
}

int SzDuel_SetUnitCurseRest(int a, int b, int value)
{
    assert(a == 3 && b == 4 && flag_calls == 0);
    store_calls++;
    store_value = value;
    return 0;
}

int SzDuel_SetUnitCurseFlag(int a, int b, int flag)
{
    assert(a == 3 && b == 4 && store_calls == 1);
    flag_calls++;
    flag_value = flag;
    return 0;
}

int SzDuel_GetGraveCount(int index)
{
    assert(index == 9 || index == 0 || index == 1);
    return index < 2 ? pair_count[index] : list_count;
}

int SzDuel_InqGrave(int index, int position)
{
    /* 0x250000 also reads the position equal to the count. */
    assert((index == 9 || index == 0 || index == 1) && position >= 0 && position < 8);
    assert(index >= 2 ? position <= list_count : position < pair_count[index]);
    return index < 2 ? pair_ids[index][position] : list_ids[position];
}

static void step(int n, int limit)
{
    limit_result = limit;
    limit_calls = store_calls = flag_calls = 0;
    store_value = flag_value = -1;
    Ta_SetUnitCurseRest(3, 4, (unsigned char)n);
}

static void check_grid_queries(void)
{
    static const int ids[] = {40, 1, 40, 2, 40};
    int i;

    /* 0x24f2f0: both real queries run over the stubs; the second receives the
     * arithmetically shifted upper part. */
    grid_word = 0x0706;
    order_count = 0;
    assert(Ta_GetAPPlusDP(3, 4) == 101020 + 101030 && order_count == 8);
    assert(order[2] == 4 && order[3] == 3 && order[6] == 5 && order[7] == 3);
    assert(extra_args[0] == 6 && extra_args[1] == 7 && extra_args[2] == 0);
    grid_word = (int)0xffff8506u;
    order_count = 0;
    assert(Ta_GetAPPlusDP(3, 4) == 101020 + 101030 && extra_args[0] == 6 && extra_args[1] == -123);

    /* 0x2503b0: zero stores directly; a positive count below the limit does
     * nothing; otherwise it stores and then calls 0x1e1420 with one. */
    step(0, 0);
    assert(limit_calls == 0 && store_calls == 1 && store_value == 0 && flag_calls == 0);
    step(5, 6);
    assert(limit_calls == 1 && store_calls == 0 && flag_calls == 0);
    step(5, 5);
    assert(limit_calls == 1 && store_calls == 1 && store_value == 5 && flag_calls == 1 && flag_value == 1);
    step(255, -1);
    assert(store_calls == 1 && store_value == 255 && flag_calls == 1);
    step(0x100, 0); /* only the low byte is used */
    assert(limit_calls == 0 && store_calls == 1 && store_value == 0 && flag_calls == 0);

    /* 0x250940: the real 0x26fce0 gives 0xc7 for id 40 and 0 otherwise. */
    memset(&entry, 0, sizeof(entry));
    ((unsigned char *)&entry)[0] = 0xc7;
    for (i = 0; i < 5; i++) {
        list_ids[i] = ids[i];
    }
    list_count = 5;
    assert(TaUtil_GetGraveCardNum(9, 6) == 3 && TaUtil_GetGraveCardNum(9, 0) == 2 && TaUtil_GetGraveCardNum(9, 7) == 0);
    list_count = 2;
    assert(TaUtil_GetGraveCardNum(9, 6) == 1 && TaUtil_GetGraveCardNum(9, 0) == 1);
    list_count = 0;
    assert(TaUtil_GetGraveCardNum(9, 0) == 0);
    list_count = -3;
    assert(TaUtil_GetGraveCardNum(9, 0) == 0);
    memset(&entry, 0, sizeof(entry));
}

static struct DotrStateEntry8 *slot(int n)
{
    order_count = 0;
    slot_calls = 0;
    return Ta_GetUnitEffect(3, 4, n);
}

static int any_slot(void)
{
    order_count = 0;
    slot_calls = 0;
    return TaUtil_IsExMonster(3, 4);
}

static void check_slots(void)
{
    int i;
    /* 0x250740: missing item, missing entry, both words -1, either word set. */
    lookup_result = 0;
    slot_present[2] = 1;
    assert(slot(2) == 0 && slot_calls == 0 && order_count == 1);
    lookup_result = &item_fixture;
    slot_present[2] = 0;
    assert(slot(2) == 0 && slot_calls == 1);
    slot_present[2] = 1;
    slot_entries[2].unknown_words[0] = -1;
    slot_entries[2].unknown_words[1] = -1;
    assert(slot(2) == 0 && slot_calls == 1);
    slot_entries[2].unknown_words[1] = 0;
    assert(slot(2) == &slot_entries[2]);
    slot_entries[2].unknown_words[0] = 5;
    slot_entries[2].unknown_words[1] = -1;
    assert(slot(2) == &slot_entries[2]);
    assert(slot_entries[2].unknown_words[0] == 5 && slot_entries[2].unknown_words[1] == -1);

    /* 0x2507d0: a word with bits above the low five gives -1 before any lookup. */
    range_result = 32;
    assert(any_slot() == -1 && slot_calls == 0 && order_count == 0);
    range_result = -1;
    assert(any_slot() == -1 && slot_calls == 0);
    range_result = 31;
    assert(any_slot() == 1 && slot_calls == 3); /* slots 0 and 1 absent, slot 2 usable */
    for (i = 0; i < 5; i++) {
        slot_present[i] = 1;
        slot_entries[i].unknown_words[0] = -1;
        slot_entries[i].unknown_words[1] = -1;
    }
    assert(any_slot() == 0 && slot_calls == 5 && order_count == 5);
    slot_entries[4].unknown_words[1] = 9;
    assert(any_slot() == 1 && slot_calls == 5);
    slot_entries[0].unknown_words[0] = 0;
    assert(any_slot() == 1 && slot_calls == 1);
    lookup_result = 0;
    assert(any_slot() == 0 && slot_calls == 0 && order_count == 5);
    lookup_result = &item_fixture;
}

static void set_item(int byte_3e, int byte_45)
{
    memset(&item_fixture, 0, sizeof(item_fixture));
    item_fixture.kind = (unsigned char)byte_3e;
    item_fixture.slot = (unsigned char)byte_45;
    item_fixture.locCol = 7;
    item_fixture.locRow = 9;
    order_count = 0;
    item_query_argument = 0;
}

/* 0x24fdd0 runs the real 0x250a60 over the stubs (40 + 1000 + bias);
 * 0x24fe40 uses the one-argument stand-in. */
static void check_item_gates(void)
{
    static const int rejected[][2] = {{0x20 | 5, 1}, {0x80 | 5, 1}, {5, 0}, {20, 1}, {20, 0}};
    int i;
    assert(Ta_IsVirus_Chk2(0) == 0 && Ta_IsVirus_Chk3(0) == 0);
    item_query_result = 9999;
    for (i = 0; i < 5; i++) {
        set_item(rejected[i][0], rejected[i][1]);
        assert(Ta_IsVirus_Chk2(&item_fixture) == 0 && Ta_IsVirus_Chk3(&item_fixture) == 0);
        assert(order_count == 0 && item_query_argument == 0);
    }
    set_item(19, 0xff);
    clamp_bias = 460; /* 1500 */
    assert(Ta_IsVirus_Chk2(&item_fixture) == 1 && order_count == 3);
    assert(extra_item == &item_fixture && extra_args[0] == 7 && extra_args[1] == 9 && extra_args[2] == 1);
    set_item(21, 1);
    clamp_bias = 459; /* 1499 */
    assert(Ta_IsVirus_Chk2(&item_fixture) == 0 && order_count == 3);
    set_item(0, 1);
    clamp_bias = -100000;
    assert(Ta_IsVirus_Chk2(&item_fixture) == 0);
    clamp_bias = 100000;

    set_item(19, 0xff);
    item_query_result = 1500;
    assert(Ta_IsVirus_Chk3(&item_fixture) == 1 && item_query_argument == &item_fixture && order_count == 0);
    item_query_result = 1499;
    assert(Ta_IsVirus_Chk3(&item_fixture) == 0);
    item_query_result = INT_MIN;
    assert(Ta_IsVirus_Chk3(&item_fixture) == 0);
    item_query_result = INT_MAX;
    assert(Ta_IsVirus_Chk3(&item_fixture) == 1);
    memset(&item_fixture, 0, sizeof(item_fixture));
}

static int side_calls, side_args[2][4], append_calls, append_args[2][2], release_calls, item_word;

int SzData_CalcExp(int side, int id, int operation, int value)
{
    assert(side_calls < 2);
    side_args[side_calls][0] = side;
    side_args[side_calls][1] = id;
    side_args[side_calls][2] = operation;
    side_args[side_calls][3] = value;
    side_calls++;
    return 1;
}

static int set_mode, set_word, set_found[4], set_calls;
int SzData_CheckDeck(int side, int list, int value)
{
    assert(set_mode && side == 2 && list == 0x102 && value >= 54 && value <= 57);
    assert(set_calls == value - 54);
    set_calls++;
    return set_found[value - 54];
}

int SzDuel_GetUnitCardNo(int a, int b)
{
    if (set_mode) {
        assert(a == 2 && b == 0);
        return set_word;
    }
    assert(a == 3 && b == 4 && release_calls == 0);
    return item_word;
}

int SzDuel_AddGrave(int index, int value)
{
    assert(append_calls < 2);
    append_args[append_calls][0] = index;
    append_args[append_calls][1] = value;
    append_calls++;
    return 0;
}

int SzDuel_DelUnit(int a, int b)
{
    assert(a == 3 && b == 4 && side_calls == 1 && append_calls == 1);
    release_calls++;
    return 1;
}

static int release(void)
{
    side_calls = append_calls = release_calls = order_count = slot_calls = 0;
    return Ta_DelUnit(3, 4);
}

/* 0x24f370 over the real 0x250740: the first pair of calls always happens;
 * the second only for a slot-4 entry whose first word has kind 81. */
static void check_release(void)
{
    memset(slot_present, 0, sizeof(slot_present));
    item_word = 321;
    assert(release() == 0 && side_calls == 1 && append_calls == 1 && release_calls == 1);
    assert(side_args[0][0] == 3 && side_args[0][1] == 321 && side_args[0][2] == 0 && side_args[0][3] == -4);
    assert(append_args[0][0] == 3 && append_args[0][1] == 321 && slot_calls == 1);

    slot_present[4] = 1;
    slot_entries[4].unknown_words[0] = (81 << 8) | 0x7f;
    slot_entries[4].unknown_words[1] = (int)0xfeab1234u;
    assert(release() == 0 && side_calls == 2 && append_calls == 2 && release_calls == 1);
    assert(append_args[1][0] == 0xfe && append_args[1][1] == 0x1234);
    assert(side_args[1][0] == 0xfe && side_args[1][1] == 0x1234 && side_args[1][2] == 0 && side_args[1][3] == -4);

    slot_entries[4].unknown_words[0] = (80 << 8) | 0x7f;
    assert(release() == 0 && side_calls == 1 && append_calls == 1);
    slot_entries[4].unknown_words[0] = (int)0x80005100u; /* unsigned shift: not kind 81 */
    assert(release() == 0 && side_calls == 1 && append_calls == 1);
    lookup_result = 0; /* 0x250740 rejects a missing item */
    slot_entries[4].unknown_words[0] = 81 << 8;
    assert(release() == 0 && side_calls == 1 && append_calls == 1 && release_calls == 1);
    lookup_result = &item_fixture;
    slot_present[4] = 0;
}

/* 0x250000: position bound (count < position only), the 999 rejection and
 * the three modes on both sides of 683. */
static void check_list_classes(void)
{
    static const int expected[4][5] = {{1, 1, 0, 0, 0}, {1, 0, 1, 0, 0}, {0, 0, 0, 0, 0}, {1, 1, 0, 0, 0}};
    static const int modes[5] = {0, 1, 2, 3, -1};
    int position, m;
    list_count = 3;
    list_ids[0] = 682;
    list_ids[1] = 683;
    list_ids[2] = 999;
    list_ids[3] = 5; /* position equal to the count is still read */
    for (position = 0; position < 4; position++) {
        for (m = 0; m < 5; m++) {
            assert(Ta_IsGraveEntryHit(9, position, modes[m]) == expected[position][m]);
        }
    }
    for (m = 0; m < 5; m++) {
        assert(Ta_IsGraveEntryHit(9, 4, modes[m]) == 0); /* the list stub rejects this read */
    }
    list_count = 0;
}

static int gate_result, gate_calls, cell_kind, cell_calls, limit_value, limit_seen;
static struct DotrStateItem *gate_item;

int func_00250900_item(struct DotrStateItem *item)
{
    gate_item = item;
    gate_calls++;
    return gate_result;
}

/* scan_mode (0x143900): cell (0, i) answers 40 + i, asked for i = 1, 2, ... in order. */
static int scan_mode, scan_calls;

int SzDuel_GetField(int x, int y)
{
    if (scan_mode) {
        assert(x == 0 && y == scan_calls + 1 && y <= 5);
        scan_calls++;
        return 40 + y;
    }
    assert(x == 7 && y == 9);
    cell_calls++;
    return cell_kind;
}

static int limit_expected = 1500;
int GetCap(int limit, int value)
{
    assert(limit == limit_expected);
    limit_seen = value;
    return limit_value;
}

static int difference(int x, int y)
{
    order_count = gate_calls = cell_calls = 0;
    limit_seen = -1;
    return TaLogic_IsUnitWin3(&item_fixture, 25, x, y);
}

/* 0x278420 over the real TaUtil_GetUnitAP2 (40 + 1000 + bias) and 0x250bc0
 * (50 + 1000 + bias). The cell stub accepts only (7, 9). */
static void check_item_difference(void)
{
    gate_calls = cell_calls = 0;
    assert(TaLogic_IsUnitWin3(0, 25, 7, 9) == 0 && gate_calls == 0 && cell_calls == 0);
    set_item(0, 0);
    cell_kind = 3;
    gate_result = 1;
    assert(difference(7, 9) == -25 && gate_item == &item_fixture && gate_calls == 1 && order_count == 0);
    gate_result = 0;
    assert(difference(7, 9) == 101040 - 25 && extra_args[2] == 1 && cell_calls == 1 && limit_seen == -1);
    item_fixture.status.unknown_bit_3 = -1;
    assert(difference(7, 9) == 101050 - 25 && extra_args[2] == 0 && extra_args[0] == 7 && extra_args[1] == 9);
    item_fixture.locRow = 8; /* position differs: the first query again */
    assert(difference(7, 9) == 101040 - 25 && extra_args[2] == 1);
    item_fixture.locRow = 9;
    item_fixture.locCol = 6;
    assert(difference(7, 9) == 101040 - 25 && extra_args[2] == 1);
    item_fixture.locCol = 7;
    item_fixture.status.unknown_bit_3 = 0;
    item_fixture.status.unknown_bit_2 = -1;
    item_fixture.status.unknown_bit_4 = -1;
    assert(difference(7, 9) == 101040 - 25 && extra_args[2] == 1);
    cell_kind = 9;
    limit_value = 1000;
    assert(difference(7, 9) == 101040 - 1000 && limit_seen == 25);
    gate_result = 1;
    assert(difference(7, 9) == -1000 && limit_seen == 25 && order_count == 0);
    gate_result = 2; /* only a result of exactly 1 selects zero */
    cell_kind = 0;
    assert(difference(7, 9) == 101040 - 25);
    memset(&item_fixture, 0, sizeof(item_fixture));
}

static int copy_calls, free_calls, free_result;
static unsigned char *copy_source, *copy_destination;

void TaLogic_CopyArray(unsigned char *source, unsigned char *destination)
{
    assert(free_calls == 0);
    copy_source = source;
    copy_destination = destination;
    memcpy(destination, source, 5);
    copy_calls++;
}

static int pick_mode, pick_len, pick_len_calls, pick_clear[5], pick_probe_calls, pick_match[8];
static int pick_marks[4], pick_mark_calls;
static unsigned char pick_seen[5], *pick_bytes;
int TaLogic_IsUseHandInCombo(unsigned char value, unsigned char *p)
{
    assert(pick_mode && p == pick_bytes && value < 5);
    pick_probe_calls++;
    return pick_clear[value] ? 0x100 : 1;
}
int TaLogic_GetHandFusion(unsigned short a, unsigned short b)
{
    assert(pick_mode && a == 50 && b >= 100 && b < 108);
    return pick_match[b - 100];
}
void TaLogic_AddEmptyCombo(unsigned char *bytes, int value)
{
    assert(pick_mode && bytes == pick_bytes && pick_mark_calls < 4);
    pick_marks[pick_mark_calls++] = value;
}
int TaLogic_GetArrayLength(unsigned char *bytes)
{
    if (pick_mode) {
        if (pick_len_calls++ == 0) memcpy(pick_seen, bytes, 5);
        return pick_len + 0x100;
    }
    assert(copy_calls == 1 && bytes == copy_source);
    free_calls++;
    return free_result;
}

static void set_score(int id, int score)
{
    ((unsigned short *)&node_entries[id - 100])[4] = (unsigned short)score;
    ((unsigned short *)&node_entries[id - 100])[5] = 0xffff; /* low half is ignored */
}

/* 0x278930 and 0x278e40. The node view holds host pointers, so only its
 * leading offsets equal the target's. */
static void check_nodes(void)
{
    static const unsigned char filled[5] = {255, 255, 255, 255, 255};
    unsigned char source[5] = {1, 2, 255, 255, 255};
    struct DotrStateNode node, n[6];
    int i;
    assert(offsetof(struct DotrStateNode, combo) == 2 && offsetof(struct DotrStateNode, children) == 8);

    memset(&node, 0x5a, sizeof(node));
    copy_calls = free_calls = 0;
    TaLogic_InitTree(&node, 0xabcd, 0, 0x177);
    assert(node.result == 0xabcd && node.combo[0] == 0x77 && memcmp(node.combo + 1, filled, 4) == 0);
    for (i = 0; i < 4; i++) {
        assert(node.children[i] == 0);
    }
    assert(copy_calls == 0 && free_calls == 0);

    memset(&node, 0x5a, sizeof(node));
    free_result = 0x102; /* narrowed to byte index 2 */
    TaLogic_InitTree(&node, 7, source, 0x33);
    assert(copy_calls == 1 && free_calls == 1 && copy_destination == node.combo);
    assert(node.result == 7 && node.combo[0] == 1 && node.combo[1] == 2 && node.combo[2] == 0x33);
    assert(node.combo[3] == 255 && node.combo[4] == 255 && node.children[3] == 0);

    /* Tree: 100(5) -> 101(9) -> 104(12); 100 -> 102(9) -> 105(12); 100 -> 103(7).
     * The first strictly larger score wins, so 104 beats the equal 105. */
    memset(n, 0, sizeof(n));
    for (i = 0; i < 6; i++) {
        n[i].result = (unsigned short)(100 + i);
    }
    set_score(100, 5);
    set_score(101, 9);
    set_score(102, 9);
    set_score(103, 7);
    set_score(104, 12);
    set_score(105, 12);
    n[0].children[0] = &n[1];
    n[0].children[2] = &n[2];
    n[0].children[3] = &n[3];
    n[1].children[3] = &n[4];
    n[2].children[0] = &n[5];
    assert(TaLogic_GetFusionRes(0) == 0);
    assert(TaLogic_GetFusionRes(&n[3]) == &n[3] && TaLogic_GetFusionRes(&n[1]) == &n[4]);
    assert(TaLogic_GetFusionRes(&n[0]) == &n[4]);
    set_score(105, 13);
    assert(TaLogic_GetFusionRes(&n[0]) == &n[5]);
    set_score(105, 0x2000 | 11); /* the score is masked to 13 bits */
    assert(TaLogic_GetFusionRes(&n[0]) == &n[4]);
    set_score(100, 12);
    assert(TaLogic_GetFusionRes(&n[0]) == &n[0]);
    n[0].result = 999; /* id 999 scores zero */
    assert(TaLogic_GetFusionRes(&n[0]) == &n[4]);
}

static int next_result = 0x3355;
static int bit_result, bit_args[3], next_calls, next_a, side_result, side_queries, flags_result, flags_index;
static struct DotrStateItem *next_item;

int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    if (query_mode) {
        return (q_bits[a][b] >> bit) & 1;
    }
    bit_args[0] = a;
    bit_args[1] = b;
    bit_args[2] = bit;
    return bit_result;
}

int Do_CheckLandAgreement(int a, struct DotrStateItem *item)
{
    next_a = a;
    next_item = item;
    next_calls++;
    if (scan_mode) {
        return a == scan_mode ? next_result : 0;
    }
    return next_result;
}

static int wide_high, wide_low, wide_calls, wide_side, wide_last;

int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    assert(a == 0 && b == 0 && d == 1 && f == 14 && (e == 0x2001c || e == 0x1c));
    wide_side = c;
    wide_last = e;
    wide_calls++;
    return e == 0x2001c ? wide_high : wide_low;
}

static void check_wide_queries(void)
{
    int saved = side_result;

    /* 0x117600: the first call decides when non-zero, else the second */
    wide_high = 4; wide_low = 0; wide_calls = 0;
    assert(TaLogic001_IsUseLordOfDragon(6) == 1 && wide_calls == 1 && wide_side == 6 && wide_last == 0x2001c);
    wide_high = 0; wide_low = 9; wide_calls = 0;
    assert(TaLogic001_IsUseLordOfDragon(6) == 1 && wide_calls == 2 && wide_side == 6 && wide_last == 0x1c);
    wide_low = 0; wide_calls = 0;
    assert(TaLogic001_IsUseLordOfDragon(6) == 0 && wide_calls == 2);

    /* 0x117340 through the real 0x27dd20: id 2, then id 0x345, then 0x24ca40 */
    side_result = 1;
    pair_count[0] = 1; pair_count[1] = 1;
    pair_ids[0][0] = 2; pair_ids[1][0] = 5;
    side_queries = 0; wide_calls = 0;
    assert(TaLogic001_IsActiveMode() == 1 && side_queries == 1 && wide_calls == 0);
    pair_ids[0][0] = 5; pair_ids[1][0] = 0x345;
    side_queries = 0;
    assert(TaLogic001_IsActiveMode() == 1 && side_queries == 2 && wide_calls == 0);
    pair_ids[1][0] = 5;
    side_queries = 0; wide_high = 1;
    assert(TaLogic001_IsActiveMode() == 1 && side_queries == 3 && wide_calls == 1 && wide_side == 1 && wide_last == 0x2001c);
    side_queries = 0; wide_high = 0; wide_low = 1; wide_calls = 0;
    assert(TaLogic001_IsActiveMode() == 0 && side_queries == 3 && wide_calls == 1);
    side_result = saved;
}

/* 0x143900 through the real 0x24deb0: the 0x24df80 stub answers next_result
 * for the cell value scan_mode and 0 for the others. */
static void check_cell_scan(void)
{
    int saved = next_result;

    set_item(0x1f, 0x80);
    next_result = 1;
    scan_mode = 41; scan_calls = 0; next_calls = 0;
    assert(TaLogic012_GetAgreeLineZ(&item_fixture) == 1 && scan_calls == 1 && next_item == &item_fixture);
    scan_mode = 45; scan_calls = 0;
    assert(TaLogic012_GetAgreeLineZ(&item_fixture) == 5 && scan_calls == 5);
    /* no cell answers, and an answer other than 1 is not accepted */
    scan_mode = 46; scan_calls = 0;
    assert(TaLogic012_GetAgreeLineZ(&item_fixture) == -1 && scan_calls == 5);
    next_result = 2;
    scan_mode = 43; scan_calls = 0;
    assert(TaLogic012_GetAgreeLineZ(&item_fixture) == -1 && scan_calls == 5);
    scan_mode = 0;
    next_result = saved;
}

int SzDuel_GetTurnOwner(void)
{
    side_queries++;
    return side_result;
}

int SzDuel_InqDestinyFlag(int index)
{
    flags_index = index;
    return flags_result;
}

/* 0x24de70, 0x24deb0, 0x24df00, 0x26fc10, 0x26f600 and 0x24f430. */
static void check_gates_and_presets(void)
{
    static const int inputs[3] = {0, 1, 5}, flipped[3] = {1, 0, 0};
    unsigned char *bytes = (unsigned char *)&node_entries[6];
    int out[5], i;

    for (i = 0; i < 3; i++) {
        bit_result = 0;
        assert(Ta_GetCurrentSide(inputs[i], 9) == inputs[i]);
        assert(bit_args[0] == inputs[i] && bit_args[1] == 9 && bit_args[2] == 1);
        bit_result = 1;
        assert(Ta_GetCurrentSide(inputs[i], 9) == flipped[i]);
        bit_result = -8;
        assert(Ta_GetCurrentSide(inputs[i], 9) == flipped[i]);
    }

    next_calls = 0;
    set_item(0x20 | 5, 1);
    assert(CheckLandAgreement(6, &item_fixture) == 0);
    set_item(0x80, 1);
    assert(CheckLandAgreement(6, &item_fixture) == 0);
    set_item(0x1f, 0);
    assert(CheckLandAgreement(6, &item_fixture) == 0 && next_calls == 0);
    set_item(0x1f, 0x80);
    assert(CheckLandAgreement(6, &item_fixture) == 0x3355 && next_calls == 1 && next_a == 6 && next_item == &item_fixture);

    next_calls = order_count = 0;
    range_result = 0x20;
    assert(CheckLandAgreement2(6, 3, 4) == 0);
    range_result = -1;
    assert(CheckLandAgreement2(6, 3, 4) == 0);
    range_result = 0x1f;
    assert(CheckLandAgreement2(6, 3, 0) == 0 && next_calls == 0 && order_count == 0);
    assert(CheckLandAgreement2(6, 3, 4) == 0x3355 && next_calls == 1 && order_count == 1);
    assert(next_a == 6 && next_item == &item_fixture);
    lookup_result = 0; /* a missing item is forwarded as null */
    order_count = 0;
    assert(CheckLandAgreement2(7, 3, 4) == 0x3355 && next_a == 7 && next_item == 0);
    lookup_result = &item_fixture;
    range_result = 0;

    memset(&node_entries[6], 0, sizeof(node_entries[6]));
    bytes[0] = 0x1f;
    ((unsigned short *)&node_entries[6])[2] = 0xffff;
    assert(TaHand_IsCardEffect(106) == 2 && TaHand_IsCardEffect(999) == 0 && TaHand_IsCardEffect(41) == 0);
    ((unsigned short *)&node_entries[6])[2] = 0xfffe;
    assert(TaHand_IsCardEffect(106) == 1);
    bytes[0] = 0x20;
    assert(TaHand_IsCardEffect(106) == 0);
    ((unsigned short *)&node_entries[6])[2] = 0xffff;
    assert(TaHand_IsCardEffect(106) == 0);

    /* 0x26f600 stores through the pointer before its null test, so a null
     * output is not exercised. 0x1df110 is stubbed as counts[0] + index. */
    side_result = 0x77;
    side_queries = 0;
    counts[0] = 2;
    flags_result = 0x7f;
    out[4] = 0x5a5a;
    assert(Ta_SetDestinyEffect(2, out) == 1 && flags_index == 2 && side_queries == 1);
    assert(out[0] == 114 && out[1] == 0x77 && out[2] == 0 && out[3] == 0 && out[4] == 0x5a5a);
    flags_result = 0x80;
    assert(Ta_SetDestinyEffect(2, out) == 0 && out[0] == 999 && out[1] == 0 && out[2] == 0 && out[3] == 0);
    flags_result = 0x100;
    counts[0] = 3; /* count 5 */
    assert(Ta_SetDestinyEffect(2, out) == 0 && out[0] == 999 && side_queries == 1);
    counts[0] = 0;

    /* 0x24f430: the side and list calls use 0x1de540's result, not `a`. */
    memset(slot_present, 0, sizeof(slot_present));
    item_word = 222;
    side_result = 1;
    side_calls = append_calls = release_calls = order_count = slot_calls = side_queries = 0;
    assert(Ta_DelUnit2(3, 4) == 0 && side_calls == 1 && append_calls == 1 && release_calls == 1);
    assert(side_queries == 2 && side_args[0][0] == 1 && side_args[0][1] == 222 && side_args[0][3] == -4);
    assert(append_args[0][0] == 1 && append_args[0][1] == 222);
    slot_present[4] = 1;
    slot_entries[4].unknown_words[0] = 81 << 8;
    slot_entries[4].unknown_words[1] = (int)0x02005678u;
    side_calls = append_calls = release_calls = order_count = slot_calls = 0;
    assert(Ta_DelUnit2(3, 4) == 0 && side_calls == 2 && append_calls == 2);
    assert(append_args[1][0] == 2 && append_args[1][1] == 0x5678 && side_args[1][0] == 2 && side_args[1][1] == 0x5678);
    slot_present[4] = 0;
}

static int vector_packed;

int func_001f6160_words(int packed, int *words)
{
    vector_packed = packed;
    words[0] = 10;
    words[1] = 20;
    words[2] = 30;
    words[3] = 40;
    return 0;
}

/* 0x2612e0, 0x261380 and 0x261420: base constant and third-word increment
 * differ; the difference wraps by 0xffff when the item word exceeds the base.
 * A missing item is dereferenced by the target and is not exercised. */
static void check_wrapped_offsets(void)
{
    static void (*const routines[3])(int *, int, int) = {TaView_SetModelFrontView, TaView_SetExzModelFrontView, TaView_SetModelBackView};
    static const int bases[3] = {0x47ff, 0x3fff, 0xc7ff}, steps[3] = {8192, 4096, 8192};
    int out[5], i;
    for (i = 0; i < 3; i++) {
        set_item(0, 0);
        out[4] = 0x5a5a;
        item_fixture.unknown_30 = 0;
        routines[i](out, 0x304, -7);
        assert(vector_packed == 0x304 && order_count == 1);
        assert(out[0] == 3 && out[1] == 20 + bases[i] && out[2] == 30 + steps[i] && out[3] == 40 && out[4] == 0x5a5a);
        item_fixture.unknown_30 = bases[i];
        routines[i](out, 0x304, 100);
        assert(out[0] == 110 && out[1] == 20);
        item_fixture.unknown_30 = bases[i] + 1;
        routines[i](out, 0x304, 0);
        assert(out[0] == 10 && out[1] == 20 + 0xfffe && out[2] == 30 + steps[i]);
        item_fixture.unknown_30 = 0xffff;
        routines[i](out, 0x304, 0);
        assert(out[1] == 20 + bases[i]);
        item_fixture.unknown_30 = -5;
        routines[i](out, 0x304, 0);
        assert(out[1] == 20 + bases[i] + 5);
    }
    memset(&item_fixture, 0, sizeof(item_fixture));

    /* 0x249630 through the real 0x250740 on slot 1. */
    memset(slot_present, 0, sizeof(slot_present));
    order_count = 0;
    assert(WallFusion(3, 4) == -1);
    slot_present[1] = 1;
    slot_entries[1].unknown_words[0] = (9 << 8) | 0xee;
    slot_entries[1].unknown_words[1] = 0;
    *(unsigned short *)&slot_entries[1].unknown_words[1] = 0xfedc;
    order_count = 0;
    assert(WallFusion(3, 4) == 0xfedc);
    slot_entries[1].unknown_words[0] = (10 << 8) | 0xee;
    order_count = 0;
    assert(WallFusion(3, 4) == -1);
    slot_entries[1].unknown_words[0] = (int)(0x80000000u | (9 << 8)); /* unsigned kind */
    order_count = 0;
    assert(WallFusion(3, 4) == -1);
    slot_present[1] = 0;
}

static void set_query(int side, int i, int present, int bits, int kind)
{
    q_present[side][i] = present;
    q_bits[side][i] = bits;
    q_kind[side][i] = kind;
    q_entries[side][i].unknown_words[0] = kind < 0 ? 0 : kind << 8;
    q_entries[side][i].unknown_words[1] = 0;
}

static void check_kind_queries(void)
{
    const int good = (1 << 3) | (1 << 4) | (1 << 5);
    int s, i, bit;
    query_mode = 1;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    /* 0x266c70: no match, then n-th match across side 0 and side 1. */
    assert(Ta_SearchUncTypeNaturalEffectUser(7, 0) == -1);
    set_query(0, 2, 1, good, 7);
    set_query(0, 5, 1, good, 7);
    set_query(0, 6, 1, good, 8);
    set_query(1, 3, 1, good, 7);
    assert(Ta_SearchUncTypeNaturalEffectUser(7, 0) == 2 && Ta_SearchUncTypeNaturalEffectUser(7, 1) == 5);
    assert(Ta_SearchUncTypeNaturalEffectUser(7, 2) == (3 | 0x100) && Ta_SearchUncTypeNaturalEffectUser(7, 3) == -1);
    assert(Ta_SearchUncTypeNaturalEffectUser(8, 0) == 6 && Ta_SearchUncTypeNaturalEffectUser(9, 0) == -1);
    for (bit = 3; bit <= 5; bit++) { /* each missing bit rejects slot 2 */
        set_query(0, 2, 1, good & ~(1 << bit), 7);
        assert(Ta_SearchUncTypeNaturalEffectUser(7, 0) == 5);
    }
    set_query(0, 2, 1, good, -1); /* null entry */
    assert(Ta_SearchUncTypeNaturalEffectUser(7, 0) == 5);
    set_query(0, 2, 1, good, 7);
    /* 0x266df0: counts both sides; skips on each failing test. */
    assert(Ta_SearchUncTypeNaturalEffectUserNum(7) == 3 && Ta_SearchUncTypeNaturalEffectUserNum(8) == 1 && Ta_SearchUncTypeNaturalEffectUserNum(9) == 0);
    set_query(0, 2, 0, good, 7); /* presence word false */
    assert(Ta_SearchUncTypeNaturalEffectUserNum(7) == 2);
    set_query(0, 2, 1, good, -1); /* null entry */
    assert(Ta_SearchUncTypeNaturalEffectUserNum(7) == 2);
    for (bit = 3; bit <= 5; bit++) {
        set_query(0, 2, 1, good & ~(1 << bit), 7);
        assert(Ta_SearchUncTypeNaturalEffectUserNum(7) == 2);
    }
    set_query(0, 2, 1, good, 7);
    assert(Ta_SearchUncTypeNaturalEffectUserNum(7) == 3);
    /* 0x266f70: mode is compared with 0x24de70, i.e. the side, flipped when grid bit 1 is set. */
    assert(Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 0) == 2 && Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 1) == 1 && Ta_SearchUncTypeNaturalEffectUserNumWithSide(8, 0) == 1);
    set_query(0, 5, 1, good | (1 << 1), 7); /* de70(0, 5) == 1 */
    assert(Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 0) == 1 && Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 1) == 2);
    set_query(1, 3, 1, good | (1 << 1), 7); /* de70(1, 3) == 0 */
    assert(Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 0) == 2 && Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 1) == 1);
    for (bit = 3; bit <= 5; bit++) {
        set_query(0, 2, 1, good & ~(1 << bit), 7);
        assert(Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 0) == 1);
    }
    set_query(0, 2, 1, good, -1);
    assert(Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 0) == 1);
    for (s = 0; s < 2; s++) for (i = 0; i < 11; i++) set_query(s, i, 0, 0, -1);
    assert(Ta_SearchUncTypeNaturalEffectUserNumWithSide(7, 0) == 0);
    query_mode = 0;
}

static void check_list_counts(void)
{
    /* Branch coverage: every mode and flag outcome of both counters. */
    static const int ids[5] = {40, 40, 2, 1, 3};
    int i;
    memset(&entry, 0, sizeof(entry));
    ((unsigned char *)&entry)[0] = 0xc7;
    for (i = 0; i < 5; i++) {
        list_ids[i] = ids[i];
    }
    list_count = 5;
    /* 0x27dbc0: id 40 is class 6, the rest class 0. */
    assert(TaLogic_GetGraveEntryAfterScreen(9, 3, 31) == 3);
    assert(TaLogic_GetGraveEntryAfterScreen(9, 3, 29) == 1 && TaLogic_GetGraveEntryAfterScreen(9, 3, 30) == 2);
    assert(TaLogic_GetGraveEntryAfterScreen(9, 0, 29) == 0 && TaLogic_GetGraveEntryAfterScreen(9, 0, 30) == 0);
    assert(TaLogic_GetGraveEntryAfterScreen(9, 5, 29) == 5 && TaLogic_GetGraveEntryAfterScreen(9, 0x105, 30) == 5);
    assert(TaLogic_GetGraveEntryAfterScreen(9, 3, 7) == 3 && TaLogic_GetGraveEntryAfterScreen(9, 0x103, 0x11d) == 1);
    list_count = -1;
    assert(TaLogic_GetGraveEntryAfterScreen(9, 2, 29) == 2);
    list_count = 5;

    /* 0x27dd20: lists 0 and 1 are distinct. */
    pair_count[0] = 3;
    pair_count[1] = 2;
    pair_ids[0][0] = 7; pair_ids[0][1] = 8; pair_ids[0][2] = 7;
    pair_ids[1][0] = 7; pair_ids[1][1] = 9;
    assert(TaLogic_GetGraveCardCount(0, 0, 7) == 0);
    assert(TaLogic_GetGraveCardCount(0, 1, 7) == 2 && TaLogic_GetGraveCardCount(1, 1, 7) == 1);
    assert(TaLogic_GetGraveCardCount(0, 2, 7) == 1 && TaLogic_GetGraveCardCount(1, 2, 7) == 2);
    assert(TaLogic_GetGraveCardCount(0, 3, 7) == 3 && TaLogic_GetGraveCardCount(1, 3, 7) == 3);
    assert(TaLogic_GetGraveCardCount(0, 3, 9) == 1 && TaLogic_GetGraveCardCount(0, 3, 5) == 0);
    pair_count[0] = 0;
    assert(TaLogic_GetGraveCardCount(0, 1, 7) == 0);
}

static void set_value(int side, int i, int type, int value)
{
    q_entries[side][i].unknown_words[1] = 0;
    *(unsigned short *)&q_entries[side][i].unknown_words[1] = value;
    ((unsigned short *)&q_entries[side][i].unknown_words[1])[1] = type;
}

static void check_kind_sum(void)
{
    const int good = (1 << 3) | (1 << 4) | (1 << 5);

    query_mode = 1;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    assert(Ta_GetEnchantOfs() == 0);
    set_query(0, 2, 1, good, 52);
    set_value(0, 2, 0, 100);
    assert(Ta_GetEnchantOfs() == 100);
    set_query(0, 4, 1, good, 52);
    set_value(0, 4, 1, 30);
    assert(Ta_GetEnchantOfs() == 70);
    set_query(1, 3, 1, good, 52);
    set_value(1, 3, 0, 0xffff);
    assert(Ta_GetEnchantOfs() == 70 + 0xffff);
    set_query(0, 6, 1, good, 52);
    set_value(0, 6, 2, 999);
    set_query(0, 7, 1, good, 51);
    set_value(0, 7, 0, 999);
    assert(Ta_GetEnchantOfs() == 70 + 0xffff);
    set_query(1, 9, 1, good, 52);
    set_value(1, 9, 1, 0xffff);
    assert(Ta_GetEnchantOfs() == 70);
    memset(q_present, 0, sizeof(q_present));
    assert(Ta_GetEnchantOfs() == 0);
}

static void set_vote(int side, int i, int mode, int flip)
{
    set_query(side, i, 1, (1 << 3) | (1 << 4) | (1 << 5) | (flip << 1), 86);
    q_entries[side][i].unknown_words[0] |= mode << 6;
}

static void check_vote(void)
{
    query_mode = 1;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    assert(Ta_GetSummonCalcRevision(0) == 0 && Ta_GetSummonCalcRevision(1) == 0);
    /* mode 3 counts +1 whatever who is; modes 0 and 1 count nothing */
    set_vote(0, 2, 3, 0);
    assert(Ta_GetSummonCalcRevision(0) == 1 && Ta_GetSummonCalcRevision(1) == 1);
    set_vote(0, 2, 0, 0);
    set_vote(0, 4, 1, 0);
    assert(Ta_GetSummonCalcRevision(0) == 0);
    /* mode 2 counts -1 unless the slot's id (side, inverted by grid bit 1) equals who */
    set_vote(0, 2, 2, 0);
    assert(Ta_GetSummonCalcRevision(0) == 0 && Ta_GetSummonCalcRevision(1) == -1);
    set_vote(0, 2, 2, 1);
    assert(Ta_GetSummonCalcRevision(0) == -1 && Ta_GetSummonCalcRevision(1) == 0);
    set_vote(1, 3, 2, 0);
    assert(Ta_GetSummonCalcRevision(1) == 0 && Ta_GetSummonCalcRevision(0) == -1);
    /* totals are reduced to their sign: +2 -> 1, -3 -> -1, +1 -1 -> 0 */
    memset(q_present, 0, sizeof(q_present));
    set_vote(0, 2, 3, 0);
    set_vote(0, 4, 3, 0);
    assert(Ta_GetSummonCalcRevision(0) == 1);
    set_vote(0, 6, 2, 0);
    assert(Ta_GetSummonCalcRevision(1) == 1 && Ta_GetSummonCalcRevision(0) == 1);
    memset(q_present, 0, sizeof(q_present));
    set_vote(0, 2, 2, 0);
    set_vote(0, 4, 2, 0);
    set_vote(0, 6, 2, 0);
    assert(Ta_GetSummonCalcRevision(1) == -1 && Ta_GetSummonCalcRevision(0) == 0);
    memset(q_present, 0, sizeof(q_present));
    set_vote(0, 2, 3, 0);
    set_vote(0, 4, 2, 0);
    assert(Ta_GetSummonCalcRevision(1) == 0);
}

/* Makes node entry 100 + k eligible (state 0) or not, with the given score. */
static void set_candidate(int side, int i, int k, int eligible, int score)
{
    unsigned char *bytes = (unsigned char *)&node_entries[k];

    memset(bytes, 0, sizeof(node_entries[k]));
    bytes[0] = eligible ? 0x1f : 0x20;
    ((unsigned short *)bytes)[4] = score;
    pair_ids[side][i] = 100 + k;
}

static void check_best_candidate(void)
{
    pair_count[0] = 4;
    pair_count[1] = 3;
    set_candidate(0, 0, 0, 1, 5);
    set_candidate(0, 1, 1, 0, 9);
    set_candidate(0, 2, 2, 1, 7);
    set_candidate(0, 3, 3, 1, 7);
    set_candidate(1, 0, 4, 1, 7);
    set_candidate(1, 1, 5, 1, 8);
    set_candidate(1, 2, 6, 1, 8);
    /* mask without bits: nothing scanned */
    assert(TaLogic_GetMaxAPGraveCard(0, 0) == -1);
    /* own side only: best eligible score, first one wins ties, ineligible skipped */
    assert(TaLogic_GetMaxAPGraveCard(0, 1) == ((0 << 24) | (2 << 16) | 7));
    assert(TaLogic_GetMaxAPGraveCard(1, 1) == ((1 << 24) | (1 << 16) | 8));
    /* other side only, from either bit pattern with mask >> 1 non-zero */
    assert(TaLogic_GetMaxAPGraveCard(0, 2) == ((1 << 24) | (1 << 16) | 8));
    assert(TaLogic_GetMaxAPGraveCard(0, 4) == ((1 << 24) | (1 << 16) | 8));
    assert(TaLogic_GetMaxAPGraveCard(1, 2) == ((0 << 24) | (2 << 16) | 7));
    /* both: the other side wins only with a strictly greater score */
    assert(TaLogic_GetMaxAPGraveCard(0, 3) == ((1 << 24) | (1 << 16) | 8));
    assert(TaLogic_GetMaxAPGraveCard(1, 3) == ((1 << 24) | (1 << 16) | 8));
    set_candidate(1, 1, 5, 1, 7);
    set_candidate(1, 2, 6, 1, 6);
    assert(TaLogic_GetMaxAPGraveCard(0, 3) == ((0 << 24) | (2 << 16) | 7));
    /* an eligible score of 0 beats the initial -1; ids of 999 read state and score 0 */
    set_candidate(0, 0, 0, 1, 0);
    pair_count[0] = 1;
    assert(TaLogic_GetMaxAPGraveCard(0, 1) == ((0 << 24) | (0 << 16) | 0));
    pair_ids[0][0] = 999;
    assert(TaLogic_GetMaxAPGraveCard(0, 1) == 0);
    /* nothing eligible, or empty lists */
    set_candidate(0, 0, 0, 0, 3);
    pair_count[1] = 2;
    set_candidate(1, 0, 4, 0, 3);
    set_candidate(1, 1, 5, 0, 3);
    assert(TaLogic_GetMaxAPGraveCard(0, 3) == -1);
    pair_count[0] = pair_count[1] = 0;
    assert(TaLogic_GetMaxAPGraveCard(0, 3) == -1);
}

static int f830_arg, f830_result, e80_calls, e80_result;
static struct DotrQueryObject1a2 *e80_arg;
static int f830_calls, f830_first;
int TaTuto_IsTuto(int value) { if (f830_calls++ == 0) f830_first = value; f830_arg = value; return f830_result; }
int TaLogic_DoAdjustLogicData(struct DotrQueryObject1a2 *p) { e80_calls++; e80_arg = p; return e80_result; }

static int gate_result, gate_args[3], tail_args[2], tail_calls;
int Do_ChkEncMatch(unsigned a, int b, int c)
{
    gate_args[0] = (int)a;
    gate_args[1] = b;
    gate_args[2] = c;
    return gate_result;
}
int Ta_GetHandEnchant3(int a, int b) { tail_args[0] = a; tail_args[1] = b; tail_calls++; return 0x9040; }

static int word_index, word_result, scale_args[2];
int SzDuel_GetLP(int index) { word_index = index; return word_result; }
int Ta_GetLPRevByNaturalEffect(int a, int b) { scale_args[0] = a; scale_args[1] = b; return 1000; }

static int draw_log[8], draw_count, draw_a0b0_arg;
static long draw_value;
void RhBase_FreeRenderFrame(void) { draw_log[draw_count++] = 1; }
DotrDrawWord RhBase_SetZBufferMode(DotrDrawWord cursor, int a) { draw_log[draw_count++] = 2; draw_a0b0_arg = a; return cursor + 16; }
long ExGetRegVal(int a, int b) { draw_log[draw_count++] = 3; return 0x100000000L * (a + 1) + b; }
struct DotrDrawPacket *ExClampMode_Pipe(struct DotrDrawPacket *packet, long value)
{
    draw_log[draw_count++] = 4;
    draw_value = value;
    return (struct DotrDrawPacket *)((char *)packet + 32);
}
void SzMsg_Free(int index) { draw_log[draw_count++] = 10 + index; }

static void check_solo_batch(void)
{
    static const int order[7] = {1, 2, 3, 4, 10, 11, 12};
    static char buffer[64];

    /* SzDuel_FinishCardSession_Pipe (straight-line coverage): call order, arguments, result. */
    draw_count = 0;
    assert(SzDuel_FinishCardSession_Pipe((DotrDrawWord)buffer) == (DotrDrawWord)(buffer + 48));
    assert(draw_count == 7 && memcmp(draw_log, order, sizeof(order)) == 0);
    assert(draw_a0b0_arg == 0 && draw_value == 0x100000004L);

    /* Ta_GetHandEnchant2 (branch coverage): gate zero and non-zero. */
    release_calls = 0;
    item_word = 0x4321;
    tail_calls = 0;
    gate_result = 0;
    assert(Ta_GetHandEnchant2(3, 4, 5) == -1 && tail_calls == 0);
    assert(gate_args[0] == 3 && gate_args[1] == 4 && gate_args[2] == 5);
    gate_result = 1;
    assert(Ta_GetHandEnchant2(3, 4, 5) == 0x9040 && tail_calls == 1);
    assert(tail_args[0] == 0x4321 && tail_args[1] == 5);

    /* Ta_GetForceEffection (branch coverage): a = -1, 0, 1 below the bound; a = 2 with
     * the side query zero and non-zero; the shift is arithmetic. */
    word_result = -7;
    assert(Ta_GetForceEffection(0, 5) == 1005 && word_index == 1);
    assert(scale_args[0] == 44 && scale_args[1] == -4);
    word_result = 9;
    assert(Ta_GetForceEffection(1, -5) == 995 && word_index == 0 && scale_args[1] == 4);
    assert(Ta_GetForceEffection(-1, 0) == 1000 && word_index == 0);
    side_result = 0;
    assert(Ta_GetForceEffection(2, 1) == 1001 && word_index == 1);
    side_result = 5;
    assert(Ta_GetForceEffection(2, 1) == 1001 && word_index == 0);
}

unsigned char EnchMatch[64];
static int step_arg_calls, step_args[4], step_value, reset_calls;
static void *step_object;
int func_0027fda0_ptr(void *p) { step_object = p; return step_value; }
void SzDuel_OpenMsgPopup(int operation, int a, int b, int value)
{
    step_args[0] = operation;
    step_args[1] = a;
    step_args[2] = b;
    step_args[3] = value;
    step_arg_calls++;
}
void SzDuel_CloseMsgPopup(void) { reset_calls++; }

static int wide_args[10], wide_calls;
int TaLogic_GetInvadeRoot2(int a, int b, int c, int d, int e, int f, int g, int h, unsigned char i, unsigned short j)
{
    int v[10] = {a, b, c, d, e, f, g, h, i, j};
    memcpy(wide_args, v, sizeof(v));
    wide_calls++;
    return 0xa130;
}

/* TaLogic_GetInvadeRoot (straight-line coverage): every forwarded argument and the result. */
static void check_wide_forward(void)
{
    static const int want[10] = {0x77, 3, 4, 5, 6, 7, 8, 9, 0xab, 0xcdef};
    q_present[1][2] = 0x77;
    wide_calls = 0;
    assert(TaLogic_GetInvadeRoot(1, 2, 3, 4, 5, 6, 7, 8, 9, 0xab, 0xcdef) == 0xa130);
    assert(wide_calls == 1 && memcmp(wide_args, want, sizeof(want)) == 0);
    q_present[1][2] = 0;
}

static int chain_result, chain_arg, chain_calls;
int Ta_GetLDLvRev(int a) { chain_arg = a; chain_calls++; return chain_result; }
static int pre_result, pre_args[3], pair_result, pair_args[2], pair_calls, kind_result, kind_calls;
int Ta_IsLDRange(int a, int b, int c) { pre_args[0] = a; pre_args[1] = b; pre_args[2] = c; return pre_result; }
int Ta_IsLDAbl(int a, int b) { pair_args[0] = a; pair_args[1] = b; pair_calls++; return pair_result; }
int SzDuel_GetUnitConstKind(int a, int b) { assert(a == 6 && b == 0); kind_calls++; return kind_result; }

static void check_solo_batch_3(void)
{
    /* Ta_GetSummonRev (branch coverage): missing entry, kind mismatch, upper bits
     * set, accepted. */
    chain_result = 0x1830;
    chain_calls = 0;
    range_result = 0x25;
    *(unsigned char *)&entry = 0x05;
    assert(Ta_GetSummonRev(3, 41) == 0 && chain_calls == 0);
    assert(Ta_GetSummonRev(3, 40) == 0x1830 && chain_calls == 1 && chain_arg == 3);
    range_result = 0x26;
    assert(Ta_GetSummonRev(3, 40) == 0);
    range_result = 0x05;
    *(unsigned char *)&entry = 0x25;
    assert(Ta_GetSummonRev(3, 40) == 0 && chain_calls == 1);

    /* Ta_IsLDRangeAvilityFromLoc (branch coverage): each rejection and the accepted case. */
    pair_calls = kind_calls = 0;
    pre_result = 0;
    pair_result = 1;
    kind_result = 0x47;
    assert(Ta_IsLDRangeAvilityFromLoc(0x1234, 6, 7, 8) == 0 && pair_calls == 0);
    assert(pre_args[0] == 6 && pre_args[1] == 0x34 && pre_args[2] == 0x12);
    pre_result = 1;
    pair_result = 0;
    assert(Ta_IsLDRangeAvilityFromLoc(-256, 6, 7, 8) == 0 && pair_calls == 1 && kind_calls == 0);
    assert(pre_args[1] == 0 && pre_args[2] == -1 && pair_args[0] == 8 && pair_args[1] == 6);
    pair_result = 1;
    assert(Ta_IsLDRangeAvilityFromLoc(0x1234, 6, 7, 8) == 1 && kind_calls == 1);
    assert(Ta_IsLDRangeAvilityFromLoc(0x1234, 6, 0x47, 8) == 0 && kind_calls == 2);
}

static void check_solo_batch_2(void)
{
    unsigned short position;
    int object;

    /* Ta_GetHandEnchant (branch coverage): bit set and clear; first and last bit of a
     * byte, the next byte, the next row. b below 752 is not exercised (it
     * would index before the row). */
    memset(EnchMatch, 0, sizeof(EnchMatch));
    tail_calls = 0;
    assert(Ta_GetHandEnchant(0, 752) == -1 && tail_calls == 0);
    EnchMatch[0] = 0x81;
    assert(Ta_GetHandEnchant(0, 752) == 0x9040 && tail_args[0] == 0 && tail_args[1] == 752);
    assert(Ta_GetHandEnchant(0, 753) == -1 && Ta_GetHandEnchant(0, 759) == 0x9040);
    assert(Ta_GetHandEnchant(0, 760) == -1 && Ta_GetHandEnchant(1, 752) == -1);
    EnchMatch[1] = 1;
    EnchMatch[7] = 2;
    assert(Ta_GetHandEnchant(0, 760) == 0x9040 && Ta_GetHandEnchant(1, 753) == 0x9040);
    assert(tail_args[0] == 1 && tail_args[1] == 753 && Ta_GetHandEnchant(1, 752) == -1);

    /* TaTuto_OpenMsgPopup (branch coverage): 0, 479 below the bound, 480 at it. */
    step_value = 0x5151;
    step_arg_calls = 0;
    position = 0;
    assert(TaTuto_OpenMsgPopup(&object, &position) == 0 && position == 80 && step_arg_calls == 1);
    assert(step_object == &object && step_args[0] == 1 && step_args[1] == 0);
    assert(step_args[2] == 400 && step_args[3] == 0x5151);
    position = 479;
    assert(TaTuto_OpenMsgPopup(&object, &position) == 0 && position == 559 && step_args[2] == -79);
    position = 480;
    assert(TaTuto_OpenMsgPopup(&object, &position) == 1 && position == 480 && step_arg_calls == 2);

    /* TaTuto_CloseMsgPopup (branch coverage): 80 and 1 above zero (the latter wraps the
     * 16-bit position), and 0. */
    step_arg_calls = reset_calls = 0;
    step_object = 0;
    position = 80;
    assert(TaTuto_CloseMsgPopup(&object, &position) == 0 && position == 0 && step_arg_calls == 1);
    assert(step_object == &object && step_args[0] == 1 && step_args[1] == 0);
    assert(step_args[2] == 480 && step_args[3] == 0x5151 && reset_calls == 0);
    position = 1;
    assert(TaTuto_CloseMsgPopup(&object, &position) == 0 && position == 65457 && step_args[2] == 480 - 65457);
    position = 0;
    assert(TaTuto_CloseMsgPopup(&object, &position) == 1 && position == 0);
    assert(step_arg_calls == 2 && reset_calls == 1);
}

/* TaLogic_AdjustLogicData (branch coverage): callee result 1, and not 1. */
static void check_query_gate(void)
{
    struct DotrQueryObject1a2 object;
    object.unknown_1a2 = 0x1234;
    side_result = 0x77;
    f830_result = 1;
    e80_calls = 0;
    e80_result = 0x5a5a;
    assert(TaLogic_AdjustLogicData(&object) == 1 && f830_arg == 0x77);
    assert(e80_calls == 0 && object.unknown_1a2 == 0x1234);
    f830_result = 2;
    assert(TaLogic_AdjustLogicData(&object) == 0x5a5a && e80_calls == 1 && e80_arg == &object);
    assert(object.unknown_1a2 == 0);
}

static int pick_kind, pick_slot, pick_calls;
int SzDuel_GetPlayer(int index) { assert(index == side_result); return pick_kind; }
int SzData_GetCurrentDeck(int side) { return side + 0x100; }
int SzData_GetLeader(int side, int list) { assert(list == side + 0x100); return side + 0x200; }
int SzData_GetRank(int side, int id) { assert(id == side + 0x200); return 0x48; }
int TaHand_IsDestinyCondition(int side) { assert(side == 1); pick_calls++; return pick_slot; }

/* 0x26eed0, branch coverage: one call per outcome and per bound. */
static int pick_case(int kind, int pair, int word, int slot)
{
    int r;

    pick_kind = kind;
    pair_result = pair;
    pair_calls = 0;
    word_result = word;
    word_index = -1;
    pick_slot = slot;
    pick_calls = 0;
    limit_seen = -1;
    limit_value = 3;
    side_result = 0x21;
    limit_expected = 0;
    r = Ta_IsDestinyDraw(1);
    limit_expected = 1500;
    return r;
}

static void check_pick(void)
{
    /* Kinds below 32 other than 19 and 1 are refused before any other call. */
    assert(pick_case(0, 1, 0, 5) == -1 && word_index == -1 && pick_calls == 0);
    assert(pick_case(31, 1, 0, 5) == -1 && word_index == -1);
    assert(pick_case(19, 1, 0, 5) == (5 | 3 << 4) && limit_seen == 0x40 && pair_calls == 0);
    assert(pick_case(1, 1, 0, 5) == 0x35 && pick_calls == 1);
    assert(pick_case(32, 1, 999, 5) == 0x35 && word_index == 1 && pair_calls == 0);
    /* Kinds above 99 also need a non-zero Ta_IsLDAbl(3, side). */
    assert(pick_case(99, 0, 0, 5) == 0x35 && pair_calls == 0);
    assert(pick_case(100, 0, 0, 5) == -1 && pair_calls == 1 && word_index == -1);
    assert(pair_args[0] == 3 && pair_args[1] == 1);
    assert(pick_case(100, 1, 0, 5) == 0x35 && pair_calls == 1);
    /* Word bound 1000 and slot sign. */
    assert(pick_case(32, 1, 1000, 5) == -1 && pick_calls == 0);
    assert(pick_case(32, 1, 0, -1) == -1 && pick_calls == 1 && limit_seen == -1);
    assert(pick_case(32, 1, 0, 0) == 0x30);
}

static int rank_query(int side_now, int flip, int side_byte, int enabled, int rank_byte)
{
    struct DotrStateItem item;

    memset(&item, 0, sizeof(item));
    item.status.unknown_bit_1 = flip ? -1 : 0;
    item.team = (unsigned char)side_byte;
    item.slot = (unsigned char)enabled;
    item.kind = (unsigned char)rank_byte;
    side_result = side_now;
    return Ta_IsUnitHide2(&item);
}

static void set_rank_entry(int side, int i, int low)
{
    set_query(side, i, 1, (1 << 3) | (1 << 4) | (1 << 5), 68);
    q_entries[side][i].unknown_words[0] |= low;
}

/* 0x267560, branch coverage. The real list callees only return slots with the three grid bits and a
 * kind-68 entry, so the function's own bit and null-entry rejections are not reachable here. */
static void check_rank_query(void)
{
    int saved_side = side_result;

    query_mode = 1;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    side_queries = 0;
    assert(Ta_IsUnitHide2(0) == 0 && side_queries == 0);
    set_rank_entry(0, 2, 30);
    /* same side: plain byte, and the byte inverted by the flag (0 -> 1, non-zero -> 0) */
    assert(rank_query(1, 0, 1, 1, 0xff) == 0 && side_queries == 1);
    assert(rank_query(1, 1, 0, 1, 0xff) == 0 && rank_query(0, 1, 5, 1, 0xff) == 0);
    /* other side but the byte at 0x45 is zero */
    assert(rank_query(0, 0, 1, 0, 0xff) == 0);
    /* low bits 30: non-zero rank gives 1 (0x20 is the first), zero rank gives 0 */
    assert(rank_query(0, 0, 1, 1, 0x20) == 1 && rank_query(0, 0, 1, 1, 0x1f) == 0);
    assert(rank_query(0, 1, 0, 1, 0xff) == 1 && rank_query(1, 1, 7, 1, 0x20) == 1);
    /* low bits 29: the opposite */
    set_rank_entry(0, 2, 29);
    assert(rank_query(0, 0, 1, 1, 0x20) == 0 && rank_query(0, 0, 1, 1, 0x1f) == 1);
    /* other low bits are passed over: a later entry (side 1) decides, or nothing does */
    set_rank_entry(0, 2, 5);
    assert(rank_query(0, 0, 1, 1, 0x20) == 0);
    set_rank_entry(1, 10, 30);
    assert(rank_query(0, 0, 1, 1, 0x20) == 1 && rank_query(0, 0, 1, 1, 0) == 0);
    /* no kind-68 entry at all */
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    assert(rank_query(0, 0, 1, 1, 0x20) == 0);
    query_mode = 0;
    side_result = saved_side;
}

static int kind11_run(int (*f)(int, int), int range)
{
    range_result = range;
    range_calls = 0;
    range_last[0] = range_last[1] = -1;
    return f(6, 9);
}

/* 0x267700 / 0x267860, branch coverage, with the limit stated at check_rank_query: the bit and
 * null-entry rejections cannot be reached through the real list callees. */
static void check_kind11_walks(void)
{
    int saved_range = range_result;

    query_mode = 1;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    /* b == 0: nothing is called */
    range_calls = 0;
    assert(Ta_IsResistAll(6, 0) == 0 && Ta_IsResistAllMaster(6, 0) == 0 && range_calls == 0);
    /* empty list */
    assert(kind11_run(Ta_IsResistAll, 0) == 0 && kind11_run(Ta_IsResistAllMaster, 0) == 0 && range_calls == 0);
    set_query(0, 2, 1, (1 << 3) | (1 << 4) | (1 << 5), 11);
    set_query(1, 10, 1, (1 << 3) | (1 << 4) | (1 << 5), 11);
    /* both parts zero: two calls with the arguments forwarded */
    assert(kind11_run(Ta_IsResistAll, 0) == 1 && range_calls == 2);
    assert(range_last[0] == 6 && range_last[1] == 9);
    assert(kind11_run(Ta_IsResistAllMaster, 0) == 2 && range_calls == 2);
    assert(range_last[0] == 6 && range_last[1] == 9);
    /* low five bits set: 0x267700 answers 0 after one call, 0x267860 tries the second slot too */
    assert(kind11_run(Ta_IsResistAll, 1) == 0 && range_calls == 1);
    assert(kind11_run(Ta_IsResistAllMaster, 1) == 0 && range_calls == 2);
    assert(kind11_run(Ta_IsResistAll, 0x1f) == 0 && range_calls == 1);
    /* only the upper part set: two calls, then the same split */
    assert(kind11_run(Ta_IsResistAll, 0x20) == 0 && range_calls == 2);
    assert(kind11_run(Ta_IsResistAllMaster, 0x20) == 0 && range_calls == 4);
    /* non-zero low six bits in the first entry: it is passed over, the second decides */
    q_entries[0][2].unknown_words[0] |= 5;
    assert(kind11_run(Ta_IsResistAll, 0) == 1 && range_calls == 2);
    assert(kind11_run(Ta_IsResistAllMaster, 0) == (0x100 | 10) && range_calls == 2);
    q_entries[1][10].unknown_words[0] |= 0x20;
    assert(kind11_run(Ta_IsResistAll, 0) == 0 && kind11_run(Ta_IsResistAllMaster, 0) == 0 && range_calls == 0);
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    query_mode = 0;
    range_result = saved_range;
}

static struct DotrStateItem score_item, score_other;
static int score_gate[2], score_cell, score_bit, score_bit_arg, score_calls[6];
static int score_bc0 = 1000, score_a60 = 2000, score_9e0 = 600;

int func_00250900_score(struct DotrStateItem *item)
{
    assert(item == &score_item || item == &score_other);
    return score_gate[item == &score_other];
}

int func_00250bc0_score(struct DotrStateItem *item, int c, int d)
{
    assert(item == &score_item && c == 7 && d == 9);
    score_calls[0]++;
    return score_bc0;
}

int func_00250a60_score(struct DotrStateItem *item, int c, int d)
{
    assert(item == &score_item && c == 7 && d == 9);
    score_calls[1]++;
    return score_a60;
}

struct DotrStateItem *func_001df540_score(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    score_calls[2]++;
    return &score_other;
}

int func_002787f0_score(struct DotrStateItem *a, struct DotrStateItem *b)
{
    assert((a == &score_item && b == &score_other) || (a == &score_other && b == &score_item));
    return a == &score_item ? 30 : 4;
}

int func_001e0700_score(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    return score_cell;
}

int func_001e2100_score(int a, int b, int bit)
{
    assert(a == 0x12 && b == 0x34);
    score_bit_arg = bit;
    return score_bit;
}

int func_00250b40_score(int a, int b, int c, int d)
{
    assert(a == 0x12 && b == 0x34 && c == 7 && d == 9);
    score_calls[3]++;
    return 500;
}

int func_002509e0_score(int a, int b, int c, int d)
{
    assert(a == 0x12 && b == 0x34 && c == 7 && d == 9);
    score_calls[4]++;
    return score_9e0;
}

/* TaLogic_IsUnitWin2: branch coverage, every callee stubbed. */
static void check_score_difference(void)
{
    memset(&score_item, 0, sizeof(score_item));
    memset(score_calls, 0, sizeof(score_calls));
    assert(TaLogic_IsUnitWin2(0, 0x1234, 7, 9) == 0 && score_calls[2] == 0);

    /* Both sides gated: only the two pair terms, 30 - 4. */
    score_gate[0] = score_gate[1] = 1;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 26 && score_calls[2] == 3);

    /* Upper bits of +0x3e / of the cell byte also zero a side; 0x1f does not. */
    score_gate[0] = score_gate[1] = 0;
    score_item.kind = 0x20;
    score_cell = 0x20;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 26);
    assert(score_calls[0] + score_calls[1] + score_calls[3] + score_calls[4] == 0);

    /* Item side: 0x250bc0 needs both bytes and bit 3; each miss uses 0x250a60. */
    score_item.kind = 0x1f;
    score_item.locCol = 7;
    score_item.locRow = 9;
    score_item.status.unknown_bit_3 = -1;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 1030 - 4 && score_calls[0] == 1);
    score_item.status.unknown_bit_3 = 0;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 2030 - 4 && score_calls[1] == 1);
    score_item.status.unknown_bit_3 = -1;
    score_item.locRow = 8;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 2030 - 4 && score_calls[1] == 2);
    score_item.locRow = 9;
    score_item.locCol = 6;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 2030 - 4 && score_calls[1] == 3 && score_calls[0] == 1);

    /* Other side: bit 5 of the cell selects 0x250b40, else 0x2509e0. */
    score_gate[0] = 1;
    score_cell = 0x1f;
    score_bit = 1;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 30 - 504 && score_calls[3] == 1 && score_bit_arg == 5);
    score_bit = 0;
    assert(TaLogic_IsUnitWin2(&score_item, 0x1234, 7, 9) == 30 - 604 && score_calls[4] == 1);
}

static int near_answer, near_calls, near_skip, forward_calls, forward_base;

int TaLogic_IsJudgeUnitDanger(int a, int b, int skip)
{
    assert(a == 0x12 && b == 0x34);
    near_skip = skip;
    near_calls++;
    return near_answer;
}

int func_00278420_score(struct DotrStateItem *item, int base, int x, int y)
{
    assert(item == &score_item && x == 7 && y == 9);
    forward_base = base;
    forward_calls++;
    return 77;
}

/* TaLogic_IsUnitWin5: branch coverage, every callee stubbed. */
static void check_score_class(void)
{
    memset(&score_item, 0, sizeof(score_item));
    memset(score_calls, 0, sizeof(score_calls));
    near_calls = forward_calls = 0;
    assert(TaLogic_IsUnitWin5(0, 0x1234, 50, 7, 9, 5) == 0 && near_calls == 0 && forward_calls == 0);
    /* position -1 forwards */
    assert(TaLogic_IsUnitWin5(&score_item, -1, 50, 7, 9, 5) == 77 && forward_calls == 1 && forward_base == 50);
    assert(near_calls == 0 && score_calls[2] == 0);

    /* the gate answers 0: plain scores against the argument */
    near_answer = 0;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 1999, 7, 9, 5) == 3 && near_calls == 1 && near_skip == 5);
    assert(score_calls[0] == 1 && score_calls[1] == 1 && score_calls[2] == 0);
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 2000, 7, 9, 5) == 3);
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 2001, 7, 9, 5) == 0);
    score_a60 = 1000; /* equal scores take the second test */
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 999, 7, 9, 5) == 2);
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 1000, 7, 9, 5) == 0);
    score_a60 = 500;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 999, 7, 9, 5) == 2);
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 1000, 7, 9, 5) == 0);

    /* the gate answers: both sides gated give zeros, so 0 */
    near_answer = 1;
    score_gate[0] = score_gate[1] = 1;
    memset(score_calls, 0, sizeof(score_calls));
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 5000, 7, 9, 5) == 0);
    assert(score_calls[0] + score_calls[1] + score_calls[4] == 0 && score_calls[2] == 1);
    /* high bits of +0x3e and of the cell do the same */
    score_gate[0] = score_gate[1] = 0;
    score_item.kind = 0x20;
    score_cell = 0x20;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 5000, 7, 9, 5) == 0);
    assert(score_calls[0] + score_calls[1] + score_calls[4] == 0 && score_calls[2] == 2);
    /* zero scores against a real base: 0 */
    score_cell = 0x1f;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 5000, 7, 9, 5) == 0 && score_calls[4] == 1 && score_calls[2] == 4);
    /* real scores (each plus 30) against a zero base */
    score_item.kind = 0x1f;
    score_cell = 0x20;
    score_a60 = 2000;
    memset(score_calls, 0, sizeof(score_calls));
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 5000, 7, 9, 5) == 3);
    assert(score_calls[0] == 1 && score_calls[1] == 1 && score_calls[2] == 3 && score_calls[4] == 0);
    /* both real: base is the 0x2509e0 answer plus 4 */
    score_cell = 0x1f;
    memset(score_calls, 0, sizeof(score_calls));
    score_9e0 = 2026;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 0, 7, 9, 5) == 3 && score_calls[2] == 4 && score_calls[4] == 1);
    score_9e0 = 2027;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 0, 7, 9, 5) == 0);
    score_a60 = 500;
    score_9e0 = 1025;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 5000, 7, 9, 5) == 2);
    score_9e0 = 1026;
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 0, 7, 9, 5) == 0);
    score_gate[0] = 1; /* the item's gate alone */
    assert(TaLogic_IsUnitWin5(&score_item, 0x1234, 0, 7, 9, 5) == 0);
    score_gate[0] = 0;
    score_a60 = 2000, score_9e0 = 600;
}

static int store_args[3], store_calls;
int SzDuel_CalcLP(int index, int operation, int value)
{
    store_args[0] = index;
    store_args[1] = operation;
    store_args[2] = value;
    store_calls++;
    return 0;
}

static void check_word_store_modes(void)
{
    /* the 0x2670f0 stub answers 1000 and records its arguments */
    store_calls = 0;
    word_result = 70;
    word_index = -1;
    Ta_CalcLPinEffectSeq(5, 0, 33);
    assert(store_calls == 1 && store_args[0] == 5 && store_args[1] == 0 && store_args[2] == 1000);
    assert(scale_args[0] == 43 && scale_args[1] == 33 && word_index == -1);
    Ta_CalcLPinEffectSeq(6, 1, 34);
    assert(store_calls == 2 && store_args[0] == 6 && store_args[1] == 1 && store_args[2] == 1000);
    assert(scale_args[0] == 44 && scale_args[1] == 34 && word_index == -1);
    Ta_CalcLPinEffectSeq(7, 2, 35);
    assert(store_calls == 3 && store_args[0] == 7 && store_args[1] == 0 && store_args[2] == 1000);
    assert(scale_args[0] == 43 && scale_args[1] == 70 && word_index == 7);
    Ta_CalcLPinEffectSeq(8, 3, 36);
    assert(store_calls == 4 && store_args[0] == 8 && store_args[1] == 1 && store_args[2] == 1000);
    assert(scale_args[0] == 44 && scale_args[1] == 70 && word_index == 8);
    /* mode 4: stored only when value is above the current word */
    Ta_CalcLPinEffectSeq(9, 4, 70);
    assert(store_calls == 4 && word_index == 9);
    Ta_CalcLPinEffectSeq(9, 4, 71);
    assert(store_calls == 5 && store_args[0] == 9 && store_args[1] == 4 && store_args[2] == 71);
    /* other modes: no call at all */
    word_index = -1;
    Ta_CalcLPinEffectSeq(9, 5, 99);
    Ta_CalcLPinEffectSeq(9, -1, 99);
    assert(store_calls == 5 && word_index == -1);
}

static int mask_case(const int *kinds, const int *needs)
{
    int list[5] = {100, 101, 102, 103, 104};
    int i;

    for (i = 0; i < 5; i++) {
        memset(&node_entries[i], 0, sizeof(node_entries[i]));
        *(unsigned char *)&node_entries[i] = (unsigned char)(kinds[i] << 5 | 3);
        need_by_id[i] = needs[i];
    }
    need_calls = 0;
    return TaLogic005_Is2Head_ThunderDragon(1, list);
}

static void check_kind_mask(void)
{
    /* 0x26fce0 returns a byte, so kinds are 0-7 here: the kind 18 and kind 15
       scans run but can never add a bit through the real callee. */
    static const int kinds[5] = {7, 1, 2, 0, 0};
    static const int early[5] = {7, 1, 0, 0, 0};
    int needs[5] = {5, 5, 5, 5, 5};
    int saved_counts[2] = {counts[0], counts[1]};
    int saved_have = have_result;
    struct DotrStateEntry20 saved_nodes[5];

    memcpy(saved_nodes, node_entries, sizeof(saved_nodes));
    need_by_id_on = 1;
    have_result = 5;
    counts[0] = 2;
    counts[1] = 4;
    /* kind 0 first usable at i = 3: bit 1, the one at i = 4 not added; all three scans run */
    assert(mask_case(kinds, needs) == 2);
    assert(need_calls == 4 + 5 + 5 && need_args[1] == 1 && have_argument == 1);
    /* i = 3 not usable: kind 0 at i = 4 gives mask 1, not 2: 0 after two scans */
    needs[3] = 6;
    assert(mask_case(kinds, needs) == 0);
    assert(need_calls == 5 + 5);
    /* no usable kind 0: 0 after the first scan alone */
    needs[4] = 6;
    assert(mask_case(kinds, needs) == 0);
    assert(need_calls == 5);
    /* kind 0 at i = 2: mask 4, 0 after two scans */
    needs[3] = needs[4] = 5;
    assert(mask_case(early, needs) == 0);
    assert(need_calls == 3 + 5);
    /* the count bounds every scan: three entries hide the kind 0 at i = 3 */
    counts[1] = 1;
    assert(mask_case(kinds, needs) == 0);
    assert(need_calls == 3);
    need_by_id_on = 0;
    have_result = saved_have;
    counts[0] = saved_counts[0];
    counts[1] = saved_counts[1];
    memcpy(node_entries, saved_nodes, sizeof(saved_nodes));
}

static int set_case(int id, int word, int f0, int f1, int f2, int f3)
{
    set_word = word;
    set_found[0] = f0;
    set_found[1] = f1;
    set_found[2] = f2;
    set_found[3] = f3;
    set_calls = 0;
    side_queries = 0;
    return TaLogic022_IsRefuseEmptyCombo(id);
}

static int set_side_case(int (*f)(int), int word, int f0, int f1, int f2, int f3)
{
    set_word = word;
    set_found[0] = f0;
    set_found[1] = f1;
    set_found[2] = f2;
    set_found[3] = f3;
    set_calls = 0;
    return f(2);
}

/* 0x112980 copies: the set_mode stubs fix side 2, list 0x102 and the 54..57 order. */
static void check_side_set(int (*f)(int))
{
    set_mode = 1;
    /* the (side, 0) word must be 58 */
    assert(set_side_case(f, 57, 0, 0, 0, 0) == 0 && set_calls == 0);
    assert(set_side_case(f, 59, 0, 0, 0, 0) == 0 && set_calls == 0);
    /* all four found (boundary 0 and positive): 1 */
    assert(set_side_case(f, 58, 0, 0, 0, 0) == 1 && set_calls == 4);
    assert(set_side_case(f, 58, 3, 5, 1, 9) == 1 && set_calls == 4);
    /* each negative answer stops the chain with 0 */
    assert(set_side_case(f, 58, -1, 0, 0, 0) == 0 && set_calls == 1);
    assert(set_side_case(f, 58, 0, -1, 0, 0) == 0 && set_calls == 2);
    assert(set_side_case(f, 58, 0, 0, -1, 0) == 0 && set_calls == 3);
    assert(set_side_case(f, 58, 0, 0, 0, -1) == 0 && set_calls == 4);
    set_mode = 0;
}

static void check_id_set(void)
{
    struct DotrStateEntry20 saved = node_entries[0];
    int saved_side = side_result;

    set_mode = 1;
    side_result = 2;
    memset(&node_entries[0], 0, sizeof(node_entries[0]));
    /* nonzero kind: 1 without asking the side */
    *(unsigned char *)&node_entries[0] = 0x20;
    assert(set_case(100, 58, 0, 0, 0, 0) == 1 && side_queries == 0);
    /* kind 0 with a halfword at +4 other than 0xffff: state 1, answer 1 */
    *(unsigned char *)&node_entries[0] = 0x1f;
    assert(set_case(100, 58, 0, 0, 0, 0) == 1 && side_queries == 0);
    /* state 2 and an id outside 54-57: 0 */
    ((unsigned short *)&node_entries[0])[2] = 0xffff;
    assert(set_case(100, 58, 0, 0, 0, 0) == 0 && side_queries == 0);
    /* ids without an entry: only 54-57 go on */
    assert(set_case(53, 58, 0, 0, 0, 0) == 0 && side_queries == 0);
    assert(set_case(58, 58, 0, 0, 0, 0) == 0 && side_queries == 0);
    assert(set_case(54, 58, 0, 0, 0, 0) == 1 && side_queries == 1 && set_calls == 4);
    assert(set_case(57, 58, 0, 5, 0, 9) == 1 && side_queries == 1 && set_calls == 4);
    /* the (side, 0) word must be 58 */
    assert(set_case(54, 57, 0, 0, 0, 0) == 0 && side_queries == 1 && set_calls == 0);
    /* each of the four members must be found (non-negative), tested in order */
    assert(set_case(55, 58, -1, 0, 0, 0) == 0 && set_calls == 1);
    assert(set_case(55, 58, 0, -1, 0, 0) == 0 && set_calls == 2);
    assert(set_case(56, 58, 0, 0, -1, 0) == 0 && set_calls == 3);
    assert(set_case(56, 58, 0, 0, 0, -1) == 0 && set_calls == 4);
    set_mode = 0;
    side_result = saved_side;
    node_entries[0] = saved;
}

static void check_kind_state_veto(int (*f)(int))
{
    struct DotrStateEntry20 saved = node_entries[0];

    memset(&node_entries[0], 0, sizeof(node_entries[0]));
    /* nonzero kind: 1 */
    *(unsigned char *)&node_entries[0] = 0x20;
    assert(f(100) == 1);
    /* kind 0 with a halfword at +4 other than 0xffff: state 1, answer 1 */
    *(unsigned char *)&node_entries[0] = 0x1f;
    assert(f(100) == 1);
    /* kind 0 and state 2: 0 */
    ((unsigned short *)&node_entries[0])[2] = 0xffff;
    assert(f(100) == 0);
    node_entries[0] = saved;
}

/* the kind byte pick_entry gives each entry (0x124c10 vetoes a low kind of 0) */
static unsigned char pick_entry_kind;

/* vetoes on fixed ids: a nonzero upper kind answers 1, then only the listed ids do */
static void check_id_veto(int (*f)(int), const int *ids, int n, const int *others, int m)
{
    struct DotrStateEntry20 saved[8];
    int i;

    memcpy(saved, node_entries, sizeof(saved));
    memset(node_entries, 0, sizeof(saved));
    *(unsigned char *)&node_entries[0] = 0x20;
    assert(f(100) == 1);
    *(unsigned char *)&node_entries[0] = 0x1f;
    assert(f(100) == 0);
    for (i = 0; i < n; i++) {
        assert(f(ids[i]) == 1);
    }
    for (i = 0; i < m; i++) {
        assert(f(others[i]) == 0);
    }
    memcpy(node_entries, saved, sizeof(saved));
}

/* 0x124c10: a nonzero upper kind, or a low kind of 0, 15 or 18 */
static void check_low_kind_veto(int (*f)(int))
{
    static const unsigned char kinds[] = {0x20, 0, 15, 18, 1, 14, 16, 17, 19, 0x1f};
    static const int want[] = {1, 1, 1, 1, 0, 0, 0, 0, 0, 0};
    struct DotrStateEntry20 saved = node_entries[0];
    unsigned i;

    memset(&node_entries[0], 0, sizeof(node_entries[0]));
    for (i = 0; i < sizeof(kinds); i++) {
        *(unsigned char *)&node_entries[0] = kinds[i];
        assert(f(100) == want[i]);
    }
    /* no entry: kind 0 */
    assert(f(41) == 1);
    node_entries[0] = saved;
}

/* 0x12e1a0 copies: a nonzero upper kind, or any nonzero 0x26fc10 state */
static void check_any_state_veto(int (*f)(int))
{
    struct DotrStateEntry20 saved = node_entries[0];

    memset(&node_entries[0], 0, sizeof(node_entries[0]));
    *(unsigned char *)&node_entries[0] = 0x20;
    assert(f(100) == 1);
    /* state 1 */
    *(unsigned char *)&node_entries[0] = 0x1f;
    assert(f(100) == 1);
    /* state 2 */
    ((unsigned short *)&node_entries[0])[2] = 0xffff;
    assert(f(100) == 1);
    /* no entry and id 999: state 0 */
    assert(f(41) == 0 && f(999) == 0);
    node_entries[0] = saved;
}

static void pick_entry(int n, int need, int high, int low, int state_word, int match)
{
    memset(&node_entries[n], 0, sizeof(node_entries[n]));
    *(unsigned char *)&node_entries[n] = pick_entry_kind;
    ((unsigned short *)&node_entries[n])[2] = (unsigned short)state_word;
    ((unsigned short *)&node_entries[n])[4] = (unsigned short)high;
    ((unsigned short *)&node_entries[n])[5] = (unsigned short)low;
    need_by_id[n] = need;
    pick_match[n] = match;
}

static int rule_veto_id = -1;
static int rule_calls;

int TaLogic002_IsRefuseEmptyCombo(int id)
{
    rule_calls++;
    return id == rule_veto_id;
}

static void pick_run(void (*f)(int, unsigned short *, int *), unsigned short *item, int *table)
{
    pick_len_calls = pick_probe_calls = pick_mark_calls = 0;
    need_calls = rule_calls = 0;
    f(1, item, table);
}

/* veto: 0 none, 1 the 0x10f220 stub, 2 the real 0x1702b0 or a kind/state veto copy (a state
 * other than 2 answers 1) */
static void check_slot_picks(void (*f)(int, unsigned short *, int *), int limit, int veto)
{
    unsigned short item[4] = {50, 0, 0, 0};
    int table[5] = {100, 999, 100, 100, 101};
    int saved_counts[2] = {counts[0], counts[1]};
    int saved_need = need_result;
    struct DotrStateEntry20 saved_nodes[2];

    memcpy(saved_nodes, node_entries, sizeof(saved_nodes));
    pick_mode = need_by_id_on = 1;
    pick_bytes = (unsigned char *)(item + 1);
    counts[0] = 2;
    counts[1] = 4;
    need_result = 0x103;
    /* no item: nothing is called */
    pick_run(f, 0, table);
    assert(need_calls == 0 && pick_probe_calls == 0);
    /* no clear slot: the list is not measured */
    memset(pick_clear, 0, sizeof(pick_clear));
    pick_run(f, item, table);
    assert(pick_probe_calls == 5 && pick_len_calls == 0 && pick_mark_calls == 0);
    /* slots 1, 3 and 4 clear; slot 1 holds 999; ids 100 and 101 pass every test at the bounds */
    pick_clear[1] = pick_clear[3] = pick_clear[4] = 1;
    pick_len = 3;
    pick_entry(0, 3, 0, limit, 0xffff, 0x10000 | 999);
    pick_entry(1, 0, 1, limit - 2, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_seen[0] == 1 && pick_seen[1] == 3 && pick_seen[2] == 4 && pick_seen[3] == 0xff && pick_seen[4] == 0xff);
    assert(pick_len_calls == 4 && pick_mark_calls == 2 && pick_marks[0] == 3 && pick_marks[1] == 4);
    assert(need_calls == 3 && rule_calls == (veto == 1 ? 2 : 0));
    if (veto == 1) {
        /* a nonzero 0x10f220 answer for id 101 skips it before its cost is read */
        rule_veto_id = 101;
        pick_run(f, item, table);
        assert(pick_mark_calls == 1 && pick_marks[0] == 3 && need_calls == 2 && rule_calls == 2);
        rule_veto_id = -1;
    }
    /* the measured length bounds the walk */
    pick_len = 2;
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && pick_marks[0] == 3);
    pick_len = 3;
    /* each test failing in turn for id 101: cost above the item's, sum above the limit,
       state not 2, pair word not 999 */
    pick_entry(1, 4, 1, limit - 2, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    pick_entry(1, 3, 1, limit - 1, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    pick_entry(1, 3, 1, limit - 2, 0, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && need_calls == (veto == 2 ? 2 : 3));
    pick_entry(1, 3, 1, limit - 2, 0xffff, 998);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    /* the slot count bounds the probes: three slots leave only slot 1 (999) */
    counts[1] = 1;
    pick_run(f, item, table);
    assert(pick_probe_calls == 3 && pick_seen[0] == 1 && pick_seen[1] == 0xff && pick_mark_calls == 0);
    pick_mode = need_by_id_on = 0;
    counts[0] = saved_counts[0];
    counts[1] = saved_counts[1];
    need_result = saved_need;
    memcpy(node_entries, saved_nodes, sizeof(saved_nodes));
}

/* Pick copies (kind/state veto) whose sum test is `== 0`: both sum halfwords must be zero. */
static void check_slot_picks_zero(void (*f)(int, unsigned short *, int *))
{
    unsigned short item[4] = {50, 0, 0, 0};
    int table[5] = {100, 999, 100, 100, 101};
    int saved_counts[2] = {counts[0], counts[1]};
    int saved_need = need_result;
    struct DotrStateEntry20 saved_nodes[2];

    memcpy(saved_nodes, node_entries, sizeof(saved_nodes));
    pick_mode = need_by_id_on = 1;
    pick_bytes = (unsigned char *)(item + 1);
    counts[0] = 2;
    counts[1] = 4;
    need_result = 0x103;
    /* no item: nothing is called */
    pick_run(f, 0, table);
    assert(need_calls == 0 && pick_probe_calls == 0);
    /* no clear slot: the list is not measured */
    memset(pick_clear, 0, sizeof(pick_clear));
    pick_run(f, item, table);
    assert(pick_probe_calls == 5 && pick_len_calls == 0 && pick_mark_calls == 0);
    /* slots 1, 3 and 4 clear; slot 1 holds 999; ids 100 and 101 pass every test (cost at the bound) */
    pick_clear[1] = pick_clear[3] = pick_clear[4] = 1;
    pick_len = 3;
    pick_entry(0, 3, 0, 0, 0xffff, 0x10000 | 999);
    pick_entry(1, 0, 0, 0, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_seen[0] == 1 && pick_seen[1] == 3 && pick_seen[2] == 4 && pick_seen[3] == 0xff && pick_seen[4] == 0xff);
    assert(pick_len_calls == 4 && pick_mark_calls == 2 && pick_marks[0] == 3 && pick_marks[1] == 4);
    assert(need_calls == 3 && rule_calls == 0);
    /* the measured length bounds the walk */
    pick_len = 2;
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && pick_marks[0] == 3);
    pick_len = 3;
    /* each test failing in turn for id 101: cost above the item's, low half 1, high half 1,
       state not 2 (vetoed before its cost is read), pair word not 999 */
    pick_entry(1, 4, 0, 0, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    pick_entry(1, 3, 0, 1, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    pick_entry(1, 3, 1, 0, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    pick_entry(1, 3, 0, 0, 0, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && need_calls == 2);
    pick_entry(1, 3, 0, 0, 0xffff, 998);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    /* the slot count bounds the probes: three slots leave only slot 1 (999) */
    counts[1] = 1;
    pick_run(f, item, table);
    assert(pick_probe_calls == 3 && pick_seen[0] == 1 && pick_seen[1] == 0xff && pick_mark_calls == 0);
    pick_mode = need_by_id_on = 0;
    counts[0] = saved_counts[0];
    counts[1] = saved_counts[1];
    need_result = saved_need;
    memcpy(node_entries, saved_nodes, sizeof(saved_nodes));
}

/* Pick copies whose veto answers 1 for every id with an entry: the walk runs, nothing is marked
   and only the item's own cost is read. */
static void check_slot_picks_vetoed(void (*f)(int, unsigned short *, int *), int limit)
{
    unsigned short item[4] = {50, 0, 0, 0};
    int table[5] = {100, 999, 100, 100, 101};
    int saved_counts[2] = {counts[0], counts[1]};
    int saved_need = need_result;
    struct DotrStateEntry20 saved_nodes[2];

    memcpy(saved_nodes, node_entries, sizeof(saved_nodes));
    pick_mode = need_by_id_on = 1;
    pick_bytes = (unsigned char *)(item + 1);
    counts[0] = 2;
    counts[1] = 4;
    need_result = 0x103;
    /* no item: nothing is called */
    pick_run(f, 0, table);
    assert(need_calls == 0 && pick_probe_calls == 0);
    /* no clear slot: the list is not measured */
    memset(pick_clear, 0, sizeof(pick_clear));
    pick_run(f, item, table);
    assert(pick_probe_calls == 5 && pick_len_calls == 0 && pick_mark_calls == 0);
    /* slots 1, 3 and 4 clear with entries that pass every later test: all vetoed */
    pick_clear[1] = pick_clear[3] = pick_clear[4] = 1;
    pick_len = 3;
    pick_entry(0, 3, 0, limit, 0xffff, 0x10000 | 999);
    pick_entry(1, 0, 1, limit - 2, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_seen[0] == 1 && pick_seen[1] == 3 && pick_seen[2] == 4 && pick_seen[3] == 0xff && pick_seen[4] == 0xff);
    assert(pick_len_calls == 4 && pick_mark_calls == 0 && need_calls == 1);
    /* the measured length bounds the walk */
    pick_len = 2;
    pick_run(f, item, table);
    assert(pick_len_calls == 3 && pick_mark_calls == 0);
    /* the slot count bounds the probes */
    counts[1] = 1;
    pick_run(f, item, table);
    assert(pick_probe_calls == 3 && pick_seen[0] == 1 && pick_seen[1] == 0xff && pick_mark_calls == 0);
    pick_mode = need_by_id_on = 0;
    counts[0] = saved_counts[0];
    counts[1] = saved_counts[1];
    need_result = saved_need;
    memcpy(node_entries, saved_nodes, sizeof(saved_nodes));
}

static int plain_veto_id = -1;
static int plain_calls;

int TaLogic017_IsRefuseEmptyCombo(int id)
{
    plain_calls++;
    return id == plain_veto_id;
}

/* 0x1566d0: no sum or state test; the veto is the 0x156870 stub. */
static void check_slot_picks_plain(void (*f)(int, unsigned short *, int *))
{
    unsigned short item[4] = {50, 0, 0, 0};
    int table[5] = {100, 999, 100, 100, 101};
    int saved_counts[2] = {counts[0], counts[1]};
    int saved_need = need_result;
    struct DotrStateEntry20 saved_nodes[2];

    memcpy(saved_nodes, node_entries, sizeof(saved_nodes));
    pick_mode = need_by_id_on = 1;
    pick_bytes = (unsigned char *)(item + 1);
    counts[0] = 2;
    counts[1] = 4;
    need_result = 0x103;
    /* no item: nothing is called */
    pick_run(f, 0, table);
    assert(need_calls == 0 && pick_probe_calls == 0);
    /* no clear slot: the list is not measured */
    memset(pick_clear, 0, sizeof(pick_clear));
    pick_run(f, item, table);
    assert(pick_probe_calls == 5 && pick_len_calls == 0 && pick_mark_calls == 0);
    /* slots 1, 3 and 4 clear; slot 1 holds 999; ids 100 (cost at the bound) and 101 pass,
       whatever their sum halfwords and state word */
    pick_clear[1] = pick_clear[3] = pick_clear[4] = 1;
    pick_len = 3;
    pick_entry(0, 3, 9, 9000, 0, 0x10000 | 999);
    pick_entry(1, 0, 0, 0, 0xffff, 999);
    plain_calls = 0;
    pick_run(f, item, table);
    assert(pick_seen[0] == 1 && pick_seen[1] == 3 && pick_seen[2] == 4 && pick_seen[3] == 0xff && pick_seen[4] == 0xff);
    assert(pick_len_calls == 4 && pick_mark_calls == 2 && pick_marks[0] == 3 && pick_marks[1] == 4);
    assert(need_calls == 3 && plain_calls == 2 && rule_calls == 0);
    /* a nonzero 0x156870 answer for id 101 skips it before its cost is read */
    plain_veto_id = 101;
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && pick_marks[0] == 3 && need_calls == 2);
    plain_veto_id = -1;
    /* the measured length bounds the walk */
    pick_len = 2;
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && pick_marks[0] == 3);
    pick_len = 3;
    /* id 101 failing each test in turn: cost above the item's, pair word not 999 */
    pick_entry(1, 4, 0, 0, 0xffff, 999);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1 && need_calls == 3);
    pick_entry(1, 3, 0, 0, 0xffff, 998);
    pick_run(f, item, table);
    assert(pick_mark_calls == 1);
    /* the slot count bounds the probes: three slots leave only slot 1 (999) */
    counts[1] = 1;
    pick_len = 1;
    plain_calls = 0;
    pick_run(f, item, table);
    assert(pick_probe_calls == 3 && pick_seen[0] == 1 && pick_seen[1] == 0xff && pick_mark_calls == 0 && plain_calls == 0);
    pick_mode = need_by_id_on = 0;
    counts[0] = saved_counts[0];
    counts[1] = saved_counts[1];
    need_result = saved_need;
    memcpy(node_entries, saved_nodes, sizeof(saved_nodes));
}

static void check_remaining_pick_copies(void)
{
    static const int ids_a[] = {496, 509, 510, 514, 502, 523}, not_a[] = {495, 497, 508, 511, 513, 515, 501, 503, 522, 524, 999};
    static const int ids_b[] = {108, 111, 104}, not_b[] = {103, 105, 107, 109, 110, 112, 999};
    static const int ids_c[] = {573, 534, 99}, not_c[] = {572, 574, 533, 535, 98, 100, 999};

    check_id_veto(TaLogic004_IsRefuseEmptyCombo, ids_a, 6, not_a, 11);
    check_id_veto(TaLogic006_IsRefuseEmptyCombo, ids_b, 3, not_b, 7);
    check_id_veto(TaLogic008_IsRefuseEmptyCombo, ids_c, 3, not_c, 7);
    check_low_kind_veto(TaLogic005_IsRefuseEmptyCombo);
    check_any_state_veto(TaLogic007_IsRefuseEmptyCombo);
    check_any_state_veto(TaLogic011_IsRefuseEmptyCombo);
    check_any_state_veto(TaLogic012_IsRefuseEmptyCombo);
    /* ids 100 and 101 are outside the fixed-id vetoes: the plain walk */
    check_slot_picks(TaLogic_ConvEmptyCombo__0011f150, 3500, 0);
    check_slot_picks(TaLogic_ConvEmptyCombo__00128900, 3300, 0);
    check_slot_picks(TaLogic_ConvEmptyCombo__00132ee0, 4300, 0);
    /* a low kind of 1 passes 0x124c10; the default kind 0 is vetoed */
    pick_entry_kind = 1;
    check_slot_picks(TaLogic_ConvEmptyCombo__00124a30, 4000, 0);
    pick_entry_kind = 0;
    check_slot_picks_vetoed(TaLogic_ConvEmptyCombo__00124a30, 4000);
    check_slot_picks_vetoed(TaLogic_ConvEmptyCombo__0012dfc0, 4200);
    check_slot_picks_vetoed(TaLogic_ConvEmptyCombo__0013fa90, 3100);
    check_slot_picks_vetoed(TaLogic_ConvEmptyCombo__00144630, 2800);
}

/* The 244-byte two-step copies: every rejection in order, then both answers
 * of the final forward. The stubs fix the first cell at (7, 9). */
static void check_two_step(int (*f)(int, int, int, int, int, int))
{
    static const int two[8][2] = {{8, 10}, {6, 8}, {8, 8}, {6, 10}, {7, 11}, {7, 7}, {9, 9}, {5, 9}};
    static const int other[5][2] = {{7, 9}, {7, 10}, {6, 9}, {8, 11}, {4, 9}};
    int i;

    query_mode = 1;
    q_present[1][4] = 1;
    q_bits[1][4] = 0;
    range_result = 0;
    cell_kind = 0x21;
    next_result = 1;
    for (i = 0; i < 8; i++) {
        cell_calls = next_calls = 0;
        assert(f(1, 4, 7, 9, two[i][0], two[i][1]) == 1);
        assert(cell_calls == 1 && next_calls == 1 && next_a == 0x21 && next_item == &item_fixture);
    }
    for (i = 0; i < 5; i++) {
        cell_calls = 0;
        assert(f(1, 4, 7, 9, other[i][0], other[i][1]) == 0 && cell_calls == 0);
    }
    q_present[1][4] = 0;
    assert(f(1, 4, 7, 9, 8, 10) == 0 && cell_calls == 0);
    q_present[1][4] = 1;
    q_bits[1][4] = 1;
    assert(f(1, 4, 7, 9, 8, 10) == 0 && cell_calls == 0);
    q_bits[1][4] = 2; /* only bit 0 is asked */
    next_result = 2;
    assert(f(1, 4, 7, 9, 8, 10) == 0 && cell_calls == 1);
    next_result = 0;
    assert(f(1, 4, 7, 9, 8, 10) == 0 && cell_calls == 2);
    range_result = 0x20; /* 0x24df00's own range rejection */
    next_result = 1;
    next_calls = 0;
    assert(f(1, 4, 7, 9, 8, 10) == 0 && cell_calls == 3 && next_calls == 0);
    q_present[1][4] = 0;
    q_bits[1][4] = 0;
    range_result = 0;
    cell_kind = 0;
    next_result = 0x3355;
    query_mode = 0;
}

static void check_id_54_57(int (*f)(int))
{
    /* both boundaries of the range, and the unsigned wrap below it */
    assert(f(53) == 0 && f(54) == 1 && f(57) == 1 && f(58) == 0);
    assert(f(0) == 0 && f(-1) == 0);
}

/* 0x267380 on the real 0x266df0, 0x266c70 and 0x250740 in query mode; the two
 * small_misc callees are stubs. Branch and loop coverage, except that the
 * real 0x266c70 only answers slots that already pass the grid-bit and entry
 * tests, so those four rejections inside 0x267380 are not reached. */
static int q_owner[2][11], owner_answer, owner_calls;
static struct DotrStateItem *owner_item;

int func_00250360_pair(int a, int b)
{
    assert((a == 0 || a == 1) && b >= 1 && b <= 10);
    return q_owner[a][b];
}

int func_00250370_item(struct DotrStateItem *item)
{
    assert(item == owner_item);
    owner_calls++;
    return owner_answer;
}

static void check_slot_answer(void)
{
    const int good = (1 << 3) | (1 << 4) | (1 << 5);
    static struct DotrStateItem it;

    query_mode = 1;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    memset(q_owner, 0, sizeof(q_owner));
    memset(&it, 0, sizeof(it));
    owner_item = &it;
    owner_answer = 7;
    owner_calls = 0;

    assert(Ta_IsUnitHide(0, 0) == 0);
    /* Side equal to the +0x44 byte, plain and inverted by +0x57 bit 1. */
    it.slot = 1;
    it.team = 1;
    assert(Ta_IsUnitHide(&it, 1) == 0);
    it.status.unknown_bit_1 = -1;
    assert(Ta_IsUnitHide(&it, 0) == 0);
    it.team = 0;
    assert(Ta_IsUnitHide(&it, 1) == 0);
    /* Another side: a zero +0x45 byte, then a clear bit 2. */
    it.slot = 0;
    assert(Ta_IsUnitHide(&it, 0) == 0);
    it.slot = 1;
    assert(Ta_IsUnitHide(&it, 0) == 1);
    it.status.unknown_bit_1 = 0;
    assert(Ta_IsUnitHide(&it, 1) == 1 && Ta_IsUnitHide(&it, 2) == 1);
    /* Bit 2 set and no kind-68 slot. */
    it.status.unknown_bit_2 = -1;
    set_query(0, 4, 1, good, 67);
    assert(Ta_IsUnitHide(&it, 1) == 0 && owner_calls == 0);
    /* Three kind-68 slots: another owner, low bits neither 29 nor 30, then 29. */
    set_query(0, 2, 1, good, 68);
    q_entries[0][2].unknown_words[0] |= 29;
    q_owner[0][2] = 5;
    set_query(0, 5, 1, good, 68);
    q_entries[0][5].unknown_words[0] |= 3;
    q_owner[0][5] = 7;
    set_query(1, 3, 1, good, 68);
    q_entries[1][3].unknown_words[0] |= 29;
    q_owner[1][3] = 7;
    it.kind = 0x1f;
    assert(Ta_IsUnitHide(&it, 1) == 1 && owner_calls == 3);
    it.kind = 0x20;
    assert(Ta_IsUnitHide(&it, 1) == 0);
    /* The same slot with 30 answers the other way. */
    q_entries[1][3].unknown_words[0] ^= 29 ^ 30;
    assert(Ta_IsUnitHide(&it, 1) == 1);
    it.kind = 0x1f;
    assert(Ta_IsUnitHide(&it, 1) == 0);
    /* The first slot wins once its owner agrees. */
    q_owner[0][2] = 7;
    assert(Ta_IsUnitHide(&it, 1) == 1);
    /* Every slot skipped: 0 after the walk. */
    q_owner[0][2] = 5;
    q_owner[1][3] = 6;
    owner_calls = 0;
    assert(Ta_IsUnitHide(&it, 1) == 0 && owner_calls == 3);
    query_mode = 0;
}

static int ea_results[4], ea_calls, ea_packed, ea_word;
int Ta_GetAgreeOwnerIDon22bitFlagLst(int packed, unsigned word)
{
    ea_packed = packed;
    ea_word = (int)word;
    return ea_results[ea_calls++];
}

static int run_kind23(int first, int second)
{
    ea_results[0] = first;
    ea_results[1] = second;
    ea_calls = wide_calls = 0;
    ea_packed = ea_word = wide_side = -1;
    return TaPerm_IsPermCurseActive(3, 9);
}

/* 0x266a80, branch coverage: each failing test, each side, both boundary slots, first acceptance wins. */
static void check_kind23_find(void)
{
    const int good = (1 << 3) | (1 << 4) | (1 << 5);
    const int word = (23 << 8) | (1 << 6) | 0x1c;
    int saved_low = wide_low;
    int bit;
    query_mode = 1;
    wide_low = 0x4321;
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    /* nothing present */
    assert(run_kind23(1, 1) == 0 && ea_calls == 0 && wide_calls == 0);
    /* side 0, slot 0 (lower boundary): asked with (a << 8 | b) and the 0x24ca40 word; rejected,
     * then accepted */
    set_query(0, 0, 1, good, 23);
    q_entries[0][0].unknown_words[0] = word;
    assert(run_kind23(0, 0) == 0 && ea_calls == 1 && wide_calls == 1);
    assert(ea_packed == 0x309 && ea_word == 0x4321 && wide_side == Ta_GetCurrentSide(0, 0) && wide_last == 0x1c);
    assert(run_kind23(1, 0) == 1 && ea_calls == 1);
    /* each failing test skips the slot */
    set_query(0, 0, 0, good, 23);
    q_entries[0][0].unknown_words[0] = word;
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    set_query(0, 0, 1, good, -1);
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    for (bit = 3; bit <= 5; bit++) {
        set_query(0, 0, 1, good & ~(1 << bit), 23);
        q_entries[0][0].unknown_words[0] = word;
        assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    }
    set_query(0, 0, 1, good, 22);
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    set_query(0, 0, 1, good, 24);
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    /* side 1, slot 10 (upper boundary) */
    set_query(0, 0, 0, 0, -1);
    set_query(1, 10, 1, good, 23);
    q_entries[1][10].unknown_words[0] = word;
    assert(run_kind23(1, 0) == 1 && ea_calls == 1 && wide_side == Ta_GetCurrentSide(1, 10));
    assert(Ta_GetCurrentSide(1, 10) != Ta_GetCurrentSide(0, 0));
    assert(run_kind23(0, 0) == 0 && ea_calls == 1);
    /* side 1 skips on each failing test too */
    set_query(1, 10, 0, good, 23);
    q_entries[1][10].unknown_words[0] = word;
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    set_query(1, 10, 1, good, -1);
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    for (bit = 3; bit <= 5; bit++) {
        set_query(1, 10, 1, good & ~(1 << bit), 23);
        q_entries[1][10].unknown_words[0] = word;
        assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    }
    set_query(1, 10, 1, good, 22);
    assert(run_kind23(1, 1) == 0 && ea_calls == 0);
    /* same index on both sides: side 0 is asked first, side 1 after a rejection */
    set_query(0, 4, 1, good, 23);
    q_entries[0][4].unknown_words[0] = word;
    set_query(1, 4, 1, good, 23);
    q_entries[1][4].unknown_words[0] = word;
    assert(run_kind23(1, 0) == 1 && ea_calls == 1 && wide_side == Ta_GetCurrentSide(0, 4));
    assert(run_kind23(0, 1) == 1 && ea_calls == 2 && wide_side == Ta_GetCurrentSide(1, 4));
    memset(q_present, 0, sizeof(q_present));
    memset(q_bits, 0, sizeof(q_bits));
    memset(q_kind, -1, sizeof(q_kind));
    wide_low = saved_low;
    query_mode = 0;
}

unsigned char turnOwner[1];
struct DotrQuerySide data_00317490_query_sides[2];
struct DotrQueryControlEnd data_00317490_query_end;
static float round_args[8], sent[2][4];
static int send_frame = 32, words_seen[4];
static float atan_args[2], atan_result;
static int atan_calls;
static int round_calls, words_calls, send_calls, send_kinds[2];
static int *words_in;
static float *send_data[2];

int func_001a22c0(float value)
{
    round_args[round_calls++] = value;
    return (int)value / 2;
}

void func_001fc950_words(int *in, float *out)
{
    words_in = in;
    memcpy(words_seen, in, sizeof(words_seen));
    words_calls++;
    out[0] = 1.5f; out[1] = 2.5f; out[2] = 3.5f; out[3] = 4.5f;
}

void func_001fa8a0_vec(int a, int b, int kind, unsigned short d, int e, float *data)
{
    assert(a == 0 && b == 0 && d == send_frame && e == 0);
    send_kinds[send_calls] = kind;
    send_data[send_calls] = data;
    memcpy(sent[send_calls++], data, sizeof(sent[0]));
}

static void check_side_start(void)
{
    struct DotrQuerySide *sides = data_00317490_query_sides;
    struct DotrQueryControlEnd *end = &data_00317490_query_end;
    unsigned char *raw;
    int side;

    for (side = 1; side >= 0; side--) {
        memset(sides, 0, sizeof(struct DotrQuerySide) * 2);
        memset(end, 0, sizeof(*end));
        raw = (unsigned char *)&sides[side];
        sides[side].unknown_33e = 5;
        sides[side].unknown_33d = 1;
        sides[side].unknown_345 = -7;
        raw[0x346] = 0x40;
        raw[0x1a2] = raw[0x1a3] = 0x55;
        ((unsigned char *)end)[0xd67] = 0x02;
        turnOwner[0] = 9;
        round_calls = words_calls = send_calls = f830_calls = e80_calls = 0;
        f830_result = side ? 0 : 1; /* 1: the control block replaces the element and
                                     * TaLogic_AdjustLogicData stops early */
        e80_result = 0;
        SzDuel_InitTurn(123, side);
        assert(turnOwner[0] == side && round_calls == 2 && round_args[0] == 100.0f && round_args[1] == -100.0f);
        assert(words_calls == 1 && words_in == sides[side].unknown_1d0);
        assert(send_calls == 2 && send_kinds[0] == 3 && send_kinds[1] == 4 && send_data[0] != send_data[1]);
        assert(sent[0][0] == -50.0f && sent[0][1] == -7.0f && sent[0][2] == 50.0f);
        assert(sent[1][0] == 1.5f && sent[1][1] == 2.5f && sent[1][2] == 3.5f && sent[1][3] == 4.5f);
        assert(raw[0x346] == 0x41 && f830_calls == 2 && f830_first == side);
        if (side) {
            assert(e80_calls == 1 && (void *)e80_arg == (void *)&sides[side] && raw[0x1a2] == 0 && raw[0x1a3] == 0);
        } else {
            assert(e80_calls == 0 && raw[0x1a2] == 0x55 && ((unsigned char *)end)[0x1a2] == 0);
        }
        assert(((unsigned char *)end)[0xd66] == 0x80 && ((unsigned char *)end)[0xd67] == 0x03);
    }
}

float func_001a6c18(float a, float b)
{
    atan_args[0] = a;
    atan_args[1] = b;
    atan_calls++;
    return atan_result;
}

static int lock_words_calls, lock_send_calls, lock_send_kinds[2], lock_send_counts[2];
static void *lock_words_in;
static void *lock_send_data[2];
static float lock_sent[2][4];

void func_001fc950_lockcam(void *in, void *out)
{
    float *o = out;

    lock_words_in = in;
    memcpy(words_seen, in, sizeof(words_seen));
    lock_words_calls++;
    o[0] = 1.5f; o[1] = 2.5f; o[2] = 3.5f; o[3] = 4.5f;
}

int func_001fa8a0_lockcam(int owner, int target, int type, int count, int value, void *data)
{
    assert(owner == 0 && target == 0 && value == 0 && lock_send_calls < 2);
    lock_send_kinds[lock_send_calls] = type;
    lock_send_counts[lock_send_calls] = count;
    lock_send_data[lock_send_calls] = data;
    memcpy(lock_sent[lock_send_calls++], data, sizeof(lock_sent[0]));
    return 0;
}

/* lSzDuel_LockCamera2 (straight-line coverage, run for both sides and on both sides of the angle's
 * rounding step: 128/65536 of a turn rounds up to 256, 127 and below round to 0). Cells
 * (5,1) and (7,3) become (100,-100) and (200,0). */
static void check_camera_lock(void)
{
    struct DotrQuerySide *sides = data_00317490_query_sides;
    static const float turns[3] = { 6.2831855f / 4, 6.2831855f / 512, 6.2831855f / 1024 };
    static const int words[3] = { 16384 + 16384, 256 + 16384, 16384 };
    int side;
    int t;

    for (t = 0; t < 3; t++) {
        side = t & 1;
        memset(sides, 0, sizeof(struct DotrQuerySide) * 2);
        sides[side].unknown_1d0[0] = 11;
        sides[side].unknown_1d0[1] = 22;
        sides[side].unknown_1d0[2] = 33;
        sides[side].unknown_1d0[3] = 44;
        sides[side].unknown_345 = -7;
        turnOwner[0] = side;
        lock_words_calls = lock_send_calls = atan_calls = 0;
        atan_result = turns[t];
        lSzDuel_LockCamera2(0x10005, 5, 1, 7, 3);
        assert(atan_calls == 1 && atan_args[0] == 100.0f && atan_args[1] == 100.0f);
        assert(lock_words_calls == 1 && lock_words_in == sides[side].unknown_1d0);
        assert(words_seen[0] == 150 && words_seen[1] == words[t]);
        assert(words_seen[2] == 0x4000 && words_seen[3] == 44);
        assert(lock_send_calls == 2 && lock_send_kinds[0] == 3 && lock_send_kinds[1] == 4);
        assert(lock_send_counts[0] == 5 && lock_send_counts[1] == 5 && lock_send_data[0] != lock_send_data[1]);
        assert(lock_sent[0][0] == 150.0f && lock_sent[0][1] == -7.0f && lock_sent[0][2] == -50.0f);
        assert(lock_sent[1][0] == 1.5f && lock_sent[1][1] == 2.5f && lock_sent[1][2] == 3.5f && lock_sent[1][3] == 4.5f);
        assert(sides[side].unknown_1d0[0] == 11 && sides[side].unknown_1d0[1] == 22);
        assert(sides[side].unknown_1d0[2] == 33 && sides[side].unknown_1d0[3] == 44);
        assert(sides[!side].unknown_1d0[0] == 0);
    }
}

signed char passwordKeyHannibal[8] = { 3, -5, 7, -128, 127, 0, -1, 100 };

/* Branch coverage. Gives entry (id 40) or a node entry (ids 100..107) the bytes that
 * encode key for that id. */
static void code_entry(struct DotrStateEntry20 *raw, int id, const unsigned char *key, int coded)
{
    struct DotrCodeEntry *e = (struct DotrCodeEntry *)raw;
    int i;

    memset(raw, 0, sizeof(*raw));
    ((unsigned char *)raw)[9] = 0xbf;
    e->coded = coded;
    for (i = 0; i < 8; i++) {
        e->code[i] = (unsigned char)(key[i] - passwordKeyHannibal[(id + i) % 8]);
    }
}

static void check_code_search(void)
{
    static const unsigned char key[8] = { 0, 1, 0x7f, 0x80, 0xff, 200, 4, 130 };
    struct DotrStateEntry20 saved = entry;
    struct DotrStateEntry20 saved_nodes[8];
    int i;

    memcpy(saved_nodes, node_entries, sizeof(saved_nodes));
    for (i = 0; i < 8; i++) {
        code_entry(&node_entries[i], 100 + i, key, 0);
    }
    /* Matching bytes without the bit are skipped; entries the callee does not have too. */
    code_entry(&entry, 40, key, 0);
    assert(lSzDuel_HitPassword((unsigned char *)key) == -1);
    /* The bit and all eight bytes (sums wrap modulo 256 both ways): the first such id. */
    code_entry(&entry, 40, key, 1);
    ((struct DotrCodeEntry *)&node_entries[2])->coded = 1;
    assert(lSzDuel_HitPassword((unsigned char *)key) == 40);
    /* A difference in the last or the first byte moves on to the next id. */
    ((struct DotrCodeEntry *)&entry)->code[7]++;
    assert(lSzDuel_HitPassword((unsigned char *)key) == 102);
    ((struct DotrCodeEntry *)&node_entries[2])->code[0]--;
    assert(lSzDuel_HitPassword((unsigned char *)key) == -1);
    /* The shift index depends on the id: id 40's bytes do not match under id 107. */
    code_entry(&node_entries[7], 40, key, 1);
    assert(lSzDuel_HitPassword((unsigned char *)key) == -1);
    code_entry(&node_entries[7], 107, key, 1);
    assert(lSzDuel_HitPassword((unsigned char *)key) == 107);
    entry = saved;
    memcpy(node_entries, saved_nodes, sizeof(saved_nodes));
}

int main(void)
{
    check_slot_answer();
    check_cell_scan();
    check_wide_queries();
    check_kind23_find();
    check_rank_query();
    check_pick();
    int have;
    check_sums();
    check_slots();
    check_item_gates();
    check_grid_queries();
    check_release();
    check_list_classes();
    check_item_difference();
    check_nodes();
    check_gates_and_presets();
    check_wrapped_offsets();
    check_kind_queries();
    check_list_counts();
    for (have = 4; have <= 6; have++) {
        need_result = 5;
        have_result = have;
        assert(TaHand_IsCardSummon2(1, 0x12345) == (have >= 5) && need_args[0] == 0x12345 && need_args[1] == 1);
        assert(have_argument == 1);
        assert(TaHand_IsCardSummon(0, 0x12345) == (have >= 5) && need_args[0] == 0x345 && need_args[1] == 0);
        assert(have_argument == 0);
    }
    memset(&entry, 0, sizeof(entry));
    ((unsigned char *)&entry)[0] = 0xc7;
    ((unsigned short *)&entry)[4] = 0xe123;
    ((unsigned short *)&entry)[5] = 0xffff;
    assert(TaHand_GetCardAPDP(999) == 0 && TaHand_GetCardAPDP(41) == 0 && TaHand_GetCardAPDP(40) == 0x01231fff);
    assert(TaHand_GetCardKind(999) == 0 && TaHand_GetCardKind(41) == 0 && TaHand_GetCardKind(40) == 0xc7);
    counts[0] = 1;
    counts[1] = 2;
    assert(TaHand_GetNextHandNum(1) == 3);
    counts[1] = 4;
    assert(TaHand_GetNextHandNum(0) == 5);
    counts[1] = 400;
    assert(TaHand_GetNextHandNum(1) == 5);
    counts[1] = -400;
    assert(TaHand_GetNextHandNum(0) == -399);
    check_kind_sum();
    check_vote();
    check_best_candidate();
    check_query_gate();
    check_solo_batch();
    check_solo_batch_2();
    check_solo_batch_3();
    check_wide_forward();
    check_score_difference();
    check_score_class();
    check_word_store_modes();
    check_kind_mask();
    check_id_set();
    check_slot_picks(TaLogic_ConvEmptyCombo__00136d60, 1000, 0);
    check_slot_picks(TaLogic_ConvEmptyCombo__0011a2f0, 3500, 0);
    check_slot_picks(TaLogic_ConvEmptyCombo__0010e6b0, 3500, 1);
    check_slot_picks(TaLogic_ConvEmptyCombo__001700d0, 4000, 2);
    check_kind_state_veto(TaLogic001_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic010_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic013_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic014_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic015_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic019_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic020_IsRefuseEmptyCombo);
    check_kind_state_veto(TaLogic021_IsRefuseEmptyCombo);
    check_slot_picks(TaLogic_ConvEmptyCombo__00114f20, 5000, 2);
    check_slot_picks(TaLogic_ConvEmptyCombo__0013b600, 4000, 2);
    check_slot_picks(TaLogic_ConvEmptyCombo__00148d40, 4200, 2);
    check_slot_picks(TaLogic_ConvEmptyCombo__0014d5c0, 4200, 2);
    check_slot_picks(TaLogic_ConvEmptyCombo__00151880, 3500, 2);
    check_slot_picks(TaLogic_ConvEmptyCombo__00160030, 4000, 2);
    check_slot_picks_zero(TaLogic_ConvEmptyCombo__00164ec0);
    check_slot_picks_plain(TaLogic_ConvEmptyCombo__001566d0);
    check_slot_picks_zero(TaLogic_ConvEmptyCombo__001691e0);
    check_remaining_pick_copies();
    check_two_step(TaLogic003_IsOpenMoveUnit);
    check_two_step(TaLogic005_IsOpenMoveUnit);
    check_two_step(TaLogic006_IsOpenMoveUnit);
    check_two_step(TaLogic007_IsOpenMoveUnit);
    check_two_step(TaLogic008_IsOpenMoveUnit);
    check_two_step(TaLogic009_IsOpenMoveUnit);
    check_two_step(TaLogic010_IsOpenMoveUnit);
    check_two_step(TaLogic011_IsOpenMoveUnit);
    check_two_step(TaLogic012_IsOpenMoveUnit);
    check_two_step(TaLogic013_IsOpenMoveUnit);
    check_two_step(TaLogic014_IsOpenMoveUnit);
    check_two_step(TaLogic015_IsOpenMoveUnit);
    check_two_step(TaLogic018_IsOpenMoveUnit);
    check_two_step(TaLogic019_IsOpenMoveUnit);
    check_two_step(TaLogic020_IsOpenMoveUnit);
    check_two_step(TaLogic021_IsOpenMoveUnit);
    check_two_step(TaLogic022_IsOpenMoveUnit);
    check_id_54_57(TaLogicCom_IsExzParts__00112960);
    check_id_54_57(TaLogic017_IsExzParts);
    check_id_54_57(TaLogicCom_IsExzParts__0016cf10);
    check_side_set(TaLogic_IsTactixExz__00112980);
    check_side_set(TaLogic_IsTactixExz__0016cf30);
    check_side_start();
    check_code_search();
    check_kind11_walks();
    check_camera_lock();
    puts("state_queries: ok");
    return 0;
}

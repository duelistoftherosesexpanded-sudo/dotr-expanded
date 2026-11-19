/* Synthetic host tests for the grid_scans group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/grid_scans.h"

static int item_dummy;
static int taken[8][8];          /* 0x1e36e0 answers non-null where set */
static int item_calls;
static int cell[8][8];           /* 0x1e3790 answers */
static int grid[8][8];           /* 0x1e22b0 answers */
static int gate_v, gate_calls;
static int hand_count[2], hand_ids[2][8], hand_reads;   /* the two graveyard lists */
static struct DotrScanRecord record;
static int record_null, record_calls;
static int bit_v, bit_calls;
static int side_calls, cell_calls, step_v, step_calls;
static int pair_calls, spot_calls;
static int spots[5];

struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(x >= 0 && x < 8 && y >= 0 && y < 8);
    item_calls++;
    return taken[x][y] ? (struct DotrStateItem *)&item_dummy : NULL;
}
int SzDuel_GetMapUnitID(int x, int y)
{
    assert(x >= 0 && x < 8 && y >= 0 && y < 8);
    return cell[x][y];
}
int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 8 && y >= 0 && y < 8);
    return grid[x][y];
}
int SzDuel_GetUnitSpellCount(int side)
{
    assert(side == 0 || side == 1 || side == 2);
    gate_calls++;
    return gate_v;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    cell_calls++;
    return 0x7777;
}
int SzDuel_GetUnitStatus(int a, int b, int bit)
{
    assert(a == 0x12 && b == 0x34 && bit == 0);
    bit_calls++;
    return bit_v;
}
int SzDuel_GetGraveCount(int index)
{
    assert(index == 0 || index == 1);
    return hand_count[index];
}
int SzDuel_InqGrave(int index, int position)
{
    assert((index == 0 || index == 1) && position >= 0 && position < hand_count[index]);
    hand_reads++;
    return hand_ids[index][position];
}
struct DotrScanRecord *func_001df540_scan(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    record_calls++;
    return record_null ? NULL : &record;
}
int Ta_GetCurrentSide(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    side_calls++;
    return 0x5555;
}
int CheckLandAgreement2(int a, int b, int c)
{
    assert(a == 0x4d && b == 0x12 && c == 0x34);
    step_calls++;
    return step_v;
}
/* Side of a cell answer: its low byte's bit 0. */
int func_00250360_pair(int a, int b)
{
    assert(a >= 0 && a < 0x100);
    pair_calls++;
    return b & 1;
}
int TaLogic020_GetLinePiece(int index)
{
    assert(index == spot_calls && index < 5);
    spot_calls++;
    return spots[index];
}

static int kinds[16];            /* 0x26fce0 answers for ids 0x100..0x10f */
static int kind_v, block_seen[24], mark_id, side_v, fit_v, fit_calls, fit_block_ok;
static int has_by[4], has_calls, own_by[4], own_calls;

static int held_mode, held_id, held_calls;
static struct DotrScanEntry held_entry[16];
static struct DotrScanPair held_pair[16];

int TaHand_GetCardKind(int id)
{
    if (id >= 0x100 && id < 0x110) {
        return kinds[id - 0x100];
    }
    assert(id == 0x2bc);
    return kind_v;
}
void func_001dcd70_item(int *block, int id)
{
    int i;

    if (held_mode) {
        /* 0x166ee0: the block's first word points at the id's entry */
        assert(id >= 0x100 && id < 0x110);
        ((struct DotrScanHeld *)block)->entry = &held_entry[id - 0x100];
        held_id = id;
        held_calls++;
        return;
    }
    for (i = 0; i < 24; i++) {
        block[i] = 0x51000000 + i;
    }
    mark_id = id;
}
int SzDuel_GetTurnOwner(void)
{
    return side_v;
}
int CheckLandAgreement(int a, struct DotrStateItem *item)
{
    int i;

    assert(a == 0x4d);
    memcpy(block_seen, item, sizeof block_seen);
    fit_block_ok = 1;
    for (i = 0; i < 24; i++) {
        fit_block_ok &= block_seen[i] == 0x51000000 + i;
    }
    fit_calls++;
    return fit_v;
}
static const int four_ids[4] = { 811, 805, 821, 824 };
static const int gain_masks[3] = { 0x0111001c, 0x001e001c, 0x000b001c };
static int gain_mode, gain_last, gain_count[2], gain_by[2][4][2], try_calls;
int SearchUnitByRange(int a, int b, int c, int d, int e, int f)
{
    if (gain_mode) {
        /* 0x14aca0: three sets in order, answered 0x200, 0x201 and gain_last */
        assert(has_calls < 3 && a == 0 && b == 0 && c == side_v && d == 1 && f == 14);
        assert(e == gain_masks[has_calls]);
        has_calls++;
        return has_calls == 3 ? gain_last : 0x1ff + has_calls;
    }
    assert(has_calls < 4 && a == 0 && b == 0 && c == 1 && d == 1 && f == 14);
    assert(e == (four_ids[has_calls] << 16 | 0x1c));
    return has_by[has_calls++];
}
int TaLogic_GetGraveCardCount(int index, int flags, int id)
{
    assert(has_calls >= 1 && index == 1 && flags == 1 && id == four_ids[has_calls - 1]);
    own_calls++;
    return own_by[has_calls - 1];
}

static int entry_dummy[2], entry_calls, pair_null, pair_calls2;
static struct DotrScanPair pair_dummy;

struct DotrScanEntry *func_001dd0e0_scan(int id)
{
    assert(id >= 0x100 && id < 0x110);
    entry_calls++;
    return (struct DotrScanEntry *)entry_dummy;
}
struct DotrScanPair *func_0026fc90_scan(int id, int b)
{
    assert(id >= 0x100 && id < 0x110 && b == 0);
    pair_calls2++;
    return pair_null ? NULL : &pair_dummy;
}

/* TaLogic017_GetEmptyExzPiece (loop coverage): each of the four spots as the first free one, and none. */
static void check_spots(void)
{
    static const int want[4] = { 0x303, 0x404, 0x402, 0x503 };
    int i, j;

    for (i = 0; i <= 4; i++) {
        memset(taken, 0, sizeof taken);
        for (j = 0; j < i; j++) {
            taken[want[j] & 0xff][want[j] >> 8] = 1;
        }
        item_calls = 0;
        assert(TaLogic017_GetEmptyExzPiece() == (i < 4 ? want[i] : -1));
        assert(item_calls == (i < 4 ? i + 1 : 4));
    }
}

static int hand(int side, int gate, int n0, int at0, int n1, int at1)
{
    int i;

    gate_v = gate;
    gate_calls = hand_reads = 0;
    hand_count[0] = n0;
    hand_count[1] = n1;
    for (i = 0; i < 8; i++) {
        hand_ids[0][i] = i == at0 ? 713 : 712;
        hand_ids[1][i] = i == at1 ? 713 : 714;
    }
    return TaLogic009_IsUseCopyCat(side);
}

/* TaLogic009_IsUseCopyCat (branch and loop coverage). */
static void check_hand(void)
{
    /* Gate boundary: 5 answers -1 without reading a slot, 4 goes on. */
    assert(hand(0, 5, 3, 0, 3, 0) == -1 && gate_calls == 1 && hand_reads == 0);
    assert(hand(0, 4, 3, 0, 3, 0) == 0 && hand_reads == 1);
    /* Found on the own side at the first, a middle and the last slot. */
    assert(hand(1, 0, 3, 9, 4, 2) == (1 << 16 | 2) && hand_reads == 3);
    assert(hand(1, 0, 3, 0, 4, 3) == (1 << 16 | 3) && hand_reads == 4);
    /* Own side empty or without the id: the other side (1 for 0, 0 for 1). */
    assert(hand(0, 0, 0, 9, 4, 1) == (1 << 16 | 1) && hand_reads == 2);
    assert(hand(0, 0, 2, 9, 4, 3) == (1 << 16 | 3) && hand_reads == 6);
    assert(hand(1, 0, 3, 2, 2, 9) == 2 && hand_reads == 5);
    /* Neither side has it; both sides empty. */
    assert(hand(0, 0, 2, 9, 3, 9) == -1 && hand_reads == 5);
    assert(hand(1, 0, 0, 9, 0, 9) == -1 && hand_reads == 0);
}

static int step(int x, int y, int to_x, int to_y, int kind, int null, int bit, int answer)
{
    record_null = null;
    bit_v = bit;
    step_v = answer;
    record_calls = bit_calls = step_calls = 0;
    memset(grid, 0, sizeof grid);
    grid[x][y] = 0x4d;
    return TaLogic004_IsOpenMoveUnit(0x12, 0x34, x, y, to_x, to_y, kind);
}

/* TaLogic004_IsOpenMoveUnit (branch coverage). */
static void check_step(void)
{
    /* Every test passes: two steps in x, in y (both directions) and one of each. */
    assert(step(1, 3, 3, 3, 2, 0, 0, 1) == 1 && record_calls == 1 && bit_calls == 1 && step_calls == 1);
    assert(step(3, 3, 1, 3, 2, 0, 0, 1) == 1);
    assert(step(3, 1, 3, 3, 2, 0, 0, 1) == 1);
    assert(step(3, 3, 3, 1, 2, 0, 0, 1) == 1);
    assert(step(2, 2, 3, 3, 2, 0, 0, 1) == 1);
    assert(step(3, 3, 2, 2, 2, 0, 0, 1) == 1);
    /* Distance 1 and 3: nothing is called. */
    assert(step(2, 3, 3, 3, 2, 0, 0, 1) == 0 && record_calls == 0);
    assert(step(1, 2, 3, 3, 2, 0, 0, 1) == 0 && record_calls == 0);
    /* Kind 1 and 3. */
    assert(step(1, 3, 3, 3, 1, 0, 0, 1) == 0 && record_calls == 0);
    assert(step(1, 3, 3, 3, 3, 0, 0, 1) == 0 && record_calls == 0);
    /* Null record; bit set; last answer 0 and 2. */
    assert(step(1, 3, 3, 3, 2, 1, 0, 1) == 0 && record_calls == 1 && bit_calls == 0);
    assert(step(1, 3, 3, 3, 2, 0, 1, 1) == 0 && bit_calls == 1 && step_calls == 0);
    assert(step(1, 3, 3, 3, 2, 0, 0, 0) == 0 && step_calls == 1);
    assert(step(1, 3, 3, 3, 2, 0, 0, 2) == 0 && step_calls == 1);
}

static int row(unsigned short *out, int side, int mask)
{
    int x;

    for (x = 0; x < 8; x++) {
        out[x] = 0xeeee;
    }
    pair_calls = 0;
    return TaLogic012_GetLineUnitLst(out, 5, side, mask);
}

/* TaLogic012_GetLineUnitLst (branch and loop coverage). */
static void check_row(void)
{
    unsigned short out[8];
    int x;

    /* Row 5: x 0 side 1, x 1 empty, x 2 side 0, x 3..5 empty, x 6 side 1. */
    for (x = 0; x < 8; x++) {
        cell[x][5] = -1;
        cell[x][4] = 0x0101;
    }
    cell[0][5] = 0x0201;
    cell[2][5] = 0x0304;
    cell[6][5] = 0x0603;
    /* Mask 0: nothing stored, no side query. */
    assert(row(out, 1, 0) == 0 && pair_calls == 0 && out[0] == 0xeeee);
    /* Bit 0: cells of the side. */
    assert(row(out, 1, 1) == 2 && pair_calls == 3 && out[0] == 0x0201 && out[1] == 0x0603 && out[2] == 0xeeee);
    assert(row(out, 0, 1) == 1 && out[0] == 0x0304 && out[1] == 0xeeee);
    /* Bit 1: cells of the other side. */
    assert(row(out, 1, 2) == 1 && pair_calls == 3 && out[0] == 0x0304 && out[1] == 0xeeee);
    assert(row(out, 0, 2) == 2 && out[0] == 0x0201 && out[1] == 0x0603);
    /* Both bits: every occupied cell once, in x order; higher bits ignored. */
    assert(row(out, 1, 3) == 3 && pair_calls == 6 && out[0] == 0x0201 && out[1] == 0x0304 && out[2] == 0x0603 && out[3] == 0xeeee);
    assert(row(out, 1, 0xfd) == 2 && out[1] == 0x0603);
    /* An empty row. */
    for (x = 0; x < 8; x++) {
        cell[x][5] = -1;
    }
    assert(row(out, 1, 3) == 0 && pair_calls == 0);
}

static void order_run(struct DotrScanOrder *order, int null, int key)
{
    memset(order, 0, sizeof *order);
    order->position = 0x1234;
    order->unknown_02 = 0x2222;
    order->unknown_04 = 0x4444;
    order->unknown_06 = 0x6666;
    order->flags.word = 0x7abc1def;
    record_null = null;
    record.unknown_3c = (unsigned short)key;
    record_calls = side_calls = cell_calls = spot_calls = 0;
    TaLogic020_CommandSpellUnit(order);
    assert(order->position == 0x1234 && order->unknown_02 == 0x2222 && order->unknown_06 == 0x6666);
}

/* TaLogic020_CommandSpellUnit (branch and loop coverage). */
static void check_order(void)
{
    struct DotrScanOrder order;
    int i;

    for (i = 0; i < 5; i++) {
        spots[i] = (i + 1) << 8 | i;      /* x = i, y = i + 1 */
    }
    memset(cell, 0, sizeof cell);
    for (i = 0; i < 5; i++) {
        cell[i][i + 1] = 0x0707;          /* another occupant */
    }
    /* Null record: nothing called after it, nothing stored. */
    order_run(&order, 1, 688);
    assert(record_calls == 1 && side_calls == 0 && cell_calls == 0 && spot_calls == 0);
    assert(order.unknown_04 == 0x4444 && order.flags.word == 0x7abc1def);
    /* Key 688 and its neighbours; no spot qualifies: the +8 word is kept. */
    order_run(&order, 0, 688);
    assert(record_calls == 2 && side_calls == 1 && cell_calls == 1 && spot_calls == 5);
    assert(order.unknown_04 == 0xcff && order.flags.word == 0x7abc1def);
    order_run(&order, 0, 687);
    assert(order.unknown_04 == 0x3ff && order.flags.word == 0x7abc1def);
    order_run(&order, 0, 689);
    assert(order.unknown_04 == 0x3ff);
    /* The order's own position at spot 2; the upper halfword of the spot is dropped. */
    cell[2][3] = 0x1234;
    spots[2] |= 0x50000;
    order_run(&order, 0, 0);
    assert(spot_calls == 3 && order.flags.word == (0x0302 << 16 | 0x1def));
    /* A negative answer at spot 0 wins first; at the last spot. */
    cell[0][1] = -1;
    order_run(&order, 0, 0);
    assert(spot_calls == 1 && order.flags.word == (0x0100 << 16 | 0x1def));
    cell[0][1] = 0x0707;
    cell[2][3] = 0x0707;
    cell[4][5] = -5;
    order_run(&order, 0, 0);
    assert(spot_calls == 5 && order.flags.word == (0x0504 << 16 | 0x1def));
}

static int fit(int kind, int side, int answer)
{
    kind_v = kind;
    side_v = side;
    fit_v = answer;
    fit_calls = fit_block_ok = 0;
    mark_id = 0;
    pair_calls = 0;
    memset(grid, 0, sizeof grid);
    grid[0][5] = 0x4d;
    return TaLogic012_IsLineEmpty(0x2bc, 5);
}

/* TaLogic012_IsLineEmpty (branch and loop coverage). */
static void check_fit(void)
{
    int x;

    for (x = 0; x < 8; x++) {
        cell[x][5] = -1;
    }
    /* Kind byte with a top bit: 0 after the block fill, nothing else called; bits above the byte
     * are ignored. */
    assert(fit(0x20, 1, 1) == 0 && mark_id == 0x2bc && fit_calls == 0);
    assert(fit(0x80, 1, 1) == 0 && fit_calls == 0);
    assert(fit(0x11f, 1, 1) == 1 && fit_calls == 1 && fit_block_ok == 1 && pair_calls == 0);
    /* Empty row: the last answer decides (0, 1, 2). */
    assert(fit(0x1f, 1, 0) == 0 && fit_calls == 1);
    assert(fit(0, 1, 2) == 0 && fit_calls == 1);
    /* Only other-side cells in the row (side is the answer's bit 0): still asked. */
    cell[0][5] = 0x0200;
    cell[6][5] = 0x0304;
    assert(fit(0, 1, 1) == 1 && pair_calls == 2 && fit_calls == 1);
    /* One own cell at the last x, at the first x: 0 without the last call. */
    assert(fit(0, 0, 1) == 0 && pair_calls == 2 && fit_calls == 0);
    cell[6][5] = -1;
    cell[0][5] = 0x0201;
    assert(fit(0, 1, 1) == 0 && pair_calls == 1 && fit_calls == 0);
    assert(fit(0, 0, 1) == 1 && fit_calls == 1);
}

static int four(int (*f)(int), int has, int own)
{
    int i;

    for (i = 0; i < 4; i++) {
        has_by[i] = has >> i & 1;
        own_by[i] = own >> i & 1 ? 7 : 0;
    }
    has_calls = own_calls = 0;
    return f(1);
}

/* 0x158810 and its thunk TaLogic017_IsExzSafe (branch coverage / forwarder). */
static void check_four(int (*f)(int))
{
    int i;

    /* Every first query answers: 1 with no second query. */
    assert(four(f, 15, 0) == 1 && has_calls == 4 && own_calls == 0);
    /* Every first query fails but the second answers. */
    assert(four(f, 0, 15) == 1 && has_calls == 4 && own_calls == 4);
    /* Both fail at id i: 0 right there. */
    for (i = 0; i < 4; i++) {
        assert(four(f, 15 & ~(1 << i), 0) == 0 && has_calls == i + 1 && own_calls == 1);
        assert(four(f, 0, 15 & ~(1 << i)) == 0 && has_calls == i + 1 && own_calls == i + 1);
        assert(four(f, 15 & ~(1 << i), 1 << i) == 1 && has_calls == 4 && own_calls == 1);
    }
}

static int hand_kinds(int gate, int n, int null)
{
    int i;

    gate_v = gate;
    gate_calls = hand_reads = entry_calls = pair_calls2 = 0;
    hand_count[0] = 0;
    hand_count[1] = n;
    for (i = 0; i < 8; i++) {
        hand_ids[1][i] = 0x100 + i;
    }
    pair_null = null;
    return TaLogic007_IsRevivePlay(1);
}

/* TaLogic007_IsRevivePlay (branch and loop coverage, short of the null read). */
static void check_hand_kinds(void)
{
    memset(kinds, 0, sizeof kinds);
    /* Gate 5: -1 at once; gate 4 with an empty graveyard: -1. */
    assert(hand_kinds(5, 3, 1) == -1 && gate_calls == 1 && hand_reads == 0);
    assert(hand_kinds(4, 0, 1) == -1 && gate_calls == 1 && hand_reads == 0);
    /* Kind 0x1f (>> 5 is 0) in every slot: no entry or pair lookup. */
    kinds[0] = kinds[1] = kinds[2] = 0x1f;
    assert(hand_kinds(0, 3, 1) == -1 && hand_reads == 3 && entry_calls == 0 && pair_calls2 == 0);
    /* Kind 0x20 in the middle slot: both lookups run; a null pair goes on. */
    kinds[1] = 0x20;
    assert(hand_kinds(0, 3, 1) == -1 && hand_reads == 3 && entry_calls == 1 && pair_calls2 == 1 && gate_calls == 1);
    /* A pair with a non-null entry goes on too, without the second gate call. */
    assert(hand_kinds(0, 3, 0) == -1 && hand_reads == 3 && entry_calls == 1 && pair_calls2 == 1 && gate_calls == 1);
}

int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    assert(value == 0x200 || value == 0x201);
    return gain_count[value & 1];
}
int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert((word == 0x200 || word == 0x201) && n >= 0 && n < gain_count[word & 1]);
    return (0x30 + (word & 1)) << 8 | n;
}
int func_001e0290_try(int a, int b, int flag)
{
    assert((a == 0x30 || a == 0x31) && b >= 0 && b < 4 && flag == (try_calls & 1));
    try_calls++;
    return gain_by[a & 1][b][flag];
}

static int gain(int last)
{
    int r;

    gain_mode = 1;
    gain_last = last;
    has_calls = try_calls = 0;
    r = TaLogic013_IsActiveMode();
    gain_mode = 0;
    return r;
}

/* TaLogic013_IsActiveMode (branch and loop coverage). */
static void check_gain(void)
{
    side_v = 1;
    memset(gain_by, 0, sizeof gain_by);
    /* both sets empty: the third set decides */
    gain_count[0] = gain_count[1] = 0;
    assert(gain(0) == 0 && has_calls == 3 && try_calls == 0);
    assert(gain(0x300) == 1 && has_calls == 3 && try_calls == 0);
    /* equal answers and a higher second answer are not a gain */
    gain_count[0] = 3, gain_count[1] = 2;
    gain_by[0][1][0] = 5, gain_by[0][1][1] = 6;
    gain_by[1][0][0] = -2, gain_by[1][0][1] = -2;
    side_v = 0;
    assert(gain(0) == 0 && has_calls == 3 && try_calls == 10);
    /* past the first set's count the gain is not reached */
    gain_by[0][3][0] = 9, gain_by[0][3][1] = 8;
    assert(gain(0) == 0 && try_calls == 10);
    /* last entry of the first set: 1 without the later sets */
    gain_by[0][2][0] = 9, gain_by[0][2][1] = 8;
    assert(gain(0) == 1 && has_calls == 1 && try_calls == 6);
    /* first entry of the second set */
    gain_by[0][2][1] = 9;
    gain_by[1][0][1] = -3;
    assert(gain(0) == 1 && has_calls == 2 && try_calls == 8);
    gain_by[1][0][1] = -2;
    gain_by[1][1][0] = 1;
    assert(gain(0) == 1 && has_calls == 2 && try_calls == 10);
    side_v = 0;
}

struct DotrScanPair *func_001e09e0_scan(struct DotrScanHeld *held, int n)
{
    assert(n == 0 && held->entry == &held_entry[held_id - 0x100]);
    return &held_pair[held_id - 0x100];
}

static int held(int side, int gate)
{
    int r;

    gate_v = gate;
    gate_calls = hand_reads = held_calls = 0;
    held_mode = 1;
    r = TaLogic020_IsUseRevive(side);
    held_mode = 0;
    return r;
}

/* TaLogic020_IsUseRevive (branch and loop coverage). */
static void check_held(void)
{
    int i;

    memset(kinds, 0, sizeof kinds);
    memset(held_entry, 0, sizeof held_entry);
    memset(held_pair, 0, sizeof held_pair);
    for (i = 0; i < 3; i++) {
        hand_ids[0][i] = 0x100 + i;
        hand_ids[1][i] = 0x108 + i;
        kinds[i] = kinds[8 + i] = 1 << 5;
        held_entry[i].deckCost = held_entry[8 + i].deckCost = 39;
    }
    hand_count[0] = 3, hand_count[1] = 2;
    /* Gate 5: -1 at once; gate 4 goes on. Entries of 39: nothing taken. */
    assert(held(0, 5) == -1 && gate_calls == 1 && hand_reads == 0);
    assert(held(0, 4) == -1 && hand_reads == 5 && held_calls == 5);
    /* Empty graveyards. */
    hand_count[0] = hand_count[1] = 0;
    assert(held(1, 0) == -1 && hand_reads == 0);
    hand_count[0] = 3, hand_count[1] = 2;
    /* Kind >> 5 of 0: the block is not filled. */
    kinds[1] = 0x1f, kinds[9] = 0;
    assert(held(0, 0) == -1 && hand_reads == 5 && held_calls == 3);
    /* 40 is taken; an equal later slot replaces it; word >> 8 of 20 does not. */
    held_entry[0].deckCost = 40;
    assert(held(0, 0) == (0 << 16 | 0));
    held_entry[2].deckCost = 40;
    assert(held(0, 0) == (0 << 16 | 2));
    held_pair[2].word = 20 << 8 | 0xff;
    assert(held(0, 0) == (0 << 16 | 0));
    held_pair[2].word = 0x01000000 | 20 << 8;
    assert(held(0, 0) == (0 << 16 | 2));
    /* The other side's slots follow: a lower entry does not replace, an equal one does. */
    held_entry[2].deckCost = 41;
    held_entry[8].deckCost = 40;
    assert(held(0, 0) == (0 << 16 | 2));
    held_entry[8].deckCost = 41;
    assert(held(0, 0) == (1 << 16 | 0));
    /* From side 1 the walk starts with side 1's slots and ends with side 0's. */
    assert(held(1, 0) == (0 << 16 | 2));
    held_entry[2].deckCost = 40;
    assert(held(1, 0) == (1 << 16 | 0));
    /* The count bounds the walk. */
    hand_count[0] = 0, hand_count[1] = 0;
    assert(held(1, 0) == -1 && held_calls == 0);
}

static int held_other(int side, int gate)
{
    int r;

    gate_v = gate;
    gate_calls = hand_reads = held_calls = 0;
    held_mode = 1;
    r = TaLogic015_IsUseRevive(side);
    held_mode = 0;
    return r;
}

/* TaLogic015_IsUseRevive (branch and loop coverage). */
static void check_held_other(void)
{
    int i;

    memset(kinds, 0, sizeof kinds);
    memset(held_entry, 0, sizeof held_entry);
    memset(held_pair, 0, sizeof held_pair);
    for (i = 0; i < 3; i++) {
        hand_ids[0][i] = 0x100 + i;
        hand_ids[1][i] = 0x108 + i;
        kinds[i] = kinds[8 + i] = 1 << 5;
        held_entry[i].deckCost = 50;
        held_entry[8 + i].deckCost = 39;
    }
    hand_count[0] = 3, hand_count[1] = 2;
    /* Gate 5: -1 at once; gate 4 walks only the other side's two slots. */
    assert(held_other(0, 5) == -1 && gate_calls == 1 && hand_reads == 0);
    assert(held_other(0, 4) == -1 && hand_reads == 2 && held_calls == 2);
    /* Kind >> 5 of 0: the block is not filled. */
    kinds[9] = 0x1f;
    assert(held_other(0, 0) == -1 && hand_reads == 2 && held_calls == 1);
    kinds[9] = 1 << 5;
    /* 40 is taken; an equal later slot replaces it; word >> 8 of 20 does not. */
    held_entry[8].deckCost = 40;
    assert(held_other(0, 0) == (1 << 16 | 0));
    held_entry[9].deckCost = 40;
    assert(held_other(0, 0) == (1 << 16 | 1));
    held_pair[9].word = 20 << 8 | 0xff;
    assert(held_other(0, 0) == (1 << 16 | 0));
    held_pair[9].word = 0x01000000 | 20 << 8;
    assert(held_other(0, 0) == (1 << 16 | 1));
    /* From side 1 the walk is over side 0's three slots; the last equal one wins. */
    assert(held_other(1, 0) == (0 << 16 | 2) && hand_reads == 3);
    /* The count bounds the walk. */
    hand_count[0] = 0;
    assert(held_other(1, 0) == -1 && held_calls == 0);
}

static int parity_v, parity_calls, low_gate_v, low_gate_calls, packed_v, packed_mask, worth_v, worth_calls;

int func_0019a5c8(void)
{
    parity_calls++;
    return parity_v;
}

int SzDuel_GetUnitMonsterCount(int side)
{
    assert(side == 0 || side == 1);
    low_gate_calls++;
    return low_gate_v;
}

int TaLogic_GetMaxAPGraveCard(int side, int mask)
{
    assert((side == 0 || side == 1) && mask == packed_mask);
    return packed_v;
}

int SzDuel_GetUnitAP2(struct DotrStateItem *item, int flag)
{
    assert(item != NULL && flag == 0);
    worth_calls++;
    return worth_v;
}

static int held_mode_scan(int side, int mask, int mode)
{
    int r;

    packed_mask = mask;
    gate_calls = hand_reads = held_calls = 0;
    parity_calls = low_gate_calls = worth_calls = 0;
    held_mode = 1;
    r = TaLogicCom_IsUseRevive(side, mask, mode);
    held_mode = 0;
    return r;
}

/* TaLogicCom_IsUseRevive (branch and loop coverage). */
static void check_held_modes(void)
{
    int i;

    memset(kinds, 0, sizeof kinds);
    memset(held_entry, 0, sizeof held_entry);
    memset(held_pair, 0, sizeof held_pair);
    for (i = 0; i < 3; i++) {
        hand_ids[0][i] = 0x100 + i;
        hand_ids[1][i] = 0x108 + i;
        kinds[i] = kinds[8 + i] = 1 << 5;
        held_entry[i].deckCost = held_entry[8 + i].deckCost = 39;
    }
    hand_count[0] = 3, hand_count[1] = 3;
    gate_v = 4;

    /* Mode 29: low gate of 5 answers -1 at once. */
    packed_v = 0x01020000;
    low_gate_v = 5;
    assert(held_mode_scan(0, 7, 29) == -1 && low_gate_calls == 1 && hand_reads == 0 && parity_calls == 0);
    /* Gate 4; kind >> 5 not 0: block not filled. */
    low_gate_v = 4;
    assert(held_mode_scan(0, 7, 29) == -1 && hand_reads == 1 && held_calls == 0 && gate_calls == 0);
    /* Kind >> 5 of 0: 2499 is refused, 2500 answers side | slot byte. */
    kinds[10] = 0x1f;
    worth_v = 2499;
    assert(held_mode_scan(0, 7, 29) == -1 && held_calls == 1 && worth_calls == 1);
    worth_v = 2500;
    assert(held_mode_scan(0, 7, 29) == 3 && worth_calls == 1);
    kinds[10] = 1 << 5;

    /* Modes 28 and 31 ask 0x19a5c8; 29 and 30 do not. Even (0) gives 29, odd
     * (positive or negative) gives 30. */
    low_gate_v = 5;
    parity_v = 2;
    assert(held_mode_scan(0, 0, 28) == -1 && parity_calls == 1 && low_gate_calls == 1 && gate_calls == 0);
    assert(held_mode_scan(0, 0, 31) == -1 && parity_calls == 1 && low_gate_calls == 1);
    parity_v = 3;
    assert(held_mode_scan(0, 0, 31) == -1 && parity_calls == 1 && low_gate_calls == 0 && gate_calls == 1);
    parity_v = -3;
    assert(held_mode_scan(0, 0, 28) == -1 && low_gate_calls == 0 && gate_calls == 1);

    /* Mode 30: gate of 5 answers -1 at once. */
    gate_v = 5;
    assert(held_mode_scan(0, 3, 30) == -1 && gate_calls == 1 && hand_reads == 0 && parity_calls == 0);
    gate_v = 4;
    /* Mask 0: neither graveyard is walked. */
    assert(held_mode_scan(0, 0, 30) == -1 && hand_reads == 0);
    /* Entries of 39: both walked, nothing taken. */
    assert(held_mode_scan(0, 3, 30) == -1 && hand_reads == 6 && held_calls == 6);
    /* 40 in own slot 1 and in the other side's slot 1. */
    held_entry[1].deckCost = 40;
    held_entry[9].deckCost = 40;
    assert(held_mode_scan(0, 1, 30) == 1 && hand_reads == 3);
    assert(held_mode_scan(0, 2, 30) == 0x10001 && hand_reads == 3);
    assert(held_mode_scan(0, 3, 30) == 0x10001 && hand_reads == 6);
    assert(held_mode_scan(1, 1, 30) == 0x10001);
    assert(held_mode_scan(1, 2, 30) == 1);
    /* word >> 8 of 20 is not taken; kind >> 5 of 0 is skipped. */
    held_pair[9].word = 20 << 8;
    assert(held_mode_scan(0, 3, 30) == 1);
    held_pair[9].word = 0;
    kinds[9] = 0x1f;
    assert(held_mode_scan(0, 3, 30) == 1 && held_calls == 5);
    held_pair[1].word = 20 << 8;
    kinds[9] = 1 << 5;
    assert(held_mode_scan(0, 1, 30) == -1);
}

int main(void)
{
    check_spots();
    check_hand();
    check_step();
    check_row();
    check_order();
    check_fit();
    check_hand_kinds();
    check_four(TaLogic017_IsSetTrap);
    check_four(TaLogic017_IsExzSafe);
    check_gain();
    check_held();
    check_held_other();
    check_held_modes();
    return 0;
}

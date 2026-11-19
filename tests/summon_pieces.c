/* Synthetic host tests for the summon_pieces group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "dotr/ee/summon_pieces.h"

#define SIDE 1
#define PACKED 0x0102
#define KEY 7

static int spots[8];
static int fields[8];
static int units[8];
static int ids[8];
static int costs[8];
static int lists[8];
static int gate;
static int blocked;
static int here_loc;
static int target_loc;
static int route_n;
static int last;
static struct DotrFreeBlock *expect_block;
static int here_calls, key008_calls, key011_calls, long_calls, short_calls, next_calls, mountain_calls;

static int find(int x, int y)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (spots[i] == ((y << 8) | x)) {
            return i;
        }
    }
    assert(0);
    return 0;
}

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == PACKED >> 8 && b == (PACKED & 0xff));
    here_calls++;
    return here_loc;
}
int Ta_GetSommonPiecePreference(int a, int i) { assert(a == SIDE && i >= 0 && i < 8); return spots[i]; }
int SzDuel_GetField(int x, int y) { return fields[find(x, y)]; }
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y) { return (struct DotrStateItem *)(intptr_t)units[find(x, y)]; }
int SzDuel_GetMapUnitID(int x, int y) { return ids[find(x, y)]; }
int func_0024fe40_free(struct DotrFreeBlock *block, int x, int y)
{
    assert(block == expect_block && fields[find(x, y)] == 9);
    return blocked;
}
int func_00278510_free(struct DotrFreeBlock *block, int packed, int c, int d, int e, int skip)
{
    assert(block == expect_block && packed == PACKED && c == KEY && d == (here_loc & 0xff) && e == here_loc >> 8 && skip == 0);
    return gate;
}
/* TaLogic011's view (free_spots.h). */
int func_0027a130_free(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       int g, int h, unsigned char i, unsigned short key)
{
    assert(block == expect_block && bytes != NULL && x == (here_loc & 0xff) && y == here_loc >> 8);
    assert(g == PACKED && h == 0 && i == route_n && key == KEY);
    long_calls++;
    last = find(column, row);
    return costs[last];
}
/* TaLogic008's view (all ints). */
int func_0027a130_piece(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x,
                        int y, int g, int h, int i, int key)
{
    assert(block == expect_block && bytes != NULL && x == (here_loc & 0xff) && y == here_loc >> 8);
    assert(g == PACKED && h == 0 && i == route_n && key == KEY);
    long_calls++;
    last = find(column, row);
    return costs[last];
}
int func_002792c0_free(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       unsigned short id, int n)
{
    assert(block == expect_block && bytes != NULL && x == (here_loc & 0xff) && y == here_loc >> 8);
    assert(id == PACKED && n == route_n);
    short_calls++;
    last = find(column, row);
    return costs[last];
}
int func_0027afc0_free(struct DotrFreeNibbles *bytes)
{
    int i;

    for (i = 0; i < 12; i++) {
        assert(bytes[i].low == 15 && bytes[i].high == 15);
    }
    return lists[last];
}
int TaLogic_GetWarnHideLvl__00134ae0(int side) { assert(side == SIDE); key008_calls++; return KEY; }
int TaLogic_GetWarnHideLvl__00141b30(int side) { assert(side == SIDE); key011_calls++; return KEY; }
int func_00131f80_piece(struct DotrFreeBlock *block, int column, int row)
{
    assert(block == expect_block);
    (void)find(column, row);
    next_calls++;
    return target_loc;
}
int func_0013eb20_piece(struct DotrFreeBlock *block, int column, int row)
{
    assert(block == expect_block);
    (void)find(column, row);
    mountain_calls++;
    return target_loc;
}

/* Place i sits at column i + 2, row i + 1; all free, cost 100, list non-empty. */
static void reset(struct DotrFreeBlock *block, int mode, int n)
{
    int i;

    memset(block, 0, sizeof(*block));
    block->kind = (mode << 5) | 0x1f;
    expect_block = block;
    for (i = 0; i < 8; i++) {
        spots[i] = ((i + 1) << 8) | (i + 2);
        fields[i] = 0;
        units[i] = 0;
        ids[i] = -1;
        costs[i] = 100;
        lists[i] = 1;
    }
    gate = blocked = 0;
    route_n = n;
    here_loc = 0x0304;
    target_loc = 0x0506; /* column 6, row 5: place 4 at distance 0, i at 2 * |i - 4| */
    here_calls = key008_calls = key011_calls = long_calls = short_calls = next_calls = mountain_calls = 0;
}

/* TaLogic008_GetSummonPiece: one assertion group per branch outcome and boundary. */
static void check_logic008(void)
{
    struct DotrFreeBlock block;
    int i;

    /* null block */
    reset(&block, 0, 4);
    assert(TaLogic008_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && key008_calls == 0 && next_calls == 0);

    /* first stage, short route (n = 4): lowest cost, earliest on a tie */
    reset(&block, 0, 4);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic008_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && short_calls == 8 && long_calls == 0 && key008_calls == 8 && next_calls == 0);
    assert(key011_calls == 0);

    /* first stage, long route when the gate answers above zero; the key is asked twice */
    reset(&block, 0, 4);
    gate = 1;
    costs[7] = 10;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key008_calls == 16);

    /* skipped places: negative, field 8, field 9 when blocked, occupied, empty list */
    reset(&block, 0, 4);
    spots[0] = -1;
    fields[1] = 8;
    fields[2] = 9;
    blocked = 1;
    units[3] = 1;
    lists[4] = 0;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    assert(short_calls == 4);
    blocked = 0;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[2]);

    /* cost bound: 255 never wins and there is no second stage in this branch */
    reset(&block, 0, 4);
    for (i = 0; i < 8; i++) {
        costs[i] = 255;
    }
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(next_calls == 0);
    costs[6] = 254;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);

    /* negative packed value: nearest free place to the next-invade point */
    reset(&block, 0, 4);
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, -1) == spots[4]);
    assert(here_calls == 0 && short_calls == 0 && next_calls == 8);
    ids[4] = 0;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]); /* 3 and 5 tie: earliest */
    fields[3] = 8;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    fields[5] = 9;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    blocked = 1;
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, -1) == spots[2]);

    /* any other mode takes the nearest stage even with a packed value (no row limit) */
    reset(&block, 1, 4);
    target_loc = (8 << 8) | 9; /* place 7 itself, row 8 */
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(here_calls == 0 && next_calls == 8);

    /* distance boundary: 13 is taken, 14 is not */
    reset(&block, 1, 4);
    for (i = 1; i < 8; i++) {
        spots[i] = -1;
    }
    target_loc = (1 << 8) | (2 + 13);
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
    target_loc = (1 << 8) | (2 + 14);
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    target_loc = ((1 + 6) << 8) | (2 + 7);
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
    target_loc = (1 << 8) | 0; /* column below the place: the minimum comes from the target */
    assert(TaLogic008_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
}

/* TaLogic011_GetSummonPiece: one assertion group per branch outcome and boundary. */
static void check_logic011(void)
{
    struct DotrFreeBlock block;
    int i;

    /* null block */
    reset(&block, 0, 2);
    assert(TaLogic011_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && key011_calls == 0);

    /* first stage in modes 0 and 3, short route (n = 2) */
    reset(&block, 0, 2);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic011_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && short_calls == 8 && long_calls == 0 && key011_calls == 8 && key008_calls == 0);
    reset(&block, 3, 2);
    costs[6] = 40;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);

    /* long route when the gate answers above zero */
    reset(&block, 0, 2);
    gate = 1;
    costs[7] = 10;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key011_calls == 16);

    /* skipped places */
    reset(&block, 0, 2);
    spots[0] = -1;
    fields[1] = 8;
    fields[2] = 9;
    blocked = 1;
    units[3] = 1;
    lists[4] = 0;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    assert(short_calls == 4);

    /* cost bound: 255 never wins, then -1 without the nearest stage */
    reset(&block, 0, 2);
    for (i = 0; i < 8; i++) {
        costs[i] = 255;
    }
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(mountain_calls == 0);
    costs[1] = 254;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[1]);

    /* negative packed value: nearest free place to the near-mountain point */
    reset(&block, 3, 2);
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, -1) == spots[4]);
    assert(here_calls == 0 && mountain_calls == 8 && next_calls == 0);
    ids[4] = 0;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    fields[3] = 9;
    blocked = 1;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);

    /* distance boundary: 13 is taken, 14 is not */
    reset(&block, 0, 2);
    for (i = 1; i < 8; i++) {
        spots[i] = -1;
    }
    target_loc = (1 << 8) | (2 + 13);
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    target_loc = (1 << 8) | (2 + 14);
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, -1) == -1);

    /* any other mode (1, 2, 4..7): the last free place in rows 0-7 (row 8 is out, row 7 is in) */
    for (i = 1; i < 8; i++) {
        if (i == 3) {
            continue;
        }
        reset(&block, i, 2);
        assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
        assert(here_calls == 0 && mountain_calls == 0);
    }
    reset(&block, 1, 2);
    fields[6] = 8;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    fields[5] = 9;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    blocked = 1;
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    for (i = 0; i < 8; i++) {
        ids[i] = 2;
    }
    assert(TaLogic011_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
}

/* Smoke test only (docs/DECOMPILATION.md, Tests).
 * TaLogic006_GetSummonPiece: the first stage over the short route (mode 5). */
static int key006_calls;
int TaLogic_GetWarnHideLvl__0012b1b0(int side) { assert(side == SIDE); key006_calls++; return KEY; }
int func_002792c0_piece6(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x,
                         int y, unsigned short id, unsigned char n)
{
    return func_002792c0_free(block, bytes, column, row, x, y, id, n);
}
static void check_logic006(void)
{
    struct DotrFreeBlock block;

    reset(&block, 0, 5);
    costs[3] = 30;
    assert(TaLogic006_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[3]);
    assert(short_calls == 8 && key006_calls == 8);
}

int main(void)
{
    check_logic006();
    check_logic008();
    check_logic011();
    return 0;
}

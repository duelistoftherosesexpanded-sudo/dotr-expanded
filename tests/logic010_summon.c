/* Synthetic host tests for the logic010_summon group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "dotr/ee/logic010_summon.h"

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
static int line_loc;
static int last;
static int here_calls, key_calls, long_calls, short_calls, line_calls;

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
    assert(block != NULL && fields[find(x, y)] == 9);
    return blocked;
}
int func_00278510_free(struct DotrFreeBlock *block, int packed, int c, int d, int e, int skip)
{
    assert(block != NULL && packed == PACKED && c == KEY && d == (here_loc & 0xff) && e == here_loc >> 8 && skip == 0);
    return gate;
}
int func_0027a130_free(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       int g, int h, unsigned char i, unsigned short key)
{
    assert(block != NULL && bytes != NULL && x == (here_loc & 0xff) && y == here_loc >> 8);
    assert(g == PACKED && h == 0 && i == 2 && key == KEY);
    long_calls++;
    last = find(column, row);
    return costs[last];
}
int func_002792c0_free(struct DotrFreeBlock *block, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       unsigned short id, int n)
{
    assert(block != NULL && bytes != NULL && x == (here_loc & 0xff) && y == here_loc >> 8 && id == PACKED && n == 2);
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
int TaLogic_GetWarnHideLvl__0013da70(int side) { assert(side == SIDE); key_calls++; return KEY; }
int TaLogic010_GetEmptyLine(int side) { assert(side == SIDE); line_calls++; return line_loc; }

/* Place i sits at column i + 2, row i + 1; all free, cost 100, list non-empty. */
static void reset(struct DotrFreeBlock *block, int mode)
{
    int i;

    memset(block, 0, sizeof(*block));
    block->kind = (mode << 5) | 0x1f;
    for (i = 0; i < 8; i++) {
        spots[i] = ((i + 1) << 8) | (i + 2);
        fields[i] = 0;
        units[i] = 0;
        ids[i] = -1;
        costs[i] = 100;
        lists[i] = 1;
    }
    gate = blocked = 0;
    here_loc = 0x0304;
    line_loc = 0x0506;
    here_calls = key_calls = long_calls = short_calls = line_calls = 0;
}

/* TaLogic010_GetSummonPiece: one assertion group per branch outcome and boundary. */
static void check_summon_piece(void)
{
    struct DotrFreeBlock block;
    int i;

    /* null block */
    reset(&block, 0);
    assert(TaLogic010_GetSummonPiece(SIDE, 0, NULL, PACKED) == -1);
    assert(here_calls == 0 && key_calls == 0);

    /* first stage, short route: lowest cost, earliest on a tie; the second parameter is not read */
    reset(&block, 0);
    costs[2] = 40;
    costs[5] = 40;
    assert(TaLogic010_GetSummonPiece(SIDE, 9, &block, PACKED) == spots[2]);
    assert(here_calls == 1 && short_calls == 8 && long_calls == 0 && key_calls == 8 && line_calls == 0);

    /* first stage, long route when the gate answers above zero */
    reset(&block, 0);
    gate = 1;
    costs[7] = 10;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[7]);
    assert(long_calls == 8 && short_calls == 0 && key_calls == 16);

    /* skipped places: negative, field 8, occupied, empty list */
    reset(&block, 0);
    spots[0] = -1;
    fields[1] = 8;
    units[2] = 1;
    lists[3] = 0;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(short_calls == 5);

    /* field 9 is skipped only when its callee answers 1 */
    reset(&block, 0);
    for (i = 0; i < 8; i++) {
        fields[i] = 9;
    }
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[0]);
    blocked = 1;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);

    /* a cost of 255 or more never wins: the second stage takes over, nearest to the line place */
    reset(&block, 0);
    for (i = 0; i < 8; i++) {
        costs[i] = 255;
    }
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    assert(line_calls == 8);
    costs[6] = 254;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);

    /* negative packed value: second stage only; occupied places are skipped, ties keep the earliest */
    reset(&block, 0);
    ids[4] = 3;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == spots[3]);
    assert(here_calls == 0 && short_calls == 0 && long_calls == 0 && line_calls == 7);
    fields[3] = 8;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    fields[5] = 9;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == spots[5]);
    blocked = 1;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == spots[2]);

    /* distance boundary: 13 is taken, 14 is not */
    reset(&block, 0);
    for (i = 1; i < 8; i++) {
        spots[i] = -1;
    }
    line_loc = (1 << 8) | (2 + 13);
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);
    line_loc = (1 << 8) | (2 + 14);
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    line_loc = ((1 + 6) << 8) | (2 + 7);
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, -1) == spots[0]);

    /* any other mode: the last free place in rows 0-7 (row 8 is out, row 7 is in) */
    reset(&block, 1);
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[6]);
    assert(here_calls == 0 && line_calls == 0);
    fields[6] = 8;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    fields[5] = 9;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[5]);
    blocked = 1;
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == spots[4]);
    for (i = 0; i < 8; i++) {
        ids[i] = 2;
    }
    assert(TaLogic010_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
}

int main(void)
{
    check_summon_piece();
    return 0;
}

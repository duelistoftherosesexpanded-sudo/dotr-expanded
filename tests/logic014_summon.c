/* Synthetic host tests for the logic014_summon group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/logic014_summon.h"

#define CELL(x, y) ((y) * 256 + (x))
#define SIDE 1
#define OWNER 0
#define PACKED 0x0207
#define HERE CELL(5, 6)
#define WARN 0x12345

static struct DotrFreeBlock block;
static char dummy;

static int pref[2][8];
static int pref_log[64][2], pref_n;
static int field_at[16][16], virus_at[16][16], unit_at[16][16], id_at[16][16];
static int short_cost[16][16], long_cost[16][16], reach_at[16][16];
static int near_cost[16][16];
static int line_cell[3];
static int last_x, last_y;
static int type_answer, type_calls, type_arg;
static int loc_calls, gate_answer, gate_calls, warn_calls;
static int short_calls, long_calls, reach_calls, near_calls, line_calls, owner_calls;
static int near_to[16][16];
static int id_calls;
static struct DotrFreeNibbles *list_seen;

static void check_list(struct DotrFreeNibbles *bytes)
{
    int i;

    if (list_seen == NULL) {
        list_seen = bytes;
    }
    assert(bytes == list_seen);
    for (i = 0; i < 12; i++) {
        assert(bytes[i].low == 0xf && bytes[i].high == 0xf);
    }
}

int SzDuel_Kind2Type(int value)
{
    type_calls++;
    type_arg = value;
    return type_answer;
}
int SzDuel_GetUnitLoc(int a, int b)
{
    assert(a == (PACKED >> 8) && b == (PACKED & 0xff));
    loc_calls++;
    return HERE;
}
int Ta_GetSommonPiecePreference(int a, int i)
{
    assert((a == SIDE || a == OWNER) && i >= 0 && i < 8 && pref_n < 64);
    pref_log[pref_n][0] = a;
    pref_log[pref_n][1] = i;
    pref_n++;
    return pref[a][i];
}
int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 16 && y >= 0 && y < 16);
    return field_at[y][x];
}
int func_0024fe40_free(struct DotrFreeBlock *b, int x, int y)
{
    assert(b == &block && x >= 0 && x < 16 && y >= 0 && y < 16);
    assert(field_at[y][x] == 9); /* asked only for field 9 */
    return virus_at[y][x];
}
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    assert(x >= 0 && x < 16 && y >= 0 && y < 16);
    return unit_at[y][x] ? (struct DotrStateItem *)(void *)&dummy : NULL;
}
int SzDuel_GetMapUnitID(int x, int y)
{
    id_calls++;
    if (x < 0 || x >= 16 || y < 0 || y >= 16) {
        return -1; /* the unused question about the target (0xff, -1 when there is none) */
    }
    return id_at[y][x];
}
int TaLogic_GetWarnHideLvl__0014f9b0(int side)
{
    assert(side == SIDE);
    warn_calls++;
    return WARN;
}
int func_00278510_free(struct DotrFreeBlock *b, int packed, int c, int d, int e, int skip)
{
    assert(b == &block && packed == PACKED && c == WARN && d == (HERE & 0xff) && e == (HERE >> 8) && skip == 0);
    gate_calls++;
    return gate_answer;
}
int func_0027a130_free(struct DotrFreeBlock *b, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       int g, int h, unsigned char i, unsigned short key)
{
    assert(b == &block && x == (HERE & 0xff) && y == (HERE >> 8) && g == PACKED && h == 0 && i == 3);
    assert(key == (WARN & 0xffff));
    check_list(bytes);
    long_calls++;
    last_x = column;
    last_y = row;
    return long_cost[row][column];
}
int func_002792c0_free(struct DotrFreeBlock *b, struct DotrFreeNibbles *bytes, int column, int row, int x, int y,
                       unsigned short id, int n)
{
    assert(b == &block);
    check_list(bytes);
    if (n == 7) {
        /* the fallback: towards the line target, id 0 */
        assert(id == 0);
        near_to[row][column] = CELL(x, y);
        near_calls++;
        return near_cost[row][column];
    }
    assert(n == 3 && x == (HERE & 0xff) && y == (HERE >> 8) && id == PACKED);
    short_calls++;
    last_x = column;
    last_y = row;
    return short_cost[row][column];
}
int func_0027afc0_free(struct DotrFreeNibbles *bytes)
{
    check_list(bytes);
    reach_calls++;
    return reach_at[last_y][last_x];
}
int TaLogic014_GetLine(int index)
{
    assert(index >= 0 && index < 3);
    line_calls++;
    return line_cell[index];
}
int SzDuel_GetTurnOwner(void)
{
    owner_calls++;
    return OWNER;
}

static void reset(void)
{
    int s, i, x, y;

    memset(&block, 0, sizeof block);
    for (s = 0; s < 2; s++)
        for (i = 0; i < 8; i++)
            pref[s][i] = -1;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            field_at[y][x] = 0;
            virus_at[y][x] = 0;
            unit_at[y][x] = 0;
            id_at[y][x] = -1;
            short_cost[y][x] = 50;
            long_cost[y][x] = 60;
            reach_at[y][x] = 1;
            near_cost[y][x] = 20;
            near_to[y][x] = -2;
        }
    for (i = 0; i < 3; i++) {
        line_cell[i] = CELL(10 + i, 12);
        unit_at[12][10 + i] = 1;
    }
    pref_n = 0;
    type_answer = 0;
    type_calls = loc_calls = gate_calls = warn_calls = 0;
    short_calls = long_calls = reach_calls = near_calls = line_calls = owner_calls = id_calls = 0;
    type_arg = -1;
    gate_answer = 0;
    last_x = last_y = 0;
    list_seen = NULL;
}

static void set_level(int level)
{
    block.kind = (unsigned char)((level << 5) | 0x1f); /* the low five bits are ignored */
}

static void check_null_and_levels(void)
{
    reset();
    assert(TaLogic014_GetSummonPiece(SIDE, 99, NULL, PACKED) == -1);
    assert(pref_n == 0 && type_calls == 0);

    /* Level 0: Kind2Type is not asked; nothing free -> -1 (first and fallback stages ran). */
    reset();
    set_level(0);
    assert(TaLogic014_GetSummonPiece(SIDE, 7, &block, PACKED) == -1);
    assert(type_calls == 0 && loc_calls == 1 && pref_n == 16);

    /* Level 3 of type 2: the same stages; Kind2Type gets the level. */
    reset();
    set_level(3);
    type_answer = 2;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(type_calls == 1 && type_arg == 3 && loc_calls == 1 && pref_n == 16);

    /* Level 7 of another type: the last low free cell. */
    reset();
    set_level(7);
    type_answer = 1;
    pref[SIDE][0] = CELL(1, 2);
    pref[SIDE][2] = CELL(3, 7);  /* last free one with a row below 8 */
    pref[SIDE][4] = CELL(4, 8);  /* row 8: not taken */
    pref[SIDE][5] = CELL(5, 1);
    field_at[1][5] = 8;          /* field 8 */
    pref[SIDE][6] = CELL(6, 1);
    field_at[1][6] = 9;          /* field 9 with the virus flag */
    virus_at[1][6] = 1;
    pref[SIDE][7] = CELL(7, 1);
    id_at[1][7] = 4;             /* occupied */
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == CELL(3, 7));
    assert(type_calls == 1 && type_arg == 7 && loc_calls == 0 && pref_n == 8 && short_calls == 0 && near_calls == 0);

    /* Field 9 without the flag is free; nothing below row 8 -> -1. */
    reset();
    set_level(1);
    type_answer = 0;
    pref[SIDE][3] = CELL(2, 3);
    field_at[3][2] = 9;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == CELL(2, 3));
    reset();
    set_level(1);
    pref[SIDE][3] = CELL(2, 9);
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
}

static void check_first_stage(void)
{
    int i;

    /* Short costs (gate <= 0): the cheapest reachable qualifying cell, first wins a tie. */
    reset();
    set_level(0);
    for (i = 0; i < 8; i++)
        pref[SIDE][i] = CELL(i, 1);
    short_cost[1][0] = 30;
    short_cost[1][1] = 20;
    short_cost[1][2] = 20;          /* tie: not taken */
    short_cost[1][3] = 5;
    reach_at[1][3] = 0;             /* not reachable */
    short_cost[1][4] = 4;
    field_at[1][4] = 8;             /* field 8 */
    short_cost[1][5] = 3;
    field_at[1][5] = 9;             /* virus */
    virus_at[1][5] = 1;
    short_cost[1][6] = 2;
    unit_at[1][6] = 1;              /* occupied */
    short_cost[1][7] = 19;
    field_at[1][7] = 9;             /* field 9 without the flag: qualifies */
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == CELL(7, 1));
    assert(short_calls == 5 && long_calls == 0 && gate_calls == 5 && warn_calls == 5 && reach_calls == 5);
    assert(near_calls == 0 && pref_n == 8);

    /* Long costs when the gate is positive; a cost of 255 or more is never taken. */
    reset();
    set_level(0);
    gate_answer = 1;
    pref[SIDE][0] = CELL(2, 2);
    pref[SIDE][1] = CELL(3, 2);
    long_cost[2][2] = 255;
    long_cost[2][3] = 254;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == CELL(3, 2));
    assert(long_calls == 2 && short_calls == 0 && warn_calls == 4);

    reset();
    set_level(0);
    gate_answer = 1;
    pref[SIDE][0] = CELL(2, 2);
    long_cost[2][2] = 255;
    near_cost[2][2] = 14;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == -1);
    assert(long_calls == 1 && near_calls == 1);

    /* Negative packed: no first stage, no location. */
    reset();
    set_level(0);
    pref[SIDE][0] = CELL(2, 2);
    near_cost[2][2] = 3;
    line_cell[0] = CELL(9, 9);
    unit_at[9][9] = 0;
    pref[OWNER][0] = CELL(4, 13);
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, -1) == CELL(2, 2));
    assert(loc_calls == 0 && short_calls == 0 && long_calls == 0 && near_calls == 1);
    assert(near_to[2][2] == CELL(4, 13));
}

static void check_fallback(void)
{
    int i;

    /* The target comes from the first line cell with no unit: line 1 -> owner preference 3. */
    reset();
    set_level(0);
    unit_at[12][11] = 0;
    pref[OWNER][3] = CELL(8, 14);
    pref[SIDE][0] = CELL(1, 1);
    near_cost[1][1] = 13;
    pref[SIDE][1] = CELL(2, 1);
    near_cost[1][1 + 1] = 13;      /* tie: the first stays */
    pref[SIDE][2] = CELL(3, 1);
    near_cost[1][3] = -1;          /* negative: never taken */
    pref[SIDE][3] = CELL(4, 1);
    id_at[1][4] = 0;               /* occupied: not costed */
    pref[SIDE][4] = CELL(5, 1);
    field_at[1][5] = 8;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, -5) == CELL(1, 1));
    assert(near_calls == 3 && near_to[1][1] == CELL(8, 14) && owner_calls == 3 && line_calls == 6);
    for (i = 0; i < pref_n; i++) {
        if (pref_log[i][0] == OWNER) {
            assert(pref_log[i][1] == 3);
        }
    }

    /* Line 2 -> preference 4; line 0 -> preference 0; none -> no owner, target (0xff, -1). */
    reset();
    set_level(0);
    unit_at[12][12] = 0;
    pref[OWNER][4] = CELL(6, 6);
    pref[SIDE][7] = CELL(1, 1);
    near_cost[1][1] = 0;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, -1) == CELL(1, 1));
    assert(near_to[1][1] == CELL(6, 6) && pref_n == 9 && line_calls == 3);
    assert(pref_log[7][0] == SIDE && pref_log[7][1] == 7 && pref_log[8][0] == OWNER && pref_log[8][1] == 4);

    reset();
    set_level(0);
    unit_at[12][10] = 0;
    pref[OWNER][0] = CELL(7, 7);
    pref[SIDE][7] = CELL(1, 1);
    near_cost[1][1] = 0;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, -1) == CELL(1, 1));
    assert(near_to[1][1] == CELL(7, 7) && pref_log[8][0] == OWNER && pref_log[8][1] == 0 && line_calls == 1);

    reset();
    set_level(0);
    pref[SIDE][7] = CELL(1, 1);
    near_cost[1][1] = 5;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, -1) == CELL(1, 1));
    assert(near_to[1][1] == -1 && owner_calls == 0 && line_calls == 3); /* (0xff, -1) */
    /* the unused id question about the target is asked once per costed cell */
    assert(id_calls == 2);

    /* Cost 14 is not near enough. */
    reset();
    set_level(0);
    pref[SIDE][0] = CELL(1, 1);
    near_cost[1][1] = 14;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, -1) == -1);
    assert(near_calls == 1);

    /* A first stage that finds nothing reachable falls through to the fallback. */
    reset();
    set_level(0);
    pref[SIDE][0] = CELL(1, 1);
    reach_at[1][1] = 0;
    near_cost[1][1] = 9;
    assert(TaLogic014_GetSummonPiece(SIDE, 0, &block, PACKED) == CELL(1, 1));
    assert(short_calls == 1 && near_calls == 1 && loc_calls == 1);
}

int main(void)
{
    check_null_and_levels();
    check_first_stage();
    check_fallback();
    puts("logic014_summon: ok");
    return 0;
}

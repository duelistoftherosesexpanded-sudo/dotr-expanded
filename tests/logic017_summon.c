/* Synthetic host tests for the logic017_summon group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/logic017_summon.h"

#define CELL(x, y) (((y) << 8) | (x))

static int loc_word;
static int loc_calls, loc_side;
static int exz_id, exz_calls, exz_last;
static int empty_calls, trap_calls, piece_side;
static int lines[8][7];
static int lines_full;
static int line_log[32][2], line_n;
static int pref[8];
static int pref_calls;
static int field_at[16][16];
static int id_at[16][16];
static char dummy;

int SzDuel_GetUnitLoc(int a, int b)
{
    assert(b == 0);
    loc_calls++;
    loc_side = a;
    return loc_word;
}
int TaLogic017_IsExzParts(int id)
{
    exz_calls++;
    exz_last = id;
    return id == exz_id ? 1 : 0;
}
int func_00156ea0_summon(int side)
{
    empty_calls++;
    piece_side = side;
    return 0x111;
}
int func_00156f70_summon(int side)
{
    trap_calls++;
    piece_side = side;
    return 0x222;
}
int TaLogic017_GetLine(int index, int far)
{
    assert(line_n < 32);
    assert(far >= 0 && far < 8 && index >= 0 && index < 7);
    line_log[line_n][0] = index;
    line_log[line_n][1] = far;
    line_n++;
    return lines[far][index];
}
struct DotrStateItem *SzDuel_GetMapUnit(int x, int y)
{
    (void)x;
    (void)y;
    return lines_full ? (struct DotrStateItem *)(void *)&dummy : NULL;
}
int Ta_GetSommonPiecePreference(int side, int i)
{
    assert(side == loc_side && i == pref_calls);
    pref_calls++;
    return pref[i];
}
int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 16 && y >= 0 && y < 16);
    return field_at[y][x];
}
int SzDuel_GetMapUnitID(int x, int y)
{
    assert(x >= 0 && x < 16 && y >= 0 && y < 16);
    return id_at[y][x];
}

static struct DotrLogic017SummonBlock block;

static void reset(void)
{
    int c, i, x, y;
    loc_word = CELL(3, 3);
    loc_calls = exz_calls = empty_calls = trap_calls = pref_calls = line_n = 0;
    loc_side = piece_side = exz_last = -99;
    exz_id = 700;
    lines_full = 1;
    for (c = 0; c < 8; c++)
        for (i = 0; i < 7; i++)
            lines[c][i] = CELL(i, c);
    for (i = 0; i < 8; i++)
        pref[i] = -1;
    for (y = 0; y < 16; y++)
        for (x = 0; x < 16; x++) {
            field_at[y][x] = 0;
            id_at[y][x] = -1;
        }
    memset(&block, 0, sizeof block);
    block.unknown_3c = 100;
}

static void check_special_cards(void)
{
    /* Null block: -1 after the location read. */
    reset();
    assert(TaLogic017_GetSummonPiece(1, 0, NULL) == -1);
    assert(loc_calls == 1 && exz_calls == 0);

    /* Parts card: the empty Exz piece of the side only from location 0x403. */
    reset();
    block.unknown_3c = 700;
    loc_word = 0x403;
    assert(TaLogic017_GetSummonPiece(1, 55, &block) == 0x111);
    assert(exz_last == 700 && empty_calls == 1 && piece_side == 1 && trap_calls == 0 && pref_calls == 0);
    reset();
    block.unknown_3c = 700;
    loc_word = 0x404;
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == -1);
    assert(empty_calls == 0 && trap_calls == 0 && pref_calls == 0 && line_n == 0);

    /* Ids 821 and 824: the empty trap piece only from 0x403; 822 is an ordinary card. */
    reset();
    block.unknown_3c = 821;
    loc_word = 0x403;
    assert(TaLogic017_GetSummonPiece(0, 0, &block) == 0x222);
    assert(trap_calls == 1 && piece_side == 0 && empty_calls == 0);
    reset();
    block.unknown_3c = 824;
    loc_word = 0x403;
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == 0x222);
    assert(trap_calls == 1 && piece_side == 1);
    reset();
    block.unknown_3c = 824;
    loc_word = 0x303;
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == -1);
    assert(trap_calls == 0 && pref_calls == 0);
    reset();
    block.unknown_3c = 822;
    loc_word = 0x403;
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == -1);
    assert(trap_calls == 0 && empty_calls == 0 && pref_calls == 8 && loc_calls == 2);
}

/* Candidates (2, 4) then (4, 4): the first is nearer to a target with x <= 2, the second to
 * one with x >= 4 (and wins a tie). */
static int pick_between(void)
{
    pref[0] = CELL(2, 4);
    pref[1] = CELL(4, 4);
    return TaLogic017_GetSummonPiece(0, 0, &block);
}

static void check_targets(void)
{
    int i;

    /* far = 2 (top three bits of +0x3e); its line's first empty cell (index 0) is the target
     * (0, 2): the left candidate. GetLine is asked again for the returned cell. */
    reset();
    block.unknown_3e = 0x5f;
    lines_full = 0;
    assert(pick_between() == CELL(2, 4));
    assert(line_n == 2 && line_log[0][0] == 0 && line_log[0][1] == 2 && line_log[1][0] == 0 && line_log[1][1] == 2);

    /* far = 2, line 2 full: line 0 is tried next (all full) and 0x503 is the target. */
    reset();
    block.unknown_3e = 0x40;
    assert(pick_between() == CELL(4, 4));
    assert(line_n == 14);
    for (i = 0; i < 7; i++) {
        assert(line_log[i][0] == i && line_log[i][1] == 2);
        assert(line_log[7 + i][0] == i && line_log[7 + i][1] == 0);
    }

    /* far = 0, line 0 full: line 1 is tried next, then 0x503. */
    reset();
    assert(pick_between() == CELL(4, 4));
    assert(line_n == 14 && line_log[0][1] == 0 && line_log[7][1] == 1 && line_log[13][1] == 1);

    /* far = 7: line 7 then line 0. */
    reset();
    block.unknown_3e = 0xe0;
    assert(pick_between() == CELL(4, 4));
    assert(line_n == 14 && line_log[0][1] == 7 && line_log[7][1] == 0);
}

/* With every line full the target is 0x503 = (3, 5); the leader stands at (3, 3). */
static int one(int cell)
{
    pref[5] = cell;
    return TaLogic017_GetSummonPiece(1, 0, &block);
}

static void check_rejections(void)
{
    reset();
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == -1);
    assert(pref_calls == 8);

    reset();
    assert(one(CELL(3, 4)) == CELL(3, 4));
    reset();
    field_at[4][3] = 8;
    assert(one(CELL(3, 4)) == -1);
    reset();
    field_at[4][3] = 9;
    assert(one(CELL(3, 4)) == -1);
    reset();
    field_at[4][3] = 7;
    assert(one(CELL(3, 4)) == CELL(3, 4));
    reset();
    id_at[4][3] = 0;
    assert(one(CELL(3, 4)) == -1);

    /* Rows below the leader's are refused; its own row is allowed. */
    reset();
    assert(one(CELL(3, 2)) == -1);
    reset();
    assert(one(CELL(3, 3)) == CELL(3, 3));

    /* Distance limit 7 from the target (3, 5). */
    reset();
    assert(one(CELL(0, 9)) == CELL(0, 9));
    reset();
    assert(one(CELL(0, 10)) == -1);
    reset();
    assert(one(CELL(7, 9)) == -1);
    reset();
    assert(one(CELL(7, 8)) == CELL(7, 8));
}

static void check_choice(void)
{
    /* A later candidate replaces an earlier one at equal distance. */
    reset();
    pref[1] = CELL(2, 5);
    pref[6] = CELL(4, 5);
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == CELL(4, 5));

    /* A pick raises the row: (3, 4) at the same distance is then refused. */
    reset();
    pref[0] = CELL(3, 6);
    pref[1] = CELL(3, 4);
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == CELL(3, 6));

    /* A farther later candidate does not replace a nearer one. */
    reset();
    pref[2] = CELL(3, 5);
    pref[3] = CELL(3, 6);
    assert(TaLogic017_GetSummonPiece(1, 0, &block) == CELL(3, 5));
}

int main(void)
{
    check_special_cards();
    check_targets();
    check_rejections();
    check_choice();
    puts("logic017_summon: ok");
    return 0;
}

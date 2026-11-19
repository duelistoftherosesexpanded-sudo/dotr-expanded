/* Synthetic host tests for the cell_picks group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/cell_picks.h"

#define WORD 0x4321
#define SIDE 1

static int kinds[7][7];
static int cells[7][7];
static int owner_answer;
static int score_answer;
static int score_calls;
static int score_args[6];
static int side_calls;

int SzDuel_GetField(int x, int y)
{
    assert(x >= 0 && x < 7 && y >= 0 && y < 7);
    return kinds[x][y];
}

int SzDuel_GetMapUnitID(int x, int y)
{
    assert(kinds[x][y] == 2);
    return cells[x][y];
}

int func_00250360_pair(int a, int b)
{
    assert(a == 0x12 && b == 0x34);
    return owner_answer;
}

int func_00250370_word(int word)
{
    assert(word == WORD);
    return SIDE;
}

int TaLogic_GetWarnHideLvl__00141b30(int side)
{
    assert(side == SIDE);
    side_calls++;
    return 77;
}

int func_00278510_word(int item, int packed, int c, int d, int e, int skip)
{
    score_args[0] = item, score_args[1] = packed, score_args[2] = c;
    score_args[3] = d, score_args[4] = e, score_args[5] = skip;
    score_calls++;
    return score_answer;
}

static void reset(void)
{
    int x;
    int y;

    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            kinds[x][y] = 0;
            cells[x][y] = -1;
        }
    }
    owner_answer = SIDE + 1;
    score_answer = 1;
    score_calls = side_calls = 0;
}

/* 0x13eb20: branch coverage. Every case leaves at least one qualifying cell,
 * since the answer has no initial value otherwise. */
static void check_nearest(void)
{
    /* One free cell of kind 2; cells of other kinds are not looked at. */
    reset();
    kinds[3][5] = 2;
    kinds[2][2] = 1;
    kinds[2][3] = 3;
    assert(TaLogic011_GetNearMountain(WORD, 2, 2) == 0x0503);
    assert(score_calls == 0);

    /* The nearer of two wins in either scan order. */
    reset();
    kinds[1][1] = kinds[6][6] = 2;
    assert(TaLogic011_GetNearMountain(WORD, 5, 5) == 0x0606);
    assert(TaLogic011_GetNearMountain(WORD, 0, 2) == 0x0101);
    /* Equal distances: the first in scan order (x, then y) stays. */
    reset();
    kinds[2][4] = kinds[4][2] = 2;
    assert(TaLogic011_GetNearMountain(WORD, 3, 3) == 0x0402);
    /* Column and row on either side of the cell give the same distance. */
    reset();
    kinds[3][3] = kinds[0][0] = 2;
    assert(TaLogic011_GetNearMountain(WORD, 6, 6) == 0x0303);
    assert(TaLogic011_GetNearMountain(WORD, 1, 1) == 0x0000);

    /* The bound: distance 13 is taken, 14 is not. */
    reset();
    kinds[0][0] = 2;
    assert(TaLogic011_GetNearMountain(WORD, 13, 0) == 0x0000);
    kinds[1][0] = 2;
    assert(TaLogic011_GetNearMountain(WORD, 14, 0) == 0x0001);

    /* An occupied cell of another owner with a positive 0x278510 answer. */
    reset();
    kinds[6][0] = kinds[2][1] = 2;
    cells[2][1] = 0x1234;
    assert(TaLogic011_GetNearMountain(WORD, 2, 1) == 0x0102);
    assert(score_calls == 1 && side_calls == 1);
    assert(score_args[0] == WORD && score_args[1] == 0x1234 && score_args[2] == 77);
    assert(score_args[3] == 2 && score_args[4] == 1 && score_args[5] == 0);
    /* Answer 0 rejects it. */
    score_answer = 0;
    score_calls = 0;
    assert(TaLogic011_GetNearMountain(WORD, 2, 1) == 0x0006);
    assert(score_calls == 1);
    /* The same owner: 0x278510 is not asked. */
    score_answer = 1;
    score_calls = side_calls = 0;
    owner_answer = SIDE;
    assert(TaLogic011_GetNearMountain(WORD, 2, 1) == 0x0006);
    assert(score_calls == 0 && side_calls == 0);
    /* An accepted occupied cell farther than the best so far does not replace it. */
    owner_answer = SIDE + 1;
    assert(TaLogic011_GetNearMountain(WORD, 6, 0) == 0x0006);
    assert(score_calls == 1);
}

int main(void)
{
    check_nearest();
    return 0;
}

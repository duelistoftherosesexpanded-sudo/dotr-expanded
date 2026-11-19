/* Synthetic host tests for the summon_cells group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "dotr/ee/summon_cells.h"

static int loc_word;
static int loc_calls, loc_a, loc_b;

int SzDuel_GetUnitLoc(int a, int b)
{
    loc_calls++;
    loc_a = a;
    loc_b = b;
    return loc_word;
}

static int call(int side, int i, int loc)
{
    int r;
    loc_word = loc;
    loc_calls = 0;
    loc_a = loc_b = -99;
    r = Ta_GetSommonPiecePreference(side, i);
    assert(loc_calls == 1 && loc_a == side && loc_b == 0);
    return r;
}

/* Independent statement of the order: steps (dx, dy) per side; only +1 steps are checked
 * against 7, -1 steps wrap as unsigned. */
static const int dx[2][8] = {{0, 0, 1, -1, 1, -1, 1, -1}, {0, 0, -1, 1, -1, 1, -1, 1}};
static const int dy[2][8] = {{-1, 1, 0, 0, -1, -1, 1, 1}, {1, -1, 0, 0, 1, 1, -1, -1}};

static int expected(int side, int i, unsigned int loc)
{
    unsigned int x = (loc & 0xff) + (unsigned int)dx[side][i];
    unsigned int y = (loc >> 8) + (unsigned int)dy[side][i];
    if ((dx[side][i] > 0 && x >= 7) || (dy[side][i] > 0 && y >= 7))
        return -1;
    return (int)((y << 8) | x);
}

static void check_literals(void)
{
    /* Centre (3, 3) = 0x303. */
    assert(call(0, 0, 0x303) == 0x203);
    assert(call(0, 1, 0x303) == 0x403);
    assert(call(0, 2, 0x303) == 0x304);
    assert(call(0, 3, 0x303) == 0x302);
    assert(call(0, 4, 0x303) == 0x204);
    assert(call(0, 5, 0x303) == 0x202);
    assert(call(0, 6, 0x303) == 0x404);
    assert(call(0, 7, 0x303) == 0x402);
    assert(call(1, 0, 0x303) == 0x403);
    assert(call(1, 1, 0x303) == 0x203);
    assert(call(1, 2, 0x303) == 0x302);
    assert(call(1, 3, 0x303) == 0x304);
    assert(call(1, 4, 0x303) == 0x402);
    assert(call(1, 5, 0x303) == 0x404);
    assert(call(1, 6, 0x303) == 0x202);
    assert(call(1, 7, 0x303) == 0x204);

    /* Edges: x = 6 or y = 6 reject the +1 steps, 5 still allowed. */
    assert(call(0, 2, 0x306) == -1);
    assert(call(0, 2, 0x305) == 0x306);
    assert(call(0, 1, 0x603) == -1);
    assert(call(0, 6, 0x605) == -1);
    assert(call(0, 6, 0x506) == -1);
    assert(call(0, 6, 0x505) == 0x606);
    assert(call(1, 5, 0x605) == -1);
    assert(call(1, 5, 0x506) == -1);
    assert(call(1, 0, 0x503) == 0x603);

    /* Subtracting cases are unchecked: x = 0 gives -1 through the wrapped low bits,
     * y = 0 gives 0xffffff00 | x. */
    assert(call(0, 3, 0x300) == -1);
    assert(call(1, 2, 0x300) == -1);
    assert(call(0, 0, 0x003) == (int)0xffffff03u);
    assert(call(1, 1, 0x004) == (int)0xffffff04u);

    /* Other sides and indices. */
    assert(call(2, 0, 0x303) == -1);
    assert(call(-1, 0, 0x303) == -1);
    assert(call(0, 8, 0x303) == -1);
    assert(call(1, 8, 0x303) == -1);
    assert(call(0, -1, 0x303) == -1);
}

static void check_all_cells(void)
{
    int side, i, x, y;
    for (side = 0; side < 2; side++)
        for (i = 0; i < 8; i++)
            for (y = 0; y < 7; y++)
                for (x = 0; x < 7; x++)
                    assert(call(side, i, (y << 8) | x) == expected(side, i, (unsigned int)((y << 8) | x)));
}

int main(void)
{
    check_literals();
    check_all_cells();
    puts("summon_cells: ok");
    return 0;
}

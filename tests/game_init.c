/* Synthetic host tests for the game_init group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/game_init.h"

struct DotrGameBlock gSzGame_gl;
static int fill_calls, option_calls, option_args[3][3];

void *RhMem_FillMemory(void *destination, int value, int byte_count)
{
    assert(destination == &gSzGame_gl && value == 0 && byte_count == 560);
    fill_calls++;
    return memset(destination, value, (size_t)byte_count);
}

int SzData_SetOption(int side, int which, int flag)
{
    option_args[option_calls][0] = side;
    option_args[option_calls][1] = which;
    option_args[option_calls][2] = flag;
    option_calls++;
    return 0;
}

/* Loop coverage for 0x1a6c80: the first and last entry of the 37-entry loop, every entry
 * the straight-line part sets (kind, limit, value, step), every other stored byte and
 * bit-field, and the three reported values in order. The block starts as all ones. */
static void check_init(void)
{
    static const int set[][5] = { /* entry, kind, limit, value, step */
        { 0, 0, 0, 0, 10 }, { 1, 2, 56, 0, 10 }, { 2, 1, 2, 0, 10 }, { 3, 4, 102, 100, 10 }, { 4, 4, 102, 100, 10 },
        { 5, 2, 10, 0, 10 }, { 6, 1, 2, 0, 10 }, { 7, 3, 683, 0, 10 }, { 8, 0, 0, 0, 10 }, { 9, 1, 5, 0, 10 },
        { 10, 1, 3, 0, 10 }, { 11, 2, 100, 0, 10 }, { 12, 1, 5, 0, 10 }, { 13, 0, 0, 0, 10 }, { 16, 1, 4, 0, 10 },
        { 17, 4, 10000, 0, 10 }, { 18, 0, 0, 0, 10 }, { 19, 4, 10000, 0, 10 }, { 20, 1, 7, 0, 10 }, { 21, 1, 2, 1, 10 },
        { 22, 4, 10000, 99, 10 }, { 23, 5, 100000, 0, 10 }, { 24, 4, 0x10000, 0, 16 }, { 25, 1, 2, 1, 10 },
        { 26, 2, 129, 128, 16 }, { 27, 2, 13, 1, 10 }, { 28, 1, 4, 3, 10 }, { 29, 1, 4, 3, 10 }, { 30, 1, 2, 1, 10 },
        { 31, 1, 2, 0, 10 }, { 32, 1, 2, 0, 10 }, { 33, 2, 18, 0, 10 }, { 34, 0, 0, 0, 10 }, { 36, 0, 0, 0, 10 },
    };
    struct DotrGameBlock *g = &gSzGame_gl;
    size_t i;

    memset(g, 0xff, sizeof(*g));
    SzGame_Init();
    assert(fill_calls == 1 && g->w000 == 0 && g->w004 == 0 && g->w008 == 0 && g->w00c == 0);
    for (i = 0; i < sizeof(set) / sizeof(set[0]); i++) {
        int n = set[i][0];

        assert(g->kind[n] == set[i][1] && g->limit[n] == set[i][2] && g->value[n] == set[i][3] && g->step[n] == set[i][4]);
    }
    assert(g->b497 == -3 && g->b505 == 0 && g->w516 == 0 && g->w520 == 0);
    assert(g->b524 == 0 && g->b525 == 0 && g->b526 == 0 && g->b527 == 128 && g->w532 == 0 && g->w536 == 0);
    assert(g->h540 == 0 && g->b542 == -3 && g->b543 == -3 && g->b544 == -3);
    assert(g->b545 == 0 && g->b546 == 0 && g->b547 == 0 && g->b552 == 0 && g->b553 == 0);
    /* +554 is set to 1; +555 copies the low byte of value 20 (0 here) */
    assert(g->b554 == 1 && g->b555 == 0);
    assert(g->f556.b01 == 0 && g->f556.b23 == 1 && g->f556.b45 == 0 && g->f556.b67 == 0);
    assert(g->f557.b02 == 0 && g->f557.b35 == 0 && g->f557.b67 == 0);
    /* bit 5 of +558 is the low bit of value 21 (1), bit 6 is set */
    assert(g->f558.b02 == 0 && g->f558.b3 == 0 && g->f558.b4 == 0 && g->f558.b5 != 0 && g->f558.b6 != 0 && g->f558.b7 == 0);
    /* values 32, 31 and 25 go to options 0, 1 and 2 of side 0 */
    assert(option_calls == 3 && option_args[0][0] == 0 && option_args[0][1] == 0 && option_args[0][2] == 0);
    assert(option_args[1][0] == 0 && option_args[1][1] == 1 && option_args[1][2] == 0);
    assert(option_args[2][0] == 0 && option_args[2][1] == 2 && option_args[2][2] == 1);
}

int main(void)
{
    check_init();
    return 0;
}

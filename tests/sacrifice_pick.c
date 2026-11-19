/* Synthetic host tests for the sacrifice_pick group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/sacrifice_pick.h"

#define ARG 0x77

static const int perms[6][3] = {
    {10, 20, 30}, {10, 30, 20}, {20, 10, 30}, {20, 30, 10}, {30, 10, 20}, {30, 20, 10},
};
static int answers[6];
static int nums[6];
static int points[6][3];
static int order_calls;
static int num_calls;
static int owner_calls;
static int point_calls;

int Ta_GetScrificeOrder(int *words, int arg)
{
    /* the six orders arrive in turn, in a block of the caller's own */
    assert(arg == ARG && order_calls < 6);
    assert(words[0] == perms[order_calls][0] && words[1] == perms[order_calls][1] && words[2] == perms[order_calls][2]);
    return answers[order_calls++];
}

int Ta_GetUnitNumFrom22bitFlagList(unsigned value)
{
    assert(value >= 100 && value < 106);
    num_calls++;
    return nums[value - 100];
}

int Ta_GetUnitOwnerIDFromLstCount(int word, int n)
{
    assert(word >= 100 && word < 106 && n >= 0 && n < 3);
    owner_calls++;
    return ((word - 99) << 8) | (n + 0x40);
}

int Ta_GetAPPlusDP(int a, int b)
{
    assert(a >= 1 && a <= 6 && b >= 0x40 && b < 0x43);
    point_calls++;
    return points[a - 1][b - 0x40];
}

static void reset(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        answers[i] = 100 + i;
        nums[i] = 3;
        points[i][0] = points[i][1] = points[i][2] = 100;
    }
    order_calls = num_calls = owner_calls = point_calls = 0;
}

static void check_sacrifice(void)
{
    int in[3] = {10, 20, 30};
    int i;

    /* no order answers: -1, nothing counted */
    reset();
    for (i = 0; i < 6; i++) {
        answers[i] = -1;
    }
    assert(Ta_GetSacrificeUnit(in, ARG) == -1);
    assert(order_calls == 6 && num_calls == 0 && owner_calls == 0 && point_calls == 0);
    assert(in[0] == 10 && in[1] == 20 && in[2] == 30);

    /* lowest sum of the three points wins */
    reset();
    points[4][1] = 1;
    assert(Ta_GetSacrificeUnit(in, ARG) == 104);
    assert(order_calls == 6 && num_calls == 6 && owner_calls == 18 && point_calls == 18);

    /* a tie goes to the earlier order */
    reset();
    points[2][0] = 5;
    points[5][2] = 5;
    assert(Ta_GetSacrificeUnit(in, ARG) == 102);
    reset();
    assert(Ta_GetSacrificeUnit(in, ARG) == 100);

    /* a count other than three, or a negative answer, takes the order out */
    reset();
    points[1][0] = 0;
    nums[1] = 2;
    points[3][0] = 1;
    nums[3] = 4;
    points[2][0] = 2;
    answers[2] = -5;
    points[5][0] = 50;
    assert(Ta_GetSacrificeUnit(in, ARG) == 105);
    assert(num_calls == 5 && owner_calls == 9 && point_calls == 9);

    /* a negative sum is skipped; a sum of zero is taken */
    reset();
    points[0][0] = -301;
    points[3][0] = -200;
    assert(Ta_GetSacrificeUnit(in, ARG) == 103);

    /* bound: a sum of 0xffff is taken, 0x10000 is not */
    reset();
    for (i = 0; i < 6; i++) {
        points[i][0] = 0x10000 - 200;
    }
    assert(Ta_GetSacrificeUnit(in, ARG) == -1);
    points[4][0] = 0xffff - 200;
    order_calls = 0;
    assert(Ta_GetSacrificeUnit(in, ARG) == 104);
}

int main(void)
{
    check_sacrifice();
    return 0;
}

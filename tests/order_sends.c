/* Synthetic host tests for the order_sends group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/order_sends.h"

static int gate_result, path_result, live_result;
static int gate_calls, path_calls, live_calls;
static int gate_args[3], path_args[5], live_arg;

int TaLogic_IsAttackedNow(int row, int column, int c)
{
    gate_calls++;
    gate_args[0] = row;
    gate_args[1] = column;
    gate_args[2] = c;
    return gate_result;
}

int TaLogic_IsAttackedNow2(int row, int column, int a, int b, int c)
{
    path_calls++;
    path_args[0] = row;
    path_args[1] = column;
    path_args[2] = a;
    path_args[3] = b;
    path_args[4] = c;
    return path_result;
}

int SzDuel_InqHandPhase(int index)
{
    live_calls++;
    live_arg = index;
    return live_result;
}

static void run(struct DotrSendOrder *order, int gate, int path, int live)
{
    memset(order, 0, sizeof(*order));
    order->position = 0x0207;
    order->summonOrder = 0x1111;
    order->unknown_06 = 0x2222;
    order->flags.word = 0x7fff1234;
    gate_result = gate;
    path_result = path;
    live_result = live;
    gate_calls = path_calls = live_calls = 0;
    TaLogic_CommandStdLDUnit(order, 3, 4, 5);
    assert(gate_calls == 1 && gate_args[0] == 2 && gate_args[1] == 7 && gate_args[2] == 5);
    assert(live_calls == 1 && live_arg == 2);
    assert(order->position == 0x0207 && order->summonOrder == 0x1111 && order->unknown_06 == 0x2222);
}

static void check_send(void)
{
    struct DotrSendOrder order;

    /* Gate 1: 0x05xx, the second check is not made, the word at +8 is kept. */
    run(&order, 1, 0, 1);
    assert(path_calls == 0 && order.code == 0x50a && order.flags.word == 0x7fff1234);
    run(&order, 1, 0, 0);
    assert(path_calls == 0 && order.code == 0x5ff && order.flags.word == 0x7fff1234);

    /* Gate not 1, second check 0: 0x06xx and (b << 8 | a) over the kept low halfword. */
    run(&order, 0, 0, 1);
    assert(path_calls == 1 && path_args[0] == 2 && path_args[1] == 7 && path_args[2] == 3);
    assert(path_args[3] == 4 && path_args[4] == 5);
    assert(order.code == 0x60a && order.flags.word == 0x04031234);
    run(&order, 2, 0, 2);
    assert(path_calls == 1 && order.code == 0x6ff && order.flags.word == 0x04031234);

    /* Second check non-zero: 0x05xx again, word kept. */
    run(&order, 0, 1, 1);
    assert(path_calls == 1 && order.code == 0x50a && order.flags.word == 0x7fff1234);
    run(&order, 0, -1, 0);
    assert(path_calls == 1 && order.code == 0x5ff && order.flags.word == 0x7fff1234);
}

int main(void)
{
    check_send();
    return 0;
}

#include "dotr/ee/order_tries.h"

void TaLogic003_CommandAtkLineUnit(struct DotrTryOrder *order)
{
    int side;
    int here;
    int first;
    int second;
    int value;
    int kind;
    int row;

    side = SzDuel_GetTurnOwner();
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    first = TaLogic003_GetEmptyAtkLineStart(side);
    second = TaLogic003_GetEmptyAtkLineEnd(side);
    row = here >> 8;
    switch (row) {
    case 0:
        if (here == first) {
            order->unknown_04 = 0xfff;
        } else {
            order->unknown_04 = 0x6ff;
            order->flags.word = (first << 16) | order->flags.half;
        }
        break;
    case 6:
        if (here == second) {
            order->unknown_04 = 0x10ff;
        } else {
            order->unknown_04 = 0x6ff;
            order->flags.word = (second << 16) | order->flags.half;
        }
        break;
    default:
        kind = (order->unknown_04 >> 8) & 0xff;
        if (kind != 15 && kind != 16) {
            value = row < 4 ? 15 : 16;
            order->unknown_04 = (value << 8) | 0xff;
            order->flags.word = order->flags.half | 0xffff0000;
        }
        break;
    }
}

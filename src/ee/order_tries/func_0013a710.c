#include "dotr/ee/order_tries.h"

void TaLogic010_CommandInvadeUnit(struct DotrTryOrder *order, int deep)
{
    int side;
    int here;
    int column;
    int row;
    int r;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if ((deep & 0xff) == 1) {
        column = here & 0xff;
        row = (here >> 8) + 1;
        r = SzDuel_GetMapUnitID(column, row);
        if (r >= 0) {
            order->unknown_04 = 0x2ff;
            order->flags.word = (r << 16) | order->flags.half;
        } else {
            order->unknown_04 = 0x6ff;
            order->flags.word = order->flags.half | (((row << 8) | column) << 16);
        }
        return;
    }
    row = here >> 8;
    if (row != func_0013a9a0_side(side)) {
        r = func_0013a870_side(side);
        if (r >= 0) {
            order->unknown_04 = 0x6ff;
            order->flags.word = (r << 16) | order->flags.half;
        } else {
            r = (((row + 1) << 8) | (here & 0xff)) << 16;
            order->unknown_04 = 0x6ff;
            order->flags.word = order->flags.half | r;
        }
    } else {
        order->unknown_04 = 0x6ff;
        order->flags.word = (here << 16) | order->flags.half;
    }
}

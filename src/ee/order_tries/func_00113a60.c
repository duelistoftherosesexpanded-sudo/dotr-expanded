#include "dotr/ee/order_tries.h"

void TaLogic001_CommandInvadeUnit(struct DotrTryOrder *order)
{
    int here;
    int column;

    func_00250360_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (CheckLandAgreement2(3, order->position >> 8, order->position & 0xff) == 1) {
        order->unknown_04 = 0x6ff;
        order->flags.word = order->flags.half | (TaLogic001_GetNextGrassPoint(here & 0xff, here >> 8) << 16);
        return;
    }
    column = here & 0xff;
    order->unknown_04 = 0x6ff;
    if (column < 3 && (here >> 8) < 3) {
        order->flags.word = order->flags.half | 0x3030000;
    } else if (column >= (here >> 8)) {
        order->flags.word = order->flags.half | 0x3050000;
    } else {
        order->flags.word = order->flags.half | 0x5030000;
    }
}

#include "dotr/ee/order_tries.h"

void TaLogic011_CommandDefenseUnit(struct DotrTryOrder *order)
{
    int here;
    int r;

    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    func_00250360_pair(order->position >> 8, order->position & 0xff);
    r = TaLogic011_GetNearMountain(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8);
    if (r >= 0) {
        order->unknown_04 = 0x6ff;
        order->flags.word = (r << 16) | order->flags.half;
    } else {
        order->unknown_04 = 0x5ff;
    }
}

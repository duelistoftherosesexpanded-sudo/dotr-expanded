#include "dotr/ee/order_tries.h"

void TaLogic005_CommandRoutineUnit(struct DotrTryOrder *order)
{
    int here;
    int r;
    int out[2];

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    func_0024de70_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    order->unknown_04 = 0x6ff;
    if (func_001e0290_try(order->position >> 8, order->position & 0xff, 0) >= 1500) {
        r = func_00122680_try(order->position >> 8, order->position & 0xff);
        if (r >= 0) {
            order->flags.word = order->flags.half | (func_00122970_try((r + 1) % 8) << 16);
        } else {
            order->flags.word = order->flags.half | (func_00122810_try(here & 0xff, here >> 8) << 16);
        }
    } else {
        TaLogic005_GetNearDefensePoint(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8, out);
        if (out[0] >= 0) {
            order->flags.word = (out[0] << 16) | order->flags.half;
        } else {
            order->unknown_04 = 0x5ff;
        }
    }
}

#include "dotr/ee/position_rules.h"

void TaLogic019_CommandBtlUnit(struct DotrPositionOrder *order)
{
    int other;
    int side;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    func_001df540_word(order->position >> 8, order->position & 0xff);
    if (func_001e0290_try(order->position >> 8, order->position & 0xff, 0) >= 2500) {
        func_0015e030_order(order);
        return;
    }
    if (TaLogic019_IsEmptyCntrGuard(order->position >> 8, order->position & 0xff, 1) == 1) {
        func_0015e520_order(order, 1);
    } else {
        r = TaLogic019_IsEmptyTrapGuard(order->position >> 8, order->position & 0xff);
        if (r >= 0) {
            order->kind = 0x6ff;
            order->flags.word = (r << 16) | order->flags.half;
        } else if (TaLogic019_IsEmptyCntrGuard(order->position >> 8, order->position & 0xff, 2) == 1) {
            func_0015e520_order(order, 2);
        } else {
            func_0015e030_order(order);
        }
    }
}

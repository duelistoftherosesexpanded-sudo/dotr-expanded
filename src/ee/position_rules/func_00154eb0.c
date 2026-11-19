#include "dotr/ee/position_rules.h"

void TaLogic017_CommandBtlUnit(struct DotrPositionOrder *order)
{
    int here;
    int other;
    int side;
    int r;
    unsigned char deep;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    func_001df540_word(order->position >> 8, order->position & 0xff);
    deep = SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5;
    order->kind = 0x7ff;
    if (TaLogic017_IsLineTop(order->position >> 8, order->position & 0xff) == 1) {
        order->flags.word = (here << 16) | order->flags.half;
        return;
    }
    if (TaLogic017_IsLinePoint((unsigned char)here, here >> 8, deep) == 0) {
        if (TaLogic017_IsLinePoint((unsigned char)here, here >> 8, 0) == 0 && (here >> 8) < 6) {
            order->flags.word = (((here & 0xff) | 0x600) << 16) | order->flags.half;
            return;
        }
    }
    r = TaLogic017_GetEmptyLine(deep);
    if (r >= 0) {
        order->flags.word = (r << 16) | order->flags.half;
        return;
    }
    r = TaLogic017_GetEmptyLine(0);
    if (r >= 0) {
        order->flags.word = (r << 16) | order->flags.half;
        return;
    }
    order->kind = 0x6ff;
    order->flags.word = 0x5030000 | order->flags.half;
}

#include "dotr/ee/order_tries.h"

void TaLogic004_CommandAtkLineUnit(struct DotrTryOrder *order)
{
    struct DotrTryPick out;
    int here;
    int kind;

    SzDuel_GetTurnOwner();
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (TaLogic004_IsUnitOnLine(order->position >> 8, order->position & 0xff) < 0) {
        TaLogic004_GetNearAtkLineEdge(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8, &out);
        if (out.word >= 0) {
            order->unknown_04 = 0x6ff;
            order->flags.word = (out.word << 16) | order->flags.half;
        } else {
            order->unknown_04 = 0x5ff;
        }
    } else {
        kind = (order->unknown_04 >> 8) & 0xff;
        if (kind != 15 && kind != 16) {
            order->unknown_04 = (((here >> 8) < 4 ? 15 : 16) << 8) | 0xff;
            order->flags.word = order->flags.half | 0xffff0000;
        }
    }
}

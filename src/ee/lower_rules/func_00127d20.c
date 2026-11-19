#include "dotr/ee/lower_rules.h"

void TaLogic006_CommandSpellUnit(struct DotrRuleOrder *order)
{
    int r;

    func_00250360_pair(order->position >> 8, order->position & 0xFF);
    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF)) {
    case 745:
        r = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
        order->missions = 0x6FF;
        if ((r >> 8) <= 2) {
            order->flags.word = order->flags.half | 0x03010000;
        } else {
            order->flags.word = order->flags.half | 0x05020000;
        }
        break;
    case 690:
        order->missions = 0x6FF;
        order->flags.word = order->flags.half | 0x04050000;
        break;
    case 685:
        order->missions = 0xCFF;
        order->flags.word = order->flags.half | 0xFFFF0000;
        break;
    default:
        order->missions = 0x5FF;
        break;
    }
}

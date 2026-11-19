#include "dotr/ee/position_rules.h"

void TaLogic018_CommandEfcMonster(struct DotrPositionOrder *order)
{
    int side;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 659:
        if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) == 0 && TaLogic_CommandGunnerUnit(order, 0) == 1) {
            return;
        }
        break;
    case 647:
        if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->kind = 0xbff;
            return;
        }
        break;
    case 655:
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 2, 52, 14)) > 5) {
            order->kind = 0x3ff;
            order->flags.word = order->flags.half | (SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff) << 16);
            return;
        }
        break;
    }
    func_00159d00_order(order);
}

#include "dotr/ee/position_rules.h"

void TaLogic014_CommandEfcMonster(struct DotrPositionOrder *order)
{
    int side;
    int kind;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    kind = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff);
    switch (kind) {
    case 612:
    case 551:
        if (TaLogic014_IsUnitNaturalEffectUse(side, kind) == 1) {
            order->kind = 0x3ff;
            order->flags.word = order->flags.half | (SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff) << 16);
            return;
        }
        break;
    case 570:
        if (TaLogic014_IsActiveMode() == 1) {
            if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
                return;
            }
        }
        break;
    case 575:
        if (TaLogic014_IsActiveMode() == 1) {
            if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) == 0) {
                if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
                    return;
                }
            }
        }
        break;
    }
    func_0014c3b0_order(order);
}

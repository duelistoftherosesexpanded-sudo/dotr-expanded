#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__0013e1d0(struct DotrRuleOrder *order)
{
    int key;

    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                if (TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xFF, 0) == 1) {
                    if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                        order->missions = 0x50A;
                    } else {
                        order->missions = 0x5FF;
                    }
                } else {
                    if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                        order->missions = 0xA05;
                    } else {
                        order->missions = 0x5FF;
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic011_CommandEfcMonster(order);
                } else {
                    func_0013e7b0_order(order, 1);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 2, 0, func_00141b30_key(key) & 0xFF);
                break;
            case 3:
                if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
                    order->missions = 0xBFF;
                } else {
                    order->missions = 0x5FF;
                }
                break;
            default:
                order->missions = 0x5FF;
                break;
            }
        }
        return 1;
    }
    return 0;
}

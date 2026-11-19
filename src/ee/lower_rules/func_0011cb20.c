#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__0011cb20(struct DotrRuleOrder *order)
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
                        order->missions = 0xFF05;
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic_CommandEfcMonster__0011d9b0(order);
                } else {
                    func_0011d110_order(order, 1);
                }
                break;
            case 3:
                TaLogic_CommandTrapUnit__0011dc00(order);
                break;
            case 4:
            case 5:
                order->missions = 0x5FF;
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 4, 0, func_0011fa80_key(key) & 0xFF);
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

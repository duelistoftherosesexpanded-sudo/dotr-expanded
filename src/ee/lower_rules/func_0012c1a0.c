#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__0012c1a0(struct DotrRuleOrder *order, int deep)
{
    int key;

    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                if (TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xFF, 0) == 1
                    || (SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF) >> 8) >= 2) {
                    if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                        order->missions = 0x50A;
                    } else {
                        order->missions = 0x5FF;
                    }
                } else {
                    if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                        order->missions = 0x60A;
                        order->flags.word = order->flags.half | (SzDuel_GetUnitLoc(key == 0 ? 1 : 0, 0) << 16);
                    } else {
                        order->missions = 0x6FF;
                        order->flags.word = order->flags.half | (SzDuel_GetUnitLoc(key == 0 ? 1 : 0, 0) << 16);
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic007_CommandEfcMonster(order, deep);
                } else {
                    func_0012c7f0_order(order, 1, deep);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 5, 0, func_00130780_key(key) & 0xFF);
                break;
            case 3:
                TaLogic_CommandTrapUnit__0012ce50(order);
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

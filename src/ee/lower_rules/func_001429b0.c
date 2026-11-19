#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__001429b0(struct DotrRuleOrder *order)
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
                if (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF) == 352) {
                    func_00143540_order(order);
                } else {
                    TaLogic012_CommandLineUnit(order);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 2, 0, func_001461c0_key(key) & 0xFF);
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

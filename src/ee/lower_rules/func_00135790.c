#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00135790(struct DotrRuleOrder *order)
{
    func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                if (TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xFF, 1) == 1) {
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
                if (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF) == 189) {
                    func_00136010_order(order);
                } else {
                    func_00135d50_order(order, 1);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 2, 1, 0);
                break;
            case 1:
                TaLogic009_CommandSpellUnit(order);
                break;
            case 3:
                func_00136010_order(order);
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

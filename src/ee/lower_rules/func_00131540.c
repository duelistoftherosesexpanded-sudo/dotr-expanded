#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00131540(struct DotrRuleOrder *order)
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
                        order->missions = 0xA06;
                        order->flags.word = (order->flags.word & 0xFFFF0000) | (SzDuel_GetUnitLoc(key == 0 ? 1 : 0, 0) & 0xFF);
                    } else {
                        order->missions = 0x6FF;
                        order->flags.word = order->flags.half | ((SzDuel_GetUnitLoc(key == 0 ? 1 : 0, 0) & 0xFF) << 16);
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                func_00131b30_order(order, 1);
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 4, 0, func_00134ae0_key(key) & 0xFF);
                break;
            case 1:
            case 3:
                TaLogic008_CommandSpellUnit(order);
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

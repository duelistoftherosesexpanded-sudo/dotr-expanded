#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00163660(struct DotrRuleOrder *order)
{
    int key;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
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
                        order->missions = 0xA06;
                        order->flags.word = (order->flags.word & 0xFFFF0000) | 0x103;
                    } else {
                        order->missions = 0x6FF;
                        order->flags.word = order->flags.half | 0x1030000;
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
            case 5:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic020_CommandEfcMonster(order);
                } else {
                    func_00163c50_order(order, 1);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 5, 1, TaLogic_GetWarnHideLvl__001676d0(key) & 0xFF);
                break;
            case 1:
            case 3:
            case 4:
                func_00164260_order(order);
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

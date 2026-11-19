#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00146ec0(struct DotrRuleOrder *order, int deep)
{
    int key;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
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
                if (TaLogic013_IsUnitInvader(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic013_CommandInvadeUnit(order);
                } else if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic013_CommandEfcMonster(order);
                } else {
                    func_001474f0_order(order);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 5, 0, TaLogic_GetWarnHideLvl__0014b330(key) & 0xFF);
                break;
            case 1:
            case 3:
                TaLogic013_CommandSpellUnit(order, deep);
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

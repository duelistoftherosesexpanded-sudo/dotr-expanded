#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__0016d6a0(struct DotrRuleOrder *order)
{
    int kind;
    int key;

    kind = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF);
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
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1
                    || ((unsigned int)(kind - 54) < 4 ? 1 : 0) == 1) {
                    TaLogic022_CommandEfcMonster(order);
                } else {
                    TaLogic022_CommandBtlUnit(order);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 5, 0, func_00172310_key(key) & 0xFF);
                break;
            case 1:
            case 3:
            case 4:
                TaLogic022_CommandSpellUnit(order);
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

#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__0010ce20(struct DotrRuleSet *set, struct DotrRuleOrder *order)
{
    int key;

    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    /* MATCHING DEVICE: the store goes through a 4-bit field view, the reads below use the byte. */
    ((struct DotrRuleSetLow *)set)->low = TaLogic002_IsActiveMode();
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
                        order->flags.word = (order->flags.word & 0xFFFF0000) | 1;
                    } else {
                        order->missions = 0x6FF;
                        order->flags.word = order->flags.half | 0x10000;
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    func_0010d880_rule(order, 1, set->aiActiveRelated & 0xF);
                } else {
                    func_0010d490_rule(order, 1, set->aiActiveRelated & 0xF);
                }
                break;
            case 3:
            case 4:
                func_0010db00_rule(order, 2, 0);
                break;
            case 5:
                func_0010db00_rule(order, 1, 0);
                break;
            case 1:
                TaLogic002_CommandMagicUnit(order);
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 2, 0, TaLogic_GetWarnHideLvl__0010eda0(key) & 0xFF);
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

#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00139d90(struct DotrRuleOrder *order, int deep)
{
    int cell;
    int key;

    cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
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
                        if ((deep & 0xFF) == 1) {
                            order->flags.word = (order->flags.word & 0xFFFF0000)
                                | ((((cell >> 8) + 1) << 8 | (cell & 0xFF)) & 0xFFFF);
                        } else {
                            order->flags.word = (order->flags.word & 0xFFFF0000) | (cell & 0xFFFF);
                        }
                    } else {
                        order->missions = 0x6FF;
                        if ((deep & 0xFF) == 1) {
                            order->flags.word = order->flags.half | (((cell >> 8) + 1) << 8 | (cell & 0xFF)) << 16;
                        } else {
                            order->flags.word = cell << 16 | order->flags.half;
                        }
                    }
                }
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                func_0013a400_order(order, 1, deep);
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 2, 0, TaLogic_GetWarnHideLvl__0013da70(key) & 0xFF);
                break;
            case 1:
            case 3:
                TaLogic010_CommandSpellUnit(order, deep);
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

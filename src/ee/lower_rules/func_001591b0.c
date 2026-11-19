#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__001591b0(struct DotrRuleOrder *order)
{
    int key;
    unsigned int at;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                TaLogic018_CommandLDUnit(order);
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic018_CommandEfcMonster(order);
                } else {
                    func_00159d00_order(order);
                }
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 3, 0, TaLogic_GetWarnHideLvl__0015d120(key) & 0xFF);
                break;
            case 5:
                func_00159d00_order(order);
                if (((order->missions >> 8) & 0xFF) == 2) {
                    at = (unsigned int)order->flags.word >> 16;
                    at = SzDuel_GetUnitLoc((at & 0xFFFF) >> 8, at & 0xFF) << 16;
                    order->missions = 0x6FF;
                    order->flags.word = at | order->flags.half;
                }
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

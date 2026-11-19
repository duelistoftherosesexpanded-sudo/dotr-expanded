#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00167d30(struct DotrRuleOrder *order)
{
    int key;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                TaLogic_CommandStdLDUnit(order, 3, 1, 1);
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                TaLogic021_CommandBtlUnit(order, 1);
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 4, 1, func_0016b0e0_arg(key) & 0xFF);
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

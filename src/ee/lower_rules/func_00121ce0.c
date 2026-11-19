#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__00121ce0(struct DotrRuleOrder *order)
{
    int key;

    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                TaLogic_CommandStdLDUnit(order, 3, 2, 0);
            }
        } else {
            switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1) {
                    TaLogic_CommandEfcMonster__001229f0(order);
                } else {
                    func_00122270_order(order, 1);
                }
                break;
            case 1:
            case 5:
                order->missions = 0x5FF;
                break;
            case 2:
                TaLogic_CommandEncUnit(order, 2, 0, func_001254a0_key(key) & 0xFF);
                break;
            case 4:
                TaLogic005_CommandTrapBUnit(order);
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

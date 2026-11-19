#include "dotr/ee/lower_rules.h"

void TaLogic013_CommandEfcMonster(struct DotrRuleOrder *order)
{
    int key;

    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF)) {
    case 291:
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, key, 1, 6, 14)) >= 3) {
            order->missions = 0x3FF;
            order->flags.word = order->flags.half | SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF) << 16;
            break;
        }
        /* fall through */
    default:
        func_001474f0_order(order);
        break;
    }
}

#include "dotr/ee/lower_rules.h"

void TaLogic010_CommandSpellUnit(struct DotrRuleOrder *order, int deep)
{
    int key;
    int cell;
    int strongestEnemyUnit;

    (void)deep;
    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    switch (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF) >> 5) {
    case 1:
        strongestEnemyUnit = TaLogic010_GetUseThunderBolt(key);
        if (strongestEnemyUnit >= 0) {
            order->missions = 0x6FF;
            order->flags.word = order->flags.half | ((cell >> 8) << 8 | (SzDuel_GetUnitLoc(strongestEnemyUnit >> 8, strongestEnemyUnit & 0xFF) & 0xFF)) << 16;
            return;
        }
        order->missions = 0x5FF;
        break;
    case 3:
        order->missions = 0x3FF;
        order->flags.word = order->flags.half | ((cell & 0xFF) | (SzDuel_GetUnitLoc(key, 0) >> 8) << 8) << 16;
        return;
    }
    order->missions = 0x5FF;
}

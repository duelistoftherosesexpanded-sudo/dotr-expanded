#include "dotr/ee/lower_rules.h"

void TaLogic005_CommandTrapBUnit(struct DotrRuleOrder *order)
{
    int x;
    int cell;
    int y;
    int best;
    int found;
    int id;

    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0) {
        Ta_GetCurrentSide(order->position >> 8, order->position & 0xFF);
        cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
        id = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF);
        found = -1;
        switch (id) {
        case 829:
            order->missions = 0x3FF;
            order->flags.word = order->flags.half;
            break;
        case 818:
            order->missions = 0x3FF;
            order->flags.word = order->flags.half | 0x1000000;
            break;
        case 820:
            best = found;
            order->missions = 0x3FF;
            for (x = 0; x < 7; x++) {
                for (y = 0; y < 7; y++) {
                    if (SzDuel_GetField(x, y) == 1 && SzDuel_GetMapUnit(x, y) == 0
                        && best < ((x < (cell & 0xFF) ? (cell & 0xFF) : x) - ((cell & 0xFF) < x ? (cell & 0xFF) : x)) + ((y < (cell >> 8) ? (cell >> 8) : y) - ((cell >> 8) < y ? (cell >> 8) : y))) {
                        found = y << 8 | x;
                    }
                }
            }
            if (found >= 0) {
                order->flags.word = order->flags.half | 0x1000000;
                break;
            }
            order->missions = 0x5FF;
            break;
        }
        order->missions = 0x5FF;
    }
}

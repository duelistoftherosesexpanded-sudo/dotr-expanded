#include "dotr/ee/lower_rules.h"

void TaLogic021_CommandInvadeUnit(struct DotrRuleOrder *order)
{
    int pair;
    int cell;
    int kind;

    pair = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    kind = (order->missions >> 8) & 0xFF;
    if (kind == 16) {
        if ((cell >> 8) == 6) {
            order->missions = 0xFFF;
            order->flags.word = (cell & 0xFF) << 16 | order->flags.half;
        } else {
            order->missions = 0x10FF;
            order->flags.word = ((cell & 0xFF) | 0x600) << 16 | order->flags.half;
        }
    } else if (kind == 15) {
        if ((cell >> 8) == 0) {
            order->missions = 0x10FF;
            order->flags.word = ((cell & 0xFF) | 0x600) << 16 | order->flags.half;
        } else {
            order->missions = 0xFFF;
            order->flags.word = (cell & 0xFF) << 16 | order->flags.half;
        }
    } else if ((cell >> 8) < 4) {
        if ((cell >> 8) == 0) {
            order->missions = 0x10FF;
            order->flags.word = (cell & 0xFF) << 16 | order->flags.half;
        } else if ((cell & 0xFF) == 3) {
            order->missions = 0x6FF;
            order->flags.word = order->flags.half | 0x30000;
        } else if ((cell & 0xFF) > 3) {
            order->missions = 0x6FF;
            order->flags.word = order->flags.half | 0x60000;
        } else {
            order->missions = 0x6FF;
            order->flags.word = order->flags.half;
        }
    } else if (SzDuel_GetMapUnit(3, 3) == 0) {
        order->missions = 0x6FF;
        order->flags.word = order->flags.half | 0x3030000;
    } else if (SzDuel_GetMapUnit(2, 3) == 0) {
        order->missions = 0x6FF;
        order->flags.word = order->flags.half | 0x3020000;
    } else if (SzDuel_GetMapUnit(2, 3) == 0) {
        order->missions = 0x6FF;
        order->flags.word = order->flags.half | 0x3040000;
    } else {
        order->missions = 0x6FF;
        order->flags.word = order->flags.half | SzDuel_GetUnitLoc(pair == 0 ? 1 : 0, 0) << 16;
    }
}

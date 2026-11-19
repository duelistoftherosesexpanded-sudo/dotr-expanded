#include "dotr/ee/lower_rules.h"

void TaLogic002_CommandMagicUnit(struct DotrRuleOrder *order)
{
    unsigned *entry;
    int cell;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xFF) == 0) {
        return;
    }
    entry = func_00250740_word(order->position >> 8, order->position & 0xFF, 0);
    if (entry == 0) {
        return;
    }
    switch (*entry >> 8) {
    case 13:
        order->missions = 0x5FF;
        break;
    case 2:
        cell = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF) & 0xFFFF;
        r = TaLogic_GetLandChange(func_001df540_word(order->position >> 8, order->position & 0xFF), cell & 0xFF, cell >> 8, 0,
                          6, 2) & 0xFFFF;
        order->missions = 0x6FF;
        if (r < 0) {
            order->flags.word = ((cell & 0xFFFF) << 16) | order->flags.half;
        } else {
            order->flags.word = ((r & 0xFFFF) << 16) | order->flags.half;
        }
        break;
    }
}

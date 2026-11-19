#include "dotr/ee/position_rules.h"

void TaLogic014_CommandSpellUnit(struct DotrPositionOrder *order)
{
    unsigned *word;
    int here;
    int r;

    func_00250360_pair(order->position >> 8, order->position & 0xff);
    word = func_00250740_word(order->position >> 8, order->position & 0xff, 0);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (word == 0) {
        order->kind = 0x5ff;
        return;
    }
    switch (*word >> 8) {
    case 2:
        if (TaLogic014_IsActiveMode() == 1) {
            r = TaLogic_GetLandChange(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8, 4, 9, 2);
            if (r >= 0) {
                order->kind = 0x6ff;
                order->flags.word = (r << 16) | order->flags.half;
            }
        }
        break;
    }
    order->kind = 0x5ff;
}

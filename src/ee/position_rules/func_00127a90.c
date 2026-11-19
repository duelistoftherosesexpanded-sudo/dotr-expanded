#include "dotr/ee/position_rules.h"

void TaLogic006_CommandInvadeUnit(struct DotrPositionOrder *order)
{
    int cell;

    cell = SzDuel_GetUnitLoc((int)(unsigned)order->position >> 8, order->position & 0xff);
    order->kind = 0x6ff;
    if (cell >> 8 < 3) {
        order->flags.word = order->flags.half | 0x3010000;
    } else {
        order->flags.word = order->flags.half | 0x5020000;
    }
}

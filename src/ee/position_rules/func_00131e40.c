#include "dotr/ee/position_rules.h"

void TaLogic008_CommandInvadeUnit(struct DotrPositionOrder *order)
{
    struct DotrSpotNibbles bytes[12];
    int owner;
    int here;
    unsigned *word;
    int i;
    struct DotrSpotNibbles *p;
    int spot;

    owner = func_00250360_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    word = func_001e0b70_word(order->position >> 8, order->position & 0xff, 1);
    for (i = 0, p = bytes; i < 12; i++, p++) p->low = p->high = 0xf;
    if (word != 0 && *word >> 8 == 8) {
        order->kind = 0x2ff;
        order->flags.word = ((owner == 0 ? 1 : 0) << 24) | order->flags.half;
        return;
    }
    order->kind = 0x6ff;
    spot = TaLogic008_GetNextInvadePoint(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8);
    if (spot >= 0) {
        order->flags.word = (spot << 16) | order->flags.half;
        return;
    }
    order->kind = 0x5ff;
}

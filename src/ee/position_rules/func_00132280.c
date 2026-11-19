#include "dotr/ee/position_rules.h"

void TaLogic008_CommandSpellUnit(struct DotrPositionOrder *order)
{
    int side;
    unsigned char level;
    int here;
    unsigned *word;
    int r;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    level = SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    word = func_001e0b70_word(order->position >> 8, order->position & 0xff, 0);
    switch (level) {
    case 3:
        order->kind = 0x3ff;
        r = TaLogic008_GetNextInvadePoint(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        order->kind = 0x5ff;
        return;
    case 1:
        if (word == 0) {
            return;
        }
        switch (*word >> 8) {
        case 2:
        case 72:
            order->kind = 0x6ff;
            r = func_001349a0_side(side);
            if (r >= 0) {
                order->flags.word = (r << 16) | order->flags.half;
                return;
            }
            order->flags.word = 0x60000 | order->flags.half;
            return;
        case 37:
            order->kind = 0x3ff;
            r = TaLogic008_GetNextInvadePoint(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8);
            if (r >= 0) {
                order->flags.word = (r << 16) | order->flags.half;
                return;
            }
            order->kind = 0x5ff;
            return;
        }
        break;
    }
    order->kind = 0x5ff;
}

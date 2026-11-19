#include "dotr/ee/position_rules.h"

void TaLogic009_CommandSpellUnit(struct DotrPositionOrder *order)
{
    int here;
    int r;

    func_00250360_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 695:
        r = TaLogic_GetLandChange(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8, 6, 5, 2);
        if (r >= 0 && (r >> 8) < 4) {
            order->kind = 0x6ff;
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        break;
    case 687:
        order->kind = 0xcff;
        order->flags.word = 0xffff0000 | order->flags.half;
        return;
    case 732:
        order->kind = 0xdff;
        order->flags.word = 0xffff0000 | order->flags.half;
        return;
    }
    order->kind = 0x5ff;
}

#include "dotr/ee/slot_orders.h"

void TaLogic013_CommandSpellUnit(struct DotrSlotOrder *order, int flag)
{
    int here;
    int side;
    unsigned *effect;
    int set;
    int i;
    int r;
    int cell;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    effect = func_00250740_word(order->position >> 8, order->position & 0xff, 0);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (effect == 0) {
        order->unknown_04 = 0x5ff;
        return;
    }
    switch (*effect >> 8) {
    case 20:
        order->unknown_04 = 0xcff;
        return;
    case 2:
        if ((unsigned char)flag == 1) {
            r = TaLogic_GetLandChange(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8, 2, 6, 2);
            if (r >= 0) {
                order->unknown_04 = 0x6ff;
                order->flags.word = (r << 16) | order->flags.half;
            }
        }
        break;
    case 4:
        if ((unsigned char)flag == 1) {
            set = SearchUnitByRange(0, 0, side, 2, 30, 14);
            for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(set); i++) {
                r = Ta_GetUnitOwnerIDFromLstCount(set, i);
                cell = SzDuel_GetUnitLoc(r >> 8, r & 0xff);
                if (TaLogic013_IsUseHarpySweaper(side, here & 0xff, cell >> 8) == 1) {
                    order->unknown_04 = 0x6ff;
                    order->flags.word = order->flags.half | ((((cell >> 8) << 8) | (here & 0xff)) << 16);
                    return;
                }
                if (TaLogic013_IsUseHarpySweaper(side, cell & 0xff, here >> 8) == 1) {
                    order->unknown_04 = 0x6ff;
                    order->flags.word = order->flags.half | ((((here >> 8) << 8) | (cell & 0xff)) << 16);
                    return;
                }
            }
        }
        break;
    }
    order->unknown_04 = 0x5ff;
}

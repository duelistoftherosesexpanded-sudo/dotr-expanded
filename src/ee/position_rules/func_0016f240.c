#include "dotr/ee/position_rules.h"
/* File-local copy of the TaLogic022_CommandBtlUnit (0x16dca0) body, which the target contains
 * expanded in place (no call); with it inlined the compiler drops the dead
 * duplicated delay-slot words. Not a claim about an original function. */
static inline void chooser(struct DotrPositionOrder *order)
{
    int other;
    int side;

            if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
                return;
            }
            side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
            other = (side == 0 ? 1 : 0) << 8;
            SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
            SzDuel_GetUnitLoc(other >> 8, other & 0xff);
            func_001df540_word(order->position >> 8, order->position & 0xff);
            if (func_001e0290_try(order->position >> 8, order->position & 0xff, 0) >= 2500) {
                func_0016dda0_order(order);
            } else if (TaLogic022_IsEmptyCntrGuard(order->position >> 8, order->position & 0xff, 1) == 1) {
                func_0016e0e0_order(order, 1);
            } else {
                func_0016dda0_order(order);
            }
}

void TaLogic022_CommandSpellUnit(struct DotrPositionOrder *order)
{
    int kind;
    int side;
    unsigned *word;
    int here;
    int r;

    SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff);
    kind = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff);
    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    word = func_00250740_word(order->position >> 8, order->position & 0xff, 0);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    switch (*word >> 8) {
    case 20:
        order->kind = 0xcff;
        return;
    case 4:
        r = func_0016cbb0_word(side, word);
        if (r >= 0) {
            order->kind = 0x6ff;
            if ((*word & 0x3f) == 0) {
                order->flags.word = order->flags.half | ((((here >> 8) << 8) | (r & 0xff)) << 16);
            } else {
                order->flags.word = (r << 16) | order->flags.half;
            }
            return;
        }
        break;
    case 2:
        r = TaLogic_GetLandChange(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8, ((unsigned char *)word)[4], 8, 2);
        if (r >= 0) {
            order->kind = 0x6ff;
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        break;
    case 37:
        if ((*word & 0x3f) == 33) {
            chooser(order);
            return;
        }
        break;
    case 31:
        if (kind == 732) {
            order->kind = 0xdff;
            return;
        }
        break;
    }
    order->kind = 0x5ff;
}

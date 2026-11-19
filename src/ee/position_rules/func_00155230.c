#include "dotr/ee/position_rules.h"

void TaLogic017_CommandSpellUnit(struct DotrPositionOrder *order)
{
    int here;
    int kind;
    int r;
    unsigned char deep;

    func_00250360_pair(order->position >> 8, order->position & 0xff);
    deep = SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    kind = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff);
    switch (deep) {
    case 1:
    case 3:
    case 2:
        order->kind = 0x7ff;
        if (TaLogic017_IsLineTop(order->position >> 8, order->position & 0xff) == 1) {
            order->flags.word = (here << 16) | order->flags.half;
            break;
        }
        if (TaLogic017_IsLinePoint((unsigned char)here, here >> 8, deep) == 0) {
            if (TaLogic017_IsLinePoint((unsigned char)here, here >> 8, 0) == 0 && (here >> 8) < 6) {
                order->flags.word = (((here & 0xff) | 0x600) << 16) | order->flags.half;
                break;
            }
        }
        r = TaLogic017_GetEmptyLine(deep);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            break;
        }
        r = TaLogic017_GetEmptyLine(0);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            break;
        }
        order->kind = 0x6ff;
        order->flags.word = 0x5030000 | order->flags.half;
        break;
    case 4:
        if (kind != 820 && kind != 823) {
            order->kind = 0x3ff;
            order->flags.word = (here << 16) | order->flags.half;
        } else {
            order->kind = 0x6ff;
            r = TaLogic017_GetEmptyLine(deep);
            if (r >= 0) {
                order->flags.word = (r << 16) | order->flags.half;
                break;
            }
            r = TaLogic017_GetEmptyLine(0);
            if (r >= 0) {
                order->flags.word = (r << 16) | order->flags.half;
                break;
            }
            order->kind = 0x6ff;
            order->flags.word = 0x5030000 | order->flags.half;
        }
        break;
    default:
        order->kind = 0x5ff;
        break;
    }
}

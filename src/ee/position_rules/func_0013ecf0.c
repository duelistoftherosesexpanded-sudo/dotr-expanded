#include "dotr/ee/position_rules.h"

void TaLogic011_CommandEfcMonster(struct DotrPositionOrder *order)
{
    unsigned char buf[112];
    struct DotrPositionCell cells[22];
    int side;
    int here;
    int i;
    int mine;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    (void)cells; /* filled and not read; the statement only quiets the host compiler */
    func_0027e770_buf(buf);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 386:
        if (TaLogic011_IsUseBindChain(side) == 1) {
            order->kind = 0x6ff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        }
        break;
    case 388:
        mine = func_001df470_side(side);
        if (mine < func_001df470_side(side == 0 ? 1 : 0)) {
            order->kind = 0x6ff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        }
        break;
    case 377:
        if (TaLogic011_IsUseLifeHourGlass(side) == 1) {
            if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
                order->kind = 0xbff;
                return;
            }
        }
        break;
    case 390:
        if (TaLogic011_IsUseBraveHourGlass(side) == 1) {
            if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
                order->kind = 0xbff;
                return;
            }
        }
        break;
    }
    func_0013e7b0_order(order, 1);
}

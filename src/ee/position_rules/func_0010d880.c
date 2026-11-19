#include "dotr/ee/position_rules.h"

void TaLogic_CommandEfcMonster__0010d880(struct DotrPositionOrder *order, int unused, int deep)
{
    unsigned char ignoreList[112];
    struct DotrPositionCell cells[22];
    unsigned short a;
    unsigned short b;
    int side;
    int here;
    int word;
    int i;
    int r;

    (void)unused;
    if ((word = func_001df540_word(order->position >> 8, order->position & 0xff)) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    b = 0;
    a = 0;
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(ignoreList);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 402:
    case 403:
        for (i = 1; i <= 2; i++) {
            r = func_0027de20_order(order, 0, word, here & 0xff, here >> 8, side == 0 ? 1 : 0, i & 0xff, TaLogic_GetWarnHideLvl__0010eda0(side), cells, 0, &a, &b, 2, 0, 2400, ignoreList);
            switch (r) {
            case 3:
            case 2:
                order->kind = 0x3ff;
                order->flags.word = (here << 16) | order->flags.half;
                return;
            case 1:
                return;
            case 0:
                order->kind = 0x3ff;
                order->flags.word = (here << 16) | order->flags.half;
                return;
            }
        }
        order->kind = 0x3ff;
        order->flags.word = (here << 16) | order->flags.half;
        return;
    case 417:
        if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
            return;
        }
        func_0010d490_pos(order, 1, deep);
        return;
    case 408:
        if (func_001df470_side(side) < 1000) {
            func_0010db00_pos(order, 0, 0);
        }
        func_0010d490_pos(order, 1, deep);
        break;
    default:
        func_0010d490_pos(order, 1, deep);
        break;
    }
}

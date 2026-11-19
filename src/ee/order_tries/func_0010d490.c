#include "dotr/ee/order_tries.h"

void TaLogic_CommandBtlUnit__0010d490(struct DotrTryOrder *order, int flag, unsigned char isActive)
{
    unsigned char ignoreList[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int unitLoc;
    int side;
    int playerLeaderLoc;
    int other;
    int unit;
    int i;
    int r;
    int k;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    unitLoc = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    playerLeaderLoc = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    unit = func_001df540_word(order->position >> 8, order->position & 0xff);
    func_0027e770_buf(ignoreList);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (flag != 0) {
        if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, unitLoc & 0xff, unitLoc >> 8, other >> 8, other & 0xff, playerLeaderLoc & 0xff, playerLeaderLoc >> 8, 2) > 0) {
            order->unknown_04 = 0x2ff;
            order->flags.word = (other << 16) | order->flags.half;
            return;
        }
    }
    b = 0;
    a = 0;
    if (isActive == 0) {
        for (i = 1; i <= 2; i++) {
            r = func_0010d820_try(order, 0, unit, unitLoc & 0xff, unitLoc >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, i & 0xff, TaLogic_GetWarnHideLvl__0010eda0(side), cells, 0, &a, &b, 2, ignoreList);
            k = 1;
            switch (r) {
            case 3:
                order->unknown_04 = 0x6ff;
                order->flags.word = (unitLoc << 16) | order->flags.half;
                return;
            case 2:
                order->unknown_04 = 0x3ff;
                order->flags.word = (unitLoc << 16) | order->flags.half;
                return;
            case 1:
                return;
            case 0:
                func_0010db00_order(order, k, k);
                return;
            }
        }
        func_0010db00_order(order, k, 0);
    } else {
        for (i = 1; i < 6; i++) {
            r = func_0010d820_try(order, 0, unit, unitLoc & 0xff, unitLoc >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, i & 0xff, TaLogic_GetWarnHideLvl__0010eda0(side), cells, 0, &a, &b, 6, ignoreList);
            k = 1;
            switch (r) {
            case 3:
                order->unknown_04 = 0x6ff;
                order->flags.word = (unitLoc << 16) | order->flags.half;
                return;
            case 2:
                order->unknown_04 = 0x3ff;
                order->flags.word = (unitLoc << 16) | order->flags.half;
                return;
            case 1:
                return;
            case 0:
                func_0010db00_order(order, k, k);
                return;
            }
        }
        func_0010db00_order(order, k, 0);
    }
}

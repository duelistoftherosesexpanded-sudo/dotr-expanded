#include "dotr/ee/order_tries.h"

void TaLogic_CommandBtlUnit__001188f0(struct DotrTryOrder *order, int flag)
{
    unsigned char ignoreList[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int side;
    int other;
    int unitLoc;
    int there;
    int pUnit;
    int i;
    int nearUnitResult;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    unitLoc = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    pUnit = func_001df540_word(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (flag != 0) {
        if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, unitLoc & 0xff, unitLoc >> 8, other >> 8, other & 0xff, there & 0xff, there >> 8, 3) > 0) {
            order->unknown_04 = 0x2ff;
            order->flags.word = (other << 16) | order->flags.half;
            return;
        }
    }
    func_0027e770_buf(ignoreList);
    b = 0;
    a = 0;
    for (there = 1; there <= 3; there++) {
        nearUnitResult = func_00118b60_try(order, 0, pUnit, unitLoc & 0xff, unitLoc >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, there & 0xff, TaLogic_GetWarnHideLvl__0011aa10(side), cells, 0, &a, &b, 3, ignoreList);
        switch (nearUnitResult) {
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
            order->unknown_04 = 0x5ff;
            return;
        }
    }
    func_00118bc0_order(order);
}

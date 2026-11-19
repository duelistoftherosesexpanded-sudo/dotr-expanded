#include "dotr/ee/order_tries.h"

void TaLogic013_CommandBtlUnit(struct DotrTryOrder *order)
{
    unsigned char ignoreList[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int side;
    int unitLoc;
    int other;
    int there;
    int unit;
    int i;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    unitLoc = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    unit = func_001df540_word(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(ignoreList);
    if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, unitLoc & 0xff, unitLoc >> 8, other >> 8, other & 0xff, there & 0xff, there >> 8, 2) > 0) {
        order->unknown_04 = 0x2ff;
        order->flags.word = (other << 16) | order->flags.half;
        return;
    }
    b = 0;
    a = 0;
    for (there = 1; there <= 2; there++) {
        r = func_001477d0_try(order, 0, unit, unitLoc & 0xff, unitLoc >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, there & 0xff, TaLogic_GetWarnHideLvl__0014b330(side), cells, 0, &a, &b, 2, ignoreList);
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
            order->unknown_04 = 0x3ff;
            order->flags.word = (unitLoc << 16) | order->flags.half;
            return;
        }
    }
    if (TaLogic013_IsDefensePoint(unitLoc & 0xff, unitLoc >> 8) == 1) {
        order->unknown_04 = 0x6ff;
        order->flags.word = (unitLoc << 16) | order->flags.half;
        return;
    }
    r = TaLogic013_GetEmptyLine();
    if (r >= 0) {
        order->unknown_04 = 0x6ff;
        order->flags.word = (r << 16) | order->flags.half;
        return;
    }
    order->unknown_04 = 0x6ff;
    order->flags.word = order->flags.half | (TaLogic013_GetInvadePoint(side, unitLoc & 0xff, unitLoc >> 8) << 16);
}

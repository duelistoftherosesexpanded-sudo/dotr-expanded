#include "dotr/ee/order_tries.h"

void TaLogic019_CommandAtkUnit(struct DotrTryOrder *order)
{
    unsigned char ignoreList[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int unitLoc;
    int other;
    int side;
    int enemyUnitsLoc;
    int pUnit;
    int i;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    unitLoc = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    enemyUnitsLoc = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    pUnit = func_001df540_word(order->position >> 8, order->position & 0xff);
    func_0027e770_buf(ignoreList);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, (unsigned char)unitLoc, unitLoc >> 8, other >> 8, other & 0xff, (unsigned char)enemyUnitsLoc, enemyUnitsLoc >> 8, 5) > 0) {
        order->unknown_04 = 0x2ff;
        order->flags.word = (other << 16) | order->flags.half;
        return;
    }
    b = 0;
    a = 0;
    for (other = 1; other <= 5; other++) {
        r = func_0015e8f0_try(order, 0, pUnit, (unsigned char)unitLoc, unitLoc >> 8, side == 0 ? 1 : 0, other & 0xff, TaLogic_GetWarnHideLvl__00162580(side), cells, 0, &a, &b, 5, ignoreList);
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
    order->unknown_04 = 0x6ff;
    if (((unitLoc >> 8) <= 3 && (enemyUnitsLoc >> 8) <= 3) || ((unitLoc >> 8) > 3 && (enemyUnitsLoc >> 8) > 3)) {
        order->flags.word = (enemyUnitsLoc << 16) | order->flags.half;
    } else {
        order->flags.word = (((unitLoc & 0xff) | 0x300) << 16) | order->flags.half;
    }
}

#include "dotr/ee/order_tries.h"

void TaLogic015_CommandBtlUnit(struct DotrTryOrder *order)
{
    unsigned char buf[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int unitLocation;
    int other;
    int side;
    int there;
    int pUnit;
    int i;
    int r;
    unsigned char isActive;

    isActive = TaLogic015_IsActiveMode();
    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    unitLocation = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    pUnit = func_001df540_word(order->position >> 8, order->position & 0xff);
    func_0027e770_buf(buf);
    if (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff) == 686) {
        order->unknown_04 = 0xcff;
        return;
    }
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, (unsigned char)unitLocation, unitLocation >> 8, other >> 8, other & 0xff, (unsigned char)there, there >> 8, 2) > 0) {
        if ((SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5) == 0) {
            order->unknown_04 = 0x2ff;
        } else {
            order->unknown_04 = 0xbff;
        }
        order->flags.word = (other << 16) | order->flags.half;
        return;
    }
    b = 0;
    a = 0;
    for (other = 1; other <= (isActive ? 5 : 2); other++) {
        r = func_00150be0_try(order, 0, pUnit, (unsigned char)unitLocation, unitLocation >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, other & 0xff, TaLogic_GetWarnHideLvl__00153ed0(side), cells, 0, &a, &b, (unsigned char)(isActive ? 5 : 2), buf);
        switch (r) {
        case 3:
            order->unknown_04 = 0x6ff;
            order->flags.word = (unitLocation << 16) | order->flags.half;
            return;
        case 2:
            order->unknown_04 = 0x3ff;
            order->flags.word = (unitLocation << 16) | order->flags.half;
            return;
        case 1:
            if (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5) {
                order->unknown_04 = 0xbff;
            }
            return;
        case 0:
            order->unknown_04 = 0x3ff;
            order->flags.word = (unitLocation << 16) | order->flags.half;
            return;
        }
    }
    order->unknown_04 = 0x6ff;
    if (((unitLocation >> 8) <= 3 && (there >> 8) <= 3) || ((unitLocation >> 8) > 3 && (there >> 8) > 3)) {
        order->flags.word = (there << 16) | order->flags.half;
    } else {
        order->flags.word = (((unitLocation & 0xff) | 0x300) << 16) | order->flags.half;
    }
}

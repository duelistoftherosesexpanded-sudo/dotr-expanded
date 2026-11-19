#include "dotr/ee/order_tries.h"

void TaLogic009_CommandBtlUnit(struct DotrTryOrder *order, int flag)
{
    unsigned char buf[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int side;
    int here;
    int there;
    int other;
    int pUnit;
    int i;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    pUnit = func_001df540_word(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(buf);
    if (flag != 0) {
        if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, here & 0xff, here >> 8, other >> 8, other & 0xff, there & 0xff, there >> 8, 2) > 0) {
            order->unknown_04 = 0x2ff;
            order->flags.word = (other << 16) | order->flags.half;
            return;
        }
    }
    b = 0;
    a = 0;
    for (there = 1; there <= 2; there++) {
        r = func_00135fb0_try(order, 0, pUnit, here & 0xff, here >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, there & 0xff, 0, cells, 0, &a, &b, 2, buf);
        switch (r) {
        case 3:
            order->unknown_04 = 0x6ff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        case 2:
            order->unknown_04 = 0x3ff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        case 1:
            return;
        case 0:
            order->unknown_04 = 0x5ff;
            return;
        }
    }
    order->unknown_04 = 0x5ff;
}

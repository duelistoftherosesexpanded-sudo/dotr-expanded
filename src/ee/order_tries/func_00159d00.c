#include "dotr/ee/order_tries.h"

void TaLogic018_CommandBtlUnit(struct DotrTryOrder *order)
{
    unsigned char buf[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int here;
    int other;
    int side;
    int there;
    int word;
    int i;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    func_0027e770_buf(buf);
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    word = func_001df540_word(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, (unsigned char)here, here >> 8, other >> 8, other & 0xff, (unsigned char)there, there >> 8, 3) > 0) {
        order->unknown_04 = 0x2ff;
        order->flags.word = (other << 16) | order->flags.half;
        return;
    }
    b = 0;
    a = 0;
    for (other = 1; other <= 3; other++) {
        r = func_00159f80_try(order, 0, word, (unsigned char)here, here >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, other & 0xff, TaLogic_GetWarnHideLvl__0015d120(side), cells, 0, &a, &b, 3, buf);
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
            order->unknown_04 = 0x3ff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        }
    }
    order->unknown_04 = 0x6ff;
    order->flags.word = (there << 16) | order->flags.half;
}

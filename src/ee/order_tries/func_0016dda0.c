#include "dotr/ee/order_tries.h"

void TaLogic022_CommandAtkUnit(struct DotrTryOrder *order)
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
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(other >> 8, other & 0xff);
    word = func_001df540_word(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(buf);
    if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, here & 0xff, here >> 8, other >> 8, other & 0xff, there & 0xff, there >> 8, 5) > 0) {
        order->unknown_04 = 0x2ff;
        order->flags.word = (other << 16) | order->flags.half;
        return;
    }
    b = 0;
    a = 0;
    for (other = 1; other <= 5; other++) {
        r = func_0016e4b0_try(order, 0, word, here & 0xff, here >> 8, side == 0 ? 1 : 0, other & 0xff, TaLogic_GetWarnHideLvl__00172310(side), cells, 0, &a, &b, 5, buf);
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

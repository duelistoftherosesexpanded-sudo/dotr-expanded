#include "dotr/ee/order_tries.h"

void TaLogic007_CommandBtlUnit(struct DotrTryOrder *order, int flag, unsigned char deep)
{
    unsigned char buf[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int here;
    int side;
    int there;
    int other;
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
    func_0027e770_buf(buf);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (flag != 0) {
        if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, here & 0xff, here >> 8, other >> 8, other & 0xff, there & 0xff, there >> 8, deep == 1 ? 5 : 2) > 0) {
            order->unknown_04 = 0x2ff;
            order->flags.word = (other << 16) | order->flags.half;
            return;
        }
    }
    b = 0;
    a = 0;
    if (deep == 0) {
        for (i = 1; i <= 2; i++) {
            r = func_0012cb80_try(order, 0, word, here & 0xff, here >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, i & 0xff, TaLogic_GetWarnHideLvl__00130780(side), cells, 0, &a, &b, 2, buf);
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
    } else {
        for (i = 1; i < 5; i++) {
            r = func_0012cb80_try(order, 0, word, here & 0xff, here >> 8, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, i & 0xff, TaLogic_GetWarnHideLvl__00130780(side), cells, 0, &a, &b, 5, buf);
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
}

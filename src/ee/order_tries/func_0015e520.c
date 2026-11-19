#include "dotr/ee/order_tries.h"

void TaLogic019_CommandLineUnit(struct DotrTryOrder *order, unsigned char deep)
{
    unsigned char buf[112];
    struct DotrTryCell cells[22];
    unsigned short a;
    unsigned short b;
    int side;
    int here;
    int other;
    int there;
    int word;
    int i;
    int r;
    int column;
    int row;
    int line;

    line = deep == 1 ? 3 : 4;
    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    other = (side == 0 ? 1 : 0) << 8;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    row = other >> 8;
    column = other & 0xff;
    there = SzDuel_GetUnitLoc(row, column);
    word = func_001df540_word(order->position >> 8, order->position & 0xff);
    func_0027e770_buf(buf);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    if (TaLogic_IsRootOpen(order->position >> 8, order->position & 0xff, here & 0xff, here >> 8, row, column, there & 0xff, there >> 8, 2) > 0) {
        order->unknown_04 = 0x2ff;
        order->flags.word = (other << 16) | order->flags.half;
        return;
    }
    column = here & 0xff;
    row = here >> 8;
    b = 0;
    a = 0;
    for (there = 1; there <= 2; there++) {
        r = func_0015e8f0_try(order, 0, word, column, row, func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, there & 0xff, TaLogic_GetWarnHideLvl__00162580(side), cells, 0, &a, &b, 2, buf);
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
    if (row == line) {
        switch ((order->unknown_04 >> 8) & 0xff) {
        case 16:
            if (column > 1) {
                order->flags.word = (row << 24) | order->flags.half;
            } else {
                order->unknown_04 = 0xfff;
                order->flags.word = (((row << 8) | 5) << 16) | order->flags.half;
            }
            break;
        case 15:
            if (column < 5) {
                order->flags.word = (((row << 8) | 5) << 16) | order->flags.half;
            } else {
                order->unknown_04 = 0x10ff;
                order->flags.word = (((row << 8) | 1) << 16) | order->flags.half;
            }
            break;
        default:
            if (func_0019a5c8() % 2 == 0) {
                order->unknown_04 = 0x10ff;
                order->flags.word = (((row << 8) | 1) << 16) | order->flags.half;
            } else {
                order->unknown_04 = 0xfff;
                order->flags.word = (((row << 8) | 5) << 16) | order->flags.half;
            }
            break;
        }
        return;
    }
    order->unknown_04 = 0x6ff;
    order->flags.word = order->flags.half | (((line << 8) | (unsigned char)here) << 16);
}

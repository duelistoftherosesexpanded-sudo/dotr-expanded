#include "dotr/ee/position_rules.h"

void TaLogic_CommandTrapUnit__00118f50(struct DotrPositionOrder *order)
{
    unsigned char buf[112];
    struct DotrPositionCell cells[22];
    unsigned short a;
    unsigned short b;
    unsigned short c;
    int side;
    int here;
    int got;
    int word;
    int i;
    int r;

    if (func_001df540_word(order->position >> 8, order->position & 0xff) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    word = func_001df540_word(order->position >> 8, order->position & 0xff);
    order->kind = 0xbff;
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(buf);
    c = 0;
    b = 0;
    a = 0;
    got = 0;
    for (i = 1; i < 3; i++) {
        got = func_002770e0_cells(got, word, here & 0xff, here >> 8,
            func_0024de70_pair(order->position >> 8, order->position & 0xff) == 0 ? 1 : 0, i & 0xff, 0,
            TaLogic_GetWarnHideLvl__0011aa10(side), cells, &c, &a, &b, buf);
        r = TaLogic_GetTrapNearCard(order->position >> 8, order->position & 0xff, here & 0xff, here >> 8, cells, 3, 31, 1, 2,
            0, TaLogic_GetWarnHideLvl__0011aa10(side), c, 1);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        r = TaLogic_GetTrapNearCard(order->position >> 8, order->position & 0xff, here & 0xff, here >> 8, cells, 3, 31, 0, 2,
            0, TaLogic_GetWarnHideLvl__0011aa10(side), c, 1);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
    }
    c = 0;
    b = 0;
    a = 0;
    got = 0;
    for (i = 1; i < 3; i++) {
        got = func_002770e0_cells(got, word, here & 0xff, here >> 8,
            func_0024de70_pair(order->position >> 8, order->position & 0xff), i & 0xff, 0, TaLogic_GetWarnHideLvl__0011aa10(side),
            cells, &c, &a, &b, buf);
        r = TaLogic_GetFollowNearUnit(here & 0xff, here >> 8, cells, 3, 34, 2, 2, c);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        r = TaLogic_GetFollowNearUnit(here & 0xff, here >> 8, cells, 3, 31, 1, 2, c);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
        r = TaLogic_GetFollowNearUnit(here & 0xff, here >> 8, cells, 3, 31, 0, 2, c);
        if (r >= 0) {
            order->flags.word = (r << 16) | order->flags.half;
            return;
        }
    }
    order->kind = 0x5ff;
}

#include "dotr/ee/position_rules.h"
void TaLogic020_CommandEfcMonster(struct DotrPositionOrder *order)
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
    c = 0;
    b = 0;
    a = 0;
    word = func_001df540_word(order->position >> 8, order->position & 0xff);
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(buf);
    got = 0;
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 297:
        for (i = 1; i <= 5; i++) {
            got = func_002770e0_cells(got, word, here & 0xff, here >> 8, side == 0 ? 1 : 0, i & 0xff, 1,
                TaLogic_GetWarnHideLvl__001676d0(side), cells, &c, &a, &b, buf);
            r = TaLogic_GetNearUnitL3(word, here & 0xff, here >> 8, cells, 5, 52, 0, 2, 1, TaLogic_GetWarnHideLvl__001676d0(side), c);
            if (r >= 0) {
                order->kind = 0x2ff;
                order->flags.word = (r << 16) | order->flags.half;
                return;
            }
        }
        break;
    case 79:
        if (func_001df470_side(side) < 2000) {
            order->kind = 0x2ff;
            order->flags.word = (here << 16) | order->flags.half;
        }
        break;
    }
    TaLogic020_CommandBtlUnit(order, 1);
}

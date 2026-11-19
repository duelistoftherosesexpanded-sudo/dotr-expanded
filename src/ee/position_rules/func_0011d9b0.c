#include "dotr/ee/position_rules.h"

void TaLogic_CommandEfcMonster__0011d9b0(struct DotrPositionOrder *order)
{
    unsigned char buf[112];
    struct DotrPositionCell cells[22];
    unsigned short a;
    unsigned short b;
    int side;
    int here;
    int word;
    int i;
    int r;

    if ((word = func_001df540_word(order->position >> 8, order->position & 0xff)) == 0) {
        return;
    }
    side = func_0024de70_pair(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    b = 0;
    a = 0;
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(buf);
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 496:
    case 510:
        for (i = 1; i <= 2; i++) {
            r = func_0027de20_order(order, 0, word, here & 0xff, here >> 8, side == 0 ? 1 : 0, i & 0xff, TaLogic_GetWarnHideLvl__0011fa80(side), cells, 0, &a, &b, 2, 0, 1500, buf);
            switch (r) {
            case 3:
            case 2:
                order->kind = 0x3ff;
                order->flags.word = (here << 16) | order->flags.half;
                return;
            case 1:
                return;
            case 0:
                order->kind = 0x3ff;
                order->flags.word = (here << 16) | order->flags.half;
                return;
            }
        }
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 14, 14)) > 3) {
            order->kind = 0x3ff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        }
        break;
    case 509:
        if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->kind = 0xbff;
            return;
        }
        break;
    }
    func_0011d110_order(order, 0);
}

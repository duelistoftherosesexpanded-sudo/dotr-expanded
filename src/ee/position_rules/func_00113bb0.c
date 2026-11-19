#include "dotr/ee/position_rules.h"

void TaLogic001_CommandEfcMonster(struct DotrPositionOrder *order, int deep)
{
    unsigned char ignoreList[112];
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
    func_0027e770_buf(ignoreList);
    got = 0;
    switch (SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff)) {
    case 83:
        if (TaLogic001_IsUseLordOfDragon(side) == 1) {
            order->kind = 0x3ff;
            order->flags.word = (here << 16) | order->flags.half;
        } else {
            order->kind = 0x5ff;
        }
        return;
    case 137:
        for (i = 1; i <= 5; i++) {
            got = func_002770e0_cells(got, word, here & 0xff, here >> 8, side == 0 ? 1 : 0, i & 0xff, 0,
                TaLogic_GetWarnHideLvl__00117c30(side), cells, &c, &a, &b, ignoreList);
            r = TaLogic_GetNearUnitL3(word, here & 0xff, here >> 8, cells, 5, 52, 0, 2, 0, TaLogic_GetWarnHideLvl__00117c30(side), c);
            if (r >= 0) {
                order->kind = 0x2ff;
                order->flags.word = (r << 16) | order->flags.half;
                return;
            }
        }
        break;
    case 191:
        if (TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->kind = 0xbff;
            return;
        }
        break;
    case 377:
        if (TaLogic001_IsUseLifeHourGlass(side) == 1 && TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->kind = 0xbff;
            return;
        }
        break;
    case 390:
        if (TaLogic001_IsUseBraveHourGlass(side) == 1 && TaLogic_CommandGunnerUnit(order, 0) == 1) {
            order->kind = 0xbff;
            return;
        }
        break;
    }
    func_00113760_spot(order, 1, deep);
}

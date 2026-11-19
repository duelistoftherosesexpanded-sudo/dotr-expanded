#include "dotr/ee/logic019_efc.h"

/* Inlined in the original: 1 when no other unit of the same controller stands in row 3
 * before this unit is met (columns 1-5), else 0. The helper and its name are a
 * reconstruction; the original source form is not known. */
static inline int lTaLogic019_IsLineOpen(int row, int column)
{
    int x;
    int id;
    for (x = 1; x <= 5; x++) {
        id = SzDuel_GetMapUnitID(x, 3);
        if (id != 0) {
            if (id == ((row << 8) | column)) return 1;
            if (func_00250360_pair(id >> 8, id & 0xff) == func_00250360_pair(row, column)) return 0;
        }
    }
    return 1;
}

void TaLogic019_CommandEfcMonster(struct DotrPositionOrder *order)
{
    int here;
    unsigned char buf[112];
    struct DotrLogic019Cell cells[22];
    unsigned short a;
    unsigned short b;
    unsigned short c;
    int side;
    int card;
    int word;
    int got;
    int i;
    int r;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    card = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff);
    word = func_001df540_word(order->position >> 8, order->position & 0xff);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    c = 0;
    b = 0;
    a = 0;
    for (i = 0; i < 22; i++) {
        cells[i].id = 0xffff;
        cells[i].kind = 0xff;
    }
    func_0027e770_buf(buf);
    got = 0;
    switch (card) {
    case 43:
        if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) == 0) {
            order->kind = 0xcff;
            order->flags.word = (here << 16) | order->flags.half;
            return;
        }
        break;
    case 87:
        TaLogic019_CommandAtkUnit(order);
        return;
    case 297:
        if (lTaLogic019_IsLineOpen(order->position >> 8, order->position & 0xff) == 1) {
            TaLogic019_CommandLineUnit(order, 1);
            return;
        }
        for (i = 1; i <= 2; i++) {
            got = func_002770e0_cells(got, word, here & 0xff, here >> 8, side == 0 ? 1 : 0, i & 0xff, 0,
                TaLogic_GetWarnHideLvl__00162580(side), cells, &c, &a, &b, buf);
            r = TaLogic_GetNearUnitL3(word, here & 0xff, here >> 8, cells, 2, 52, 0, 2, 0, TaLogic_GetWarnHideLvl__00162580(side), c);
            if (r >= 0) {
                order->kind = 0x2ff;
                order->flags.word = (r << 16) | order->flags.half;
                return;
            }
        }
        TaLogic019_CommandLDGuardUnit(order->position >> 8, order->position & 0xff, order);
        return;
    }
    TaLogic019_CommandBtlUnit(order);
}

#include "dotr/ee/position_rules.h"

void TaLogic017_CommandEfcMonster(struct DotrPositionOrder *order)
{
    int side;
    int i;
    int kind;
    int list;
    int word;
    unsigned char count;

    side = func_00250360_pair(order->position >> 8, order->position & 0xff);
    kind = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xff);
    count = 0;
    switch (kind) {
    case 56:
    case 57:
    case 54:
    case 55:
        order->kind = 0x6ff;
        order->flags.word = order->flags.half | (SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff) << 16);
        return;
    case 64:
        list = SearchUnitByRange(0, 0, side, 2, 3, 14);
        for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(list); i++) {
            word = Ta_GetUnitOwnerIDFromLstCount(list, i);
            if (TaLogic_IsJudgeUnitDanger(word >> 8, word & 0xff, 0) == 1) {
                count++;
            }
        }
        if (count > 1) {
            TaLogic017_CommandBtlUnit(order);
            order->kind = 0x3ff;
            return;
        }
        break;
    case 95:
        if (func_001df470_side(side) < 1200) {
            TaLogic017_CommandBtlUnit(order);
            order->kind = 0x3ff;
            return;
        }
        break;
    }
    TaLogic017_CommandBtlUnit(order);
}


#include "dotr/ee/lower_rules.h"

int TaLogic_CommandUnit__001547a0(struct DotrRuleOrder *order)
{
    int key;
    int cell;
    int kind;
    int word;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF);
    key = func_00250360_pair(order->position >> 8, order->position & 0xFF);
    if (func_001df540_word(order->position >> 8, order->position & 0xFF) != 0
        && SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xFF, 0) == 0 && order->position != 0xFFFF) {
        if ((order->position & 0xFF) == 0) {
            if (order->flags.word == -1) {
                if (TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xFF, 0) == 1) {
                    if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                        order->missions = 0x50A;
                    } else {
                        order->missions = 0x5FF;
                    }
                } else {
                    if ((cell = SzDuel_GetMapUnitID(3, 5)) < 0) {
                        if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                            order->missions = 0xA06;
                            order->flags.word = (order->flags.word & 0xFFFF0000) | 0x403;
                        } else {
                            order->missions = 0x6FF;
                            order->flags.word = order->flags.half | 0x4030000;
                        }
                    } else if (SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xFF) == 0x403
                        && func_00250360_pair(cell >> 8, cell & 0xFF) == key && cell != order->position) {
                        if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                            order->missions = 0x20A;
                            order->flags.word = (cell << 16) | order->flags.half;
                        } else {
                            order->missions = 0x2FF;
                            order->flags.word = (cell << 16) | order->flags.half;
                        }
                    } else {
                        if (SzDuel_InqHandPhase(order->position >> 8) == 1) {
                            order->missions = 0xA06;
                            order->flags.word = (order->flags.word & 0xFFFF0000) | 0x403;
                        } else {
                            order->missions = 0x6FF;
                            order->flags.word = order->flags.half | 0x4030000;
                        }
                    }
                }
            }
        } else {
            kind = SzDuel_GetUnitKind(order->position >> 8, order->position & 0xFF);
            word = SzDuel_GetUnitCardNo(order->position >> 8, order->position & 0xFF);
            switch (kind >> 5) {
            case 0:
                if (func_002507d0_pair(order->position >> 8, order->position & 0xFF) == 1 || TaLogic017_IsExzParts(word) == 1) {
                    TaLogic017_CommandEfcMonster(order);
                } else {
                    TaLogic017_CommandBtlUnit(order);
                }
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                TaLogic017_CommandSpellUnit(order);
                break;
            default:
                order->missions = 0x5FF;
                break;
            }
        }
        return 1;
    }
    return 0;
}

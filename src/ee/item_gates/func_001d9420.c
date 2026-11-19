#include "dotr/ee/item_gates.h"

int lSzDuel_IsUnitHide(struct DotrStateItem *unit)
{
    struct DotrStateControl *c = data_00317490_control;
    int result = 0;
    struct DotrStateCell *cell;
    int flag;
    int side;
    int other;
    int t;
    unsigned char raw;

    if (unit != 0) {
        cell = SzDuel_GetMapInfo(unit->locCol, unit->locRow);
        t = SzGame_GetMenuNum(29);
        flag = (t & (1 << unit->team)) ? 1 : 0;
        raw = c->unknown_d3c;
        side = raw;
        other = raw ^ 1;
        if (unit->team >= 2) {
            return 0;
        }
        if (cell == 0 || (struct DotrStateItem *)cell->unit != unit) {
            if (flag) {
                if (SzDuel_GetPlayer(side) < 32 && SzDuel_GetPlayer(other) >= 100) {
                    result = 1;
                }
            } else {
                if ((SzPad_Check(raw, 1, 2) & 0xffff) == 0) {
                    result = 1;
                }
            }
        } else {
            result = Ta_IsUnitHide(unit, raw);
            if (result == 0) {
                if (!unit->status.unknown_bit_4) {
                    t = func_00250360_pair(unit->team, unit->slot);
                    if (flag) {
                        if (SzDuel_GetPlayer(side) < 32 && SzDuel_GetPlayer(other) >= 100 && SzDuel_GetPlayer(t) < 32) {
                            result = 1;
                        }
                    } else {
                        if (c->unknown_d3c == t) {
                            if ((SzPad_Check(c->unknown_d3c, 1, 2) & 0xffff) == 0) {
                                result = 1;
                            }
                        } else {
                            result = 1;
                        }
                    }
                }
            }
        }
    }
    return result;
}

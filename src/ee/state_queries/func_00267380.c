#include "dotr/ee/state_queries.h"

int Ta_IsUnitHide(struct DotrStateItem *unit, int side)
{
    int pos;
    int owner;
    int i;
    int n;
    struct DotrStateEntry8 *w;
    int mine;
    int flag;

    if (unit == 0) {
        return 0;
    }
    flag = unit->status.unknown_bit_1;
    if (flag == 0) {
        mine = unit->team;
    } else {
        mine = unit->team == 0 ? 1 : 0;
    }
    if (side == mine) {
        return 0;
    }
    if (unit->slot == 0) {
        return 0;
    }
    if (!unit->status.unknown_bit_2) {
        return 1;
    }
    if ((n = Ta_SearchUncTypeNaturalEffectUserNum(68)) > 0) {
        for (i = 0; i < n; i++) {
            pos = Ta_SearchUncTypeNaturalEffectUser(68, i);
            if (SzDuel_GetUnitStatus(pos >> 8, pos & 0xff, 4) != 0 && SzDuel_GetUnitStatus(pos >> 8, pos & 0xff, 3) != 0 && SzDuel_GetUnitStatus(pos >> 8, pos & 0xff, 5) != 0) {
                owner = func_00250360_pair(pos >> 8, pos & 0xff);
                if (owner == func_00250370_item(unit) && (w = Ta_GetUnitEffect(pos >> 8, pos & 0xff, 2)) != 0) {
                    switch (w->unknown_words[0] & 0x3f) {
                    case 29:
                        return (unit->kind >> 5) == 0 ? 1 : 0;
                    case 30:
                        return (unit->kind >> 5) != 0 ? 1 : 0;
                    }
                }
            }
        }
    }
    return 0;
}

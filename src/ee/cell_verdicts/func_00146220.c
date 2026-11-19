#include "dotr/ee/cell_verdicts.h"
int TaLogic_IsOpenUnit__00146220(int x, int y)
{
    int r;
    int kind;
    int side;
    unsigned char count;
    int i;
    int set;
    int cell;
    r = SzDuel_GetUnitLoc(x, y);
    kind = SzDuel_GetUnitCardNo(x, y);
    side = func_00250360_pair(x, y);
    count = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (kind) {
    case 325: /* The Drdek */
        set = SearchUnitByRange(0, 0, x, 2, 52, 14);
        for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(set); i++) {
            cell = Ta_GetUnitOwnerIDFromLstCount(set, i);
            if (SzDuel_GetUnitStatus(cell >> 8, cell & 0xff, 3)) {
                count++;
            }
        }
        if (count >= 3) {
            return 1;
        }
        break;
    case 192: /* Beautiful Beast Trainer */
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, x, 1, 5, 14)) >= 3) {
            return 1;
        }
        break;
    case 731: /* Fairy Meteor Crush */
    case 751: /* Creature Swap */
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic012_IsPinch(side) == 1) {
            return 1;
        }
        break;
    case 846: /* Resurrection of Chakra */
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic012_IsDoCerem(func_00250360_pair(x, y), 846) == 1) {
            if (SzDuel_GetField(r & 0xff, r >> 8) != 9) {
                return 1;
            }
        }
        break;
    }
    return 2;
}

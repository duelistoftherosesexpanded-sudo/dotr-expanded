#include "dotr/ee/cell_verdicts.h"
/* File-local helper: a spelling that reproduces the target's repeated
 * 0x14c9d0 call and kept result, not a claim about an original function. */
static inline int spot_find(void)
{
    int i;
    int spot;

    for (i = 0; i < 3; i++) {
        spot = TaLogic014_GetLine(i);
        if ((SzDuel_GetMapUnit(spot & 0xff, spot >> 8) == 0 ? 1 : 0) == 1) {
            return TaLogic014_GetLine(i);
        }
    }
    return -1;
}
int TaLogic_IsOpenUnit__0014f490(int x, int y)
{
    int kind;
    int pos;
    int side;
    unsigned *p;
    int r;
    kind = SzDuel_GetUnitCardNo(x, y);
    pos = SzDuel_GetUnitLoc(x, y);
    side = func_00250360_pair(x, y);
    p = SzDuel_GetUnitEffect(x, y, 0);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 612: /* Maiden of the Aqua */
        case 551: /* Jellyfish */
            if (TaLogic014_IsUnitNaturalEffectUse(side, kind) == 1) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (TaLogic014_IsDoCerem(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (p == 0) {
            return 2;
        }
        switch (*p >> 8) {
        case 13: /* Alter LP (Heal/Damage) */
            return 1;
        case 2: /* Terrain Change */
            r = TaLogic_GetLandChange(func_001df540_word(x, y), pos & 0xFF, pos >> 8, 4, 9, 2);
            if (r >= 0 && r == pos) {
                return 1;
            }
            break;
        case 4: /* Destruction */
            if ((spot_find() < 0 ? 1 : 0) == 1) {
                if (SzDuel_GetUnitSpellCount(x) == 0) {
                    return 1;
                }
            }
            break;
        case 18: /* AP/DP Buff */
            if (SzDuel_GetUnitMonsterCount(x) >= 4) {
                return 1;
            }
            break;
        }
        break;
    default:
        return 2;
    }
    return 2;
}

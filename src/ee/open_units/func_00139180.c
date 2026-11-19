#include "dotr/ee/open_units.h"
/* The sacrifice test of logic 009 (bound 2600), expanded in place by the compiler; the
 * out-of-line TaLogic009_IsDoCerem at 0x138f90 is assumed to be the same code (not compared). */
static inline int IsDoCerem_inline(int value, int index)
{
    int x;
    int r;
    int triple[3];
    int cell;
    int y;
    int i;
    int word;

    Ta_GetCeremSacrifice(triple, TaHand_GetCardEffect(index, 0)->unknown_4);
    if ((word = Ta_GetSacrificeUnit(triple, value)) < 0) {
        return 0;
    }
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        cell = Ta_GetUnitOwnerIDFromLstCount(word, i);
        x = cell >> 8;
        y = cell & 0xff;
        r = SzDuel_GetUnitLoc(x, y);
        if (TaUtil_GetUnitAP(x, y, r & 0xff, r >> 8) >= 2600) {
            return 0;
        }
    }
    return 1;
}
/* Effect 23 condition, expanded in place; no original name is known for it. */
static inline int Effect23_inline(int side)
{
    int own;
    int x;
    int y;
    if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 2, 0, 14)) >= 2) {
        return 1;
    }
    own = Ta_GetUnitOwnerIDFromLstCount(SearchUnitByRange(0, 0, side, 3, 33, 14), 0);
    x = own >> 8;
    y = own & 0xff;
    if (side != func_00250360_pair(x, y)) {
        if ((SzDuel_GetUnitKind(x, y) >> 5) == 0 && (SzDuel_GetUnitKind(x, y) & 0x1f) == 0) {
            return 1;
        }
    }
    return 0;
}
int TaLogic_IsOpenUnit__00139180(int x, int y)
{
    int i;
    int kind;
    unsigned *effect;
    int pos;
    unsigned char cnt;
    kind = SzDuel_GetUnitCardNo(x, y);
    func_00250360_pair(x, y);
    effect = SzDuel_GetUnitEffect(x, y, 0);
    pos = SzDuel_GetUnitLoc(x, y);
    cnt = 0;
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0:
        switch (kind) {
        case 613:
            for (i = 1; i <= 11; i++) {
                if (SzDuel_GetUnitCurseRest(x, i) > 0 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCurseRest(x == 0 ? 1 : 0, i) > 0 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
                return 1;
            }
            break;
        }
        return 2;
    case 5:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 1) == 5) {
            return 2;
        }
        if (IsDoCerem_inline(func_00250360_pair(x, y), kind) == 1) {
            if (SzDuel_GetField(pos & 0xFF, pos >> 8) != 9) {
                return 1;
            }
        }
        break;
    case 1:
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 1) == 5) {
            return 2;
        }
        if (effect == 0) {
            return 2;
        }
        switch (*effect >> 8) {
        case 2: /* terrain change */
            i = TaLogic_GetLandChange(func_001df540_word(x, y), pos & 0xff, pos >> 8, 6, 5, 2);
            if (i >= 0 && (i >> 8) < 4 && pos == i) {
                return 1;
            }
            break;
        case 13: /* alter LP */
            return 1;
        case 23: /* spellbind */
            if (Effect23_inline(x) == 1) {
                return 1;
            }
            break;
        case 31: /* revolt/steal */
            if (kind == 733) {
                i = TaLogic009_GetRevoltObj(x);
                if (i >= 0 && (i & 0xffff) != 0xffff) {
                    return 1;
                }
            }
            break;
        case 32: /* magic-neutralizing field */
            for (i = 1; i <= 11; i++) {
                if (SzDuel_GetUnitCurseRest(x, i) != 0 && x == func_00250360_pair(x, i)) {
                    cnt++;
                }
                if (SzDuel_GetUnitCurseRest(x == 0 ? 1 : 0, i) != 0 && x == func_00250360_pair(x == 0 ? 1 : 0, i)) {
                    cnt++;
                }
            }
            if (cnt >= 2) {
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

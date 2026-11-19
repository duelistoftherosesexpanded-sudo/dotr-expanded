#include "dotr/ee/open_unit_reply.h"

int TaLogic_IsOpenUnit__00153ae0(int x, int y)
{
    unsigned short words[24];
    int r;
    int pb;
    int who;
    int kind;
    unsigned *effect;
    int ay;
    int ax;
    int by;
    int bx;
    int cy;
    int cx;
    kind = SzDuel_GetUnitCardNo(x, y);
    SzDuel_GetUnitLoc(x, y);
    who = Ta_IsUnitControl(x, y);
    effect = SzDuel_GetUnitEffect(x, y, 0);
    if (SzDuel_GetUnitStatus(x, y, 4)) {
        return 0;
    }
    TaUtil_GetUnitLst(who, 2, words);
    switch (SzDuel_GetUnitKind(x, y) >> 5) {
    case 0: /* monster */
        switch (kind) {
        case 36: /* Time Wizard */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, who, 1, 0x95F0037, 14)) >= 2) {
                return 1;
            }
            break;
        case 213: /* Mountain Warrior */
            if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, who, 1, 29, 22)) >= 2) {
                return 1;
            }
            break;
        }
        return 2;
    case 1: /* magic */
        if (TaLogic_GetActionReaction(x, y, 0, 0, 6, 0) == 5) {
            return 2;
        }
        if (effect == 0) {
            return 2;
        }
        switch (*effect >> 8) {
        case 13: /* effect kind 13: alter LP */
            return 1;
        case 33: /* effect kind 33: swap attack and defence */
            TaLogic_GetUnitCtlLst2(who == 0 ? 1 : 0, words, 29, 0, 0);
            r = TaLogic_GetMaxApUnit(words);
            ax = r >> 8;
            ay = r & 0xFF;
            kind = SzDuel_GetUnitLoc(ax, ay);
            TaLogic_GetUnitCtlLst(who, words, 29, 1);
            r = TaLogic_GetMaxApUnit(words);
            bx = r >> 8;
            by = r & 0xFF;
            pb = SzDuel_GetUnitLoc(bx, by);
            TaUtil_GetUnitLst(who, 1, words);
            cx = kind & 0xFF;
            cy = kind >> 8;
            if (TaUtil_GetUnitAP(ax, ay, cx, cy) >= 2000) {
                r = TaUtil_GetUnitAP(ax, ay, cx, cy);
                if (TaUtil_GetUnitAP(bx, by, pb & 0xFF, pb >> 8) < r) {
                    return 1;
                }
            }
            break;
        }
        break;
    default:
        return 2;
    }
    return 2;
}

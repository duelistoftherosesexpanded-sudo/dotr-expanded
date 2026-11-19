#include "dotr/ee/kind_applies.h"

/* MATCHING DEVICE: the body is an inlined helper with `return`s, which reproduces the
 * original delay slots. Not a claim about the original source layout. */
static inline int apply_kind_00248460(int side, int pos, int c, int teamId, int total, int id)
{
    int val;
    unsigned *p;

    {
        val = gAnmPat_Bakura[id];
        switch (EnchantId[id]) {
        case 0: /* normal equip */
            if (val == 0) {
                id = teamId & 0xff;
                total = Ta_GetForceEffection(id, total);
                if (c >> 16) {
                    TaUtil_CalcUnitAP(side, pos, 1, total);
                    TaUtil_CalcUnitDP(side, pos, 1, total);
                } else {
                    TaUtil_CalcUnitAP(side, pos, 0, total);
                    TaUtil_CalcUnitDP(side, pos, 0, total);
                }
                SzDuel_CalcLP(id == 0 ? 1 : 0, 1, total);
                return c;
            } else {
                total = val + total;
                if (c >> 16) {
                    TaUtil_CalcUnitAP(side, pos, 1, total);
                    TaUtil_CalcUnitDP(side, pos, 1, total);
                    return c;
                } else {
                    TaUtil_CalcUnitAP(side, pos, 0, total);
                    TaUtil_CalcUnitDP(side, pos, 0, total);
                    return c;
                }
            }
        case 1: /* gives the monster an effect */
            switch (val) {
            case 1:
                p = SzDuel_GetUnitEffect(side, pos, 4);
                p[0] = 5211;
                p[1] = 0;
                p = SzDuel_GetUnitEffect(side, pos, 3);
                p[0] = 14428;
                p[1] = 0x1f0129;
                return c;
            case 2:
                p = SzDuel_GetUnitEffect(side, pos, 0);
                p[0] = 1152;
                p[1] = 0;
                return c;
            case 3:
                p = SzDuel_GetUnitEffect(side, pos, 0);
                p[0] = 1159;
                p[1] = 0;
                return c;
            case 4:
                p = SzDuel_GetUnitEffect(side, pos, 0);
                p[0] = 1153;
                p[1] = 0;
                return c;
            case 5:
                p = SzDuel_GetUnitEffect(side, pos, 4);
                p[0] = 765;
                p[1] = 0x10009;
                return c;
            }
            break;
        case 2: /* Cursebreaker */
            TaUtil_CalcUnitAP(side, pos, 4, 0);
            TaUtil_CalcUnitDP(side, pos, 4, 0);
            Ta_SetUnitCurseRest(side, pos, 0);
            SzDuel_SetUnitSpellRest(side, pos, 0);
            return c;
        case 3: /* Paralyzing Potion */
            Ta_SetUnitCurseRest(side, pos, 255);
            SzDuel_SetUnitStatus(side, pos, 0, 1);
            return c;
        case 4: /* fusion equips */
            return c;
        }
    }
    return c;
}

int Enchant(int side, int pos, int c, int teamId)
{
    int total;
    int id;

    SzDuel_GetUnitCardNo(side, pos);
    total = 0;
    total += Ta_GetEnchantOfs();
    id = c & 0x3fff;
    if (Do_ChkEncMatch(side, pos, id) != 0) {
        c = apply_kind_00248460(side, pos, c, teamId, total, id);
    } else {
        c = -1;
    }
    return c;
}


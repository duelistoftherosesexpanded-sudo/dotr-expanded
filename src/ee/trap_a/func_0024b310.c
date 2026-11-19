#include "dotr/ee/trap_a.h"

/* MATCHING DEVICE: the other side is spelled `s == 0 ? 1 : 0`; `s ? 0 : 1` compiles differently. */
#define OPP(s) ((s) == 0 ? 1 : 0)
/* MATCHING DEVICE: the operand order of both comparisons. */
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define MIN(a, b) ((b) < (a) ? (b) : (a))

/* MATCHING DEVICE: every switch case that fires is its own copy of this body (the original
 * has thirteen copies of the code); `list` is indexed plainly, not through a pointer. */
#define BODY(no, par) \
    if (Ta_IsLDRangeAvilityWithLocation(nSide, nID, nX, nZ, 11)) { \
        Ta_SetRangeVisual(loc & 0xff, loc >> 8, 0, Ta_IsLDAbl(9, side), 1, 1, 11); \
        eInfo->eNO = 254; \
        eInfo->dwFrom = 0; \
        eInfo->dwTo = side << 8; \
        eInfo->dwPar = 16; \
        return 1; \
    } \
    if (Ta_IsResistAll(nSide, nID)) { \
        return 0; \
    } \
    eInfo->eNO = no; \
    eInfo->dwFrom = list[i]; \
    eInfo->dwTo = (nSide << 8) | nID; \
    eInfo->dwPar = par; \
    return 257;

int Chk_TrapA(int nSide, int nID, int nX, int nZ, struct jp_7b52 *eInfo, int PrNo)
{
    /* MATCHING DEVICE: this declaration order decides the registers. */
    int list[5];
    int side;
    int f1;
    int f2;
    unsigned *p;
    int cnt;
    int y;
    int id;
    int lo;
    int hi;
    int x;
    int r;
    int loc;
    unsigned *w;
    int i;

    side = Ta_GetCurrentSide(nSide, nID);
    f1 = 0;
    f2 = 0;
    if (nID == 0) {
        return 0;
    }
    if ((PrNo & 0xffff) == 0) {
        if (Ta_IsLDRange(OPP(side), nX, nZ)) {
            if (!(SzDuel_GetUnitKind(nSide, nID) >> 5)) {
                if (Ta_IsLDAbl(16, OPP(side)) && Ta_IsLD_DestObj(OPP(side), SzDuel_GetUnitKind(nSide, nID) & 0x1f)) {
                    f1 = ConvUnitIDtoFlagList(0, nSide, nID);
                }
                if (Ta_IsLDAbl(15, OPP(side)) && Ta_IsLD_CurseObj(OPP(side), SzDuel_GetUnitKind(nSide, nID) & 0x1f)) {
                    f2 = ConvUnitIDtoFlagList(0, nSide, nID);
                }
                if (f1 > 0 || f2 > 0) {
                    eInfo->eNO = 113;
                    eInfo->dwFrom = f1;
                    eInfo->dwTo = f2;
                    eInfo->dwPar = 0x80000000;
                    return 257;
                }
            }
            f1 = 0;
            if (Ta_IsLDAbl(13, OPP(side)) && !SzDuel_GetUnitStatus(nSide, nID, 2) && !SzDuel_GetUnitStatus(nSide, nID, 4)) {
                f1 = ConvUnitIDtoFlagList(0, nSide, nID);
            }
            if (f1 > 0) {
                eInfo->eNO = 113;
                eInfo->dwFrom = 0;
                eInfo->dwTo = 0;
                eInfo->dwPar = f1 | 0x80000000;
                return 0x10201;
            }
        }
    }
    p = func_00250740_trapa(nSide, nID, 1);
    if (p && (p[0] >> 8) == 7) {
        return 0;
    }
    if (Ta_SearchUncTypeNaturalEffectUserNum(15) > 0) {
        return 0;
    }
    cnt = 0;
    for (i = 0; i < 5; i++) {
        list[i] = -1;
    }
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            id = SzDuel_GetMapUnitID(x, y);
            if (id < 0) {
                continue;
            }
            hi = id >> 8;
            lo = id & 0xff;
            if (side == Ta_GetCurrentSide(hi, lo)) {
                continue;
            }
            if ((MAX(x, nX) - MIN(x, nX)) + (MAX(y, nZ) - MIN(y, nZ)) >= 2) {
                continue;
            }
            if ((SzDuel_GetUnitKind(hi, lo) >> 5) != 3) {
                continue;
            }
            if (Ta_GetCurrentSide(nSide, nID) == Ta_GetCurrentSide(hi, lo)) {
                continue;
            }
            /* MATCHING DEVICE: the bit test is written out inside the loop. */
            if (((PrNo >> 16) & 1) == 1 && !SzDuel_GetUnitStatus(hi, lo, 4)) {
                continue;
            }
            list[cnt] = id;
            cnt++;
        }
    }
    if (cnt == 0) {
        return 0;
    }
    r = Ta_IsActiveTrapB(0, 46);
    if (r >= 0) {
        eInfo->eNO = 102;
        eInfo->dwFrom = 1;
        eInfo->dwTo = r;
        eInfo->dwPar = 0;
        return 1;
    }
    SortByOrder(list, 0, GetCap(0, cnt - 1));
    loc = SzDuel_GetUnitLoc(side, 0);
    for (i = 0; i < cnt; i++) {
        w = func_00250740_trapa(list[i] >> 8, list[i] & 0xff, 0);
        switch (SzDuel_GetUnitKindOfs(list[i] >> 8, list[i] & 0xff)) {
        case 0:
            BODY(44, w[1])
        case 1:
            BODY(44, w[1])
        case 2:
            BODY(44, w[1])
        case 3:
            BODY(44, w[1])
        case 4:
            BODY(44, w[1])
        case 5:
            BODY(44, w[1])
        case 6:
            if (!(SzDuel_GetUnitKind(nSide, nID) >> 5) && TaUtil_GetUnitAP(nSide, nID, nX, nZ) < 1001) {
                BODY(45, 0)
            }
            break;
        case 7:
            if (!(SzDuel_GetUnitKind(nSide, nID) >> 5) && TaUtil_GetUnitAP(nSide, nID, nX, nZ) < 1501) {
                BODY(45, 0)
            }
            break;
        case 8:
            if (!(SzDuel_GetUnitKind(nSide, nID) >> 5) && TaUtil_GetUnitAP(nSide, nID, nX, nZ) < 2001) {
                BODY(45, 0)
            }
            break;
        case 9:
            if (!(SzDuel_GetUnitKind(nSide, nID) >> 5) && TaUtil_GetUnitAP(nSide, nID, nX, nZ) < 3001) {
                BODY(45, 0)
            }
            break;
        case 10:
            if (!(SzDuel_GetUnitKind(nSide, nID) >> 5) && TaUtil_GetUnitAP(nSide, nID, nX, nZ) < 4001) {
                BODY(45, 0)
            }
            break;
        case 11:
            BODY(46, 0)
        case 12:
            if (SzDuel_GetUnitKind(nSide, nID) >> 5) {
                BODY(45, 0)
            }
            break;
        }
    }
    return 0;
}

#include "dotr/ee/driver_scans.h"

#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define ID(w) (w) >> 8, (w) & 0xFF

/* v1, pInfo, Dist, Loc and i are the Japanese build's local names; wUnit, bMin and p are not in
 * its debug information and are named here. */
int TaLogic_GetFollowNearUnit(int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase, int sKey, unsigned char fOpen, unsigned char fDef, unsigned short wUnitNum)
{
    int i;
    int v1;
    int Loc;
    int Dist;
    struct jp_9aeb0 *pInfo;
    unsigned short wUnit;
    unsigned char bMin;
    struct jp_9b50e *p;

    p = nUnitLst;
    bMin = 7;
    wUnit = 0xFFFF;
    for (i = 0; i < wUnitNum; i++) {
        if (SzDuel_GetUnit(ID(p->nUnitID)) != 0) {
            Loc = SzDuel_GetUnitLoc(ID(p->nUnitID));
            if (TaLogic_IsStatusMatch(SzDuel_GetUnitStatus(ID(p->nUnitID), 2), fOpen) == 1
                && TaLogic_IsStatusMatch(SzDuel_GetUnitStatus(ID(p->nUnitID), 5), fDef) == 1
                && TaLogic_IsUnitCategory(ID(p->nUnitID), sKey) == 1) {
                Dist = p->bDist;
                if (Dist <= bChase && Dist > 0) {
                    if (Dist < bMin) {
                        bMin = (MAX(Loc & 0xFF, nX) - MIN(Loc & 0xFF, nX)) + (MAX(Loc >> 8, nZ) - MIN(Loc >> 8, nZ));
                        wUnit = p->nUnitID;
                        bChase = Dist;
                    } else if (Dist == bMin) {
                        pInfo = SzDuel_GetCardConstInfo(SzDuel_GetUnitCardNo(ID(p->nUnitID)));
                        v1 = pInfo->bDeckCost;
                        pInfo = SzDuel_GetCardConstInfo(SzDuel_GetUnitCardNo(ID(wUnit)));
                        if (pInfo->bDeckCost < v1) {
                            wUnit = p->nUnitID;
                            bMin = (MAX(Loc & 0xFF, nX) - MIN(Loc & 0xFF, nX)) + (MAX(Loc >> 8, nZ) - MIN(Loc >> 8, nZ));
                            bChase = Dist;
                        }
                    }
                }
            }
        }
        p++;
    }
    if (wUnit == 0xFFFF) {
        return -1;
    }
    return wUnit;
}

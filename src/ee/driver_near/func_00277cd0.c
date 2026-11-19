#include "dotr/ee/driver_near.h"

#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#define ID(w) (w) >> 8, (w) & 0xFF

/* v1, Dist, Loc and i are the Japanese build's local names; v2, bMin, wUnit, wLoc and p are not in
 * its debug information and are named here. */
int TaLogic_GetTrapNearCard(int tSide, int tID, int nX, int nZ, struct jp_9b50e *nUnitLst, unsigned char bChase, int sKey, unsigned char fOpen, unsigned char fDef, unsigned char fView, int bWarnLvl, unsigned short wUnitNum, unsigned char fTrap)
{
    int i;
    int Loc;
    int Dist;
    int v1;
    int v2;
    unsigned char bMin;
    unsigned short wUnit;
    unsigned short wLoc;
    struct jp_9b50e *p;

    p = nUnitLst;
    bMin = 7;
    wUnit = 0xFFFF;
    wLoc = wUnit;
    for (i = 0; i < wUnitNum; i++) {
        if (SzDuel_GetUnit(ID(p->nUnitID)) != 0) {
            Loc = SzDuel_GetUnitLoc(ID(p->nUnitID));
            if ((MAX(Loc & 0xFF, nX) - MIN(Loc & 0xFF, nX)) + (MAX(Loc >> 8, nZ) - MIN(Loc >> 8, nZ)) <= bChase
                && TaLogic_IsStatusMatch(SzDuel_GetUnitStatus(ID(p->nUnitID), 2), fOpen) == 1
                && TaLogic_IsStatusMatch(SzDuel_GetUnitStatus(ID(p->nUnitID), 5), fDef) == 1
                && TaLogic_IsUnitCategory(ID(p->nUnitID), sKey) == 1
                && ((TaLogic_IsJudgeUnitDanger(ID(p->nUnitID), fView) == 1
                     && TaTrapA_IsTrapAct(tSide, tID, ID(p->nUnitID), Loc & 0xFF, Loc >> 8) == 1)
                    || (TaLogic_IsJudgeUnitDanger(ID(p->nUnitID), fView) == 1 && fTrap == 1))) {
                Dist = p->bDist;
                if (Dist <= bChase && Dist > 0) {
                    if (Dist < bMin) {
                        bMin = (MAX(Loc & 0xFF, nX) - MIN(Loc & 0xFF, nX)) + (MAX(Loc >> 8, nZ) - MIN(Loc >> 8, nZ));
                        wUnit = p->nUnitID;
                        wLoc = Loc;
                        bChase = Dist;
                    } else if (Dist == bMin) {
                        if (TaLogic_IsJudgeUnitDanger(ID(p->nUnitID), fView) == 1) {
                            v1 = TaLogic_IsUnitWin2(SzDuel_GetUnit(tSide, tID), p->nUnitID, Loc & 0xFF, Loc >> 8);
                        } else {
                            v1 = bWarnLvl;
                        }
                        if (TaLogic_IsJudgeUnitDanger(ID(wUnit), fView) == 1) {
                            v2 = TaLogic_IsUnitWin2(SzDuel_GetUnit(tSide, tID), wUnit, wLoc & 0xFF, wLoc >> 8);
                        } else {
                            v2 = bWarnLvl;
                        }
                        if (v1 < v2) {
                        wUnit = p->nUnitID;
                        bMin = (MAX(Loc & 0xFF, nX) - MIN(Loc & 0xFF, nX)) + (MAX(Loc >> 8, nZ) - MIN(Loc >> 8, nZ));
                        wLoc = Loc;
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

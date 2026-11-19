#include "dotr/ee/driver_scans.h"

int TaLogic_GetNearCard2(struct jp_9a533 *pVarInfo, int nX, int nZ, int nSide, unsigned char bChase, int sKind, int fOpen, int fDef, int fView, int bWarnLvl)
{
    unsigned short wIgnoreLst[49];
    struct jp_9b50e nUnitLst[22];
    unsigned short wSize;
    unsigned short wNodeCount;
    unsigned short wUnitNum;
    struct PieceTreetag *pTree;
    int i;
    int Unit;

    if (SzGame_GetHSyncCount() > 12000) {
        return -1;
    }
    for (i = 0; i < 22; i++) {
        nUnitLst[i].nUnitID = 0xFFFF;
        nUnitLst[i].bDist = 255;
    }
    TaLogic_InitIgnoreLst(wIgnoreLst);
    wUnitNum = 0;
    wNodeCount = 0;
    wSize = 0;
    pTree = 0;
    for (i = 1; i <= bChase; i++) {
        pTree = TaLogic_GetNearUnitL2(pTree, pVarInfo, nX, nZ, nSide, i, fView, bWarnLvl, nUnitLst, &wUnitNum, &wSize, &wNodeCount, wIgnoreLst);
        Unit = TaLogic_GetNearUnitL3(pVarInfo, nX, nZ, nUnitLst, bChase, sKind, fOpen, fDef, fView, bWarnLvl, wUnitNum);
        if (Unit >= 0) {
            return Unit;
        }
        if (SzGame_GetHSyncCount() > 12000) {
            return -1;
        }
    }
    return -1;
}

#include "dotr/ee/driver_tree.h"

struct PieceTreetag *TaLogic_GetNearUnitPickUpTree(struct PieceTreetag *pTree, struct PieceTreetag *pPrev,
    struct jp_9a533 *pVarInfo, int nX, int nZ, int nSide, unsigned char bDist, int fView, unsigned short bWarnLvl,
    unsigned char bDepth, unsigned short *wSize, unsigned short *wNodeCount, struct jp_9b50e *nUnitLst,
    unsigned short *wUnitNum, unsigned short *wIgnoreLst)
{
    unsigned int dwMoveRev;
    int k;
    int ZLoopStart;
    unsigned char Node0;
    int XLoopEnd;
    int ZLoopEnd;
    int Unit;
    int XLoopStart;
    int j;
    int i;

    dwMoveRev = GetMoveRev(pVarInfo, nX, nZ);
    if (pVarInfo == 0) {
        return 0;
    }
    if (SzGame_GetHSyncCount() > 12000) {
        return 0;
    }
    if (pTree == 0) {
        if (bDepth > bDist) {
            return 0;
        }
        if ((pTree = TaLogic_RootTreeAloc(wSize, wNodeCount)) == 0) {
            return 0;
        }
        TaLogic_SetRootTreeDat(pTree, pPrev, nX, nZ);
        if (SzDuel_GetMapUnit(nX, nZ) != 0 && nSide == Ta_IsUnitControl2(SzDuel_GetMapUnit(nX, nZ))) {
            Unit = SzDuel_GetMapUnitID(nX, nZ);
            if (!TaLogic_IsAdjustUnitLst(nUnitLst, Unit, bDepth) && *wUnitNum < 22) {
                nUnitLst[*wUnitNum].nUnitID = SzDuel_GetMapUnitID(nX, nZ);
                nUnitLst[*wUnitNum].bDist = bDepth;
                (*wUnitNum)++;
            }
            if (TaLogic_IsUnitWin4(pVarInfo, Unit, bWarnLvl, nX, nZ, fView) < 0) {
                return pTree;
            }
        }
    }
    Unit = dwMoveRev & 0xF;
    k = 0;
    XLoopStart = GetCap(0, nX - (unsigned int)Unit);
    XLoopEnd = nX + (unsigned int)Unit;
    /* MATCHING DEVICE: both caps are conditional expressions; as `if` statements the first one
     * is scheduled differently (the next subtraction moves into the branch delay slot). */
    XLoopEnd = (unsigned int)XLoopEnd >= 7 ? 6 : XLoopEnd;
    ZLoopStart = GetCap(0, nZ - (unsigned int)Unit);
    ZLoopEnd = nZ + (unsigned int)Unit;
    ZLoopEnd = (unsigned int)ZLoopEnd >= 7 ? 6 : ZLoopEnd;
    Node0 = bDepth == 0 ? 1 : 0;
    for (i = XLoopStart; i <= XLoopEnd; i++) {
        for (j = ZLoopStart; j <= ZLoopEnd; j++) {
            if (i >= 0 && j >= 0 && i < 7 && j < 7 && (unsigned int)DRIVER_TREE_DIST < (unsigned int)Unit + 1 && DRIVER_TREE_DIST != 0 &&
                TaLogic_IsPassInvadeRoot2(pVarInfo, nX, nZ, i, j, 0, fView, Node0, bWarnLvl) &&
                !TaLogic_IsRootPast2(pTree, i, j, nX, nZ, bDepth)) {
                pTree->pNextP[k] = TaLogic_GetNearUnitPickUpTree(pTree->pNextP[k], pTree, pVarInfo, i, j, nSide, bDist,
                    fView, bWarnLvl, bDepth + 1, wSize, wNodeCount, nUnitLst, wUnitNum, wIgnoreLst);
                k++;
            }
        }
    }
    return pTree;
}

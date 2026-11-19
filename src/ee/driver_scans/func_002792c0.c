#include "dotr/ee/driver_scans.h"

int TaLogic_GetRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ, unsigned short gUnitID, unsigned char bAbort)
{
    struct PieceTreetag *pTree;
    unsigned short wIgnoreLst[49];
    unsigned short wSize;
    unsigned short wCount;
    unsigned char bDist;
    unsigned char EndCond;

    EndCond = 0;
    wCount = 0;
    wSize = 0;
    bDist = 1;
    pTree = 0;
    if (pVarInfo == 0) {
        return -1;
    }
    TaLogic_InitIgnoreLst(wIgnoreLst);
    if (SzGame_GetHSyncCount() > 12000) {
        return -1;
    }
    if (GetMoveRev(pVarInfo, nX, nZ) == 0) {
        return 0;
    }
    if (((gX < nX ? nX : gX) - (nX < gX ? nX : gX)) + ((gZ < nZ ? nZ : gZ) - (nZ < gZ ? nZ : gZ)) > bAbort) {
        return -1;
    }
    while (EndCond == 0 && bDist <= bAbort) {
        pTree = TaLogic_GetRootTree2(pTree, 0, pVarInfo, nX, nZ, gX, gZ, bDist, &EndCond, &gUnitID, 0, &wSize, &wCount, wIgnoreLst);
        if (EndCond == 1) {
            TaLogic_GetShortRoot(pMovePiece, pTree, gX, gZ);
            return bDist;
        }
        if (TaLogic_IsConnectRoot(pTree, 0, bDist) == 1) {
            bDist++;
        } else {
            return -1;
        }
        if (SzGame_GetHSyncCount() > 12000) {
            return -1;
        }
    }
    return -1;
}

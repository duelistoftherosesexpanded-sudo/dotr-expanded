#include "dotr/ee/driver_tree.h"

/* TaLogic_GetInvadeRootTree2 (driver.c; prototype and parameter names from the Japanese build's
 * debug information): allocates a node for (nX, nZ) when pTree is null (unless bDepth exceeds
 * bDist; marks *EndCond and stops at the goal), then for every cell of the 7 x 7 board within
 * the move range (GetMoveRev & 15, Manhattan distance 1..range) that TaLogic_IsPassInvadeRoot2
 * accepts and TaLogic_IsRootPast2 has not visited, recurses into the next child slot (one slot
 * per in-range cell, used or not) at depth + 1. Returns the node.
 * The child slots are reached as bytes at +4 of the node (DRIVER_TREE_SLOT_*). MATCHING DEVICE:
 * the local copy of the node keeps the node type; a byte-pointer cast copy of the parameter
 * allocates registers differently. */
struct PieceTreetag *TaLogic_GetInvadeRootTree2(struct PieceTreetag *pTree, struct PieceTreetag *pPrev, struct jp_9a533 *pVarInfo, int nX, int nZ, int gX, int gZ, unsigned char bDist, unsigned char *EndCond, int *gUnitID, int fView, unsigned short bWarnLvl, unsigned char bDepth, unsigned short *wSize, unsigned short *wNodeCount, unsigned short *wIgnoreLst)
{
    int spE0;
    int minRowBound;
    unsigned char sp100;
    unsigned int maxColBound;
    unsigned int maxRowBound;
    unsigned char *currentTree;
    unsigned int sp140;
    int sp150;
    unsigned char sp167;
    int sp168;
    int sp16C;
    int temp_s0;
    int moveRange;
    int var_s0;
    int var_s1;
    unsigned char *currentTree2;
    struct PieceTreetag *var_s6;

    var_s6 = pTree;
    sp16C = gX;
    sp168 = gZ;
    sp167 = bDist;
    moveRange = GetMoveRev(pVarInfo, nX, nZ);
    if (pVarInfo == 0) {
        return 0;
    }
    if (var_s6 == 0) {
        if ((int)bDepth > (int)sp167) {
            return 0;
        }
        if ((var_s6 = TaLogic_RootTreeAloc(wSize, wNodeCount)) == 0) {
            return 0;
        }
        TaLogic_SetRootTreeDat(var_s6, pPrev, nX, nZ);
        if ((nX == sp16C) && (nZ == sp168)) {
            *EndCond = 1;
            return var_s6;
        }
    }
    temp_s0 = moveRange & 0xF;
    spE0 = 0;
    var_s1 = GetCap(0, nX - temp_s0);
    if ((unsigned int)(nX + temp_s0) >= 7U) {
        maxColBound = 6;
    } else {
        maxColBound = nX + temp_s0;
    }
    minRowBound = GetCap(0, nZ - temp_s0);
    maxRowBound = nZ + temp_s0;
    if (maxRowBound >= 7U) {
        maxRowBound = 6;
    }
    sp100 = (bDepth != 0) ? 0U : 1U;
    if (var_s1 <= (int)maxColBound) {
        sp140 = temp_s0 + 1;
        sp150 = bDepth + 1;
        currentTree = (unsigned char *)var_s6;
        do {
            var_s0 = minRowBound;
            if (var_s0 <= (int)maxRowBound) {
                currentTree2 = currentTree;
                do {
                    if ((var_s1 >= 0) && (var_s0 >= 0) && (var_s1 < 7) && (var_s0 < 7) && ((unsigned int)((((var_s1 < nX) ? nX : var_s1) - ((nX < var_s1) ? nX : var_s1)) + (((var_s0 < nZ) ? nZ : var_s0) - ((nZ < var_s0) ? nZ : var_s0))) < sp140) && (((((var_s1 < nX) ? nX : var_s1) - ((nX < var_s1) ? nX : var_s1)) + (((var_s0 < nZ) ? nZ : var_s0) - ((nZ < var_s0) ? nZ : var_s0))) != 0)) {
                        if ((TaLogic_IsPassInvadeRoot2(pVarInfo, nX, nZ, var_s1, var_s0, gUnitID, fView, sp100, bWarnLvl) == 1) && (TaLogic_IsRootPast2(var_s6, var_s1, var_s0, nX, nZ, bDepth) == 0)) {
                            *(struct PieceTreetag **)(currentTree2 + DRIVER_TREE_SLOT_OFFSET) = TaLogic_GetInvadeRootTree2(*(struct PieceTreetag **)(currentTree2 + DRIVER_TREE_SLOT_OFFSET), var_s6, pVarInfo, var_s1, var_s0, sp16C, sp168, sp167, EndCond, gUnitID, fView, bWarnLvl, (unsigned char)sp150, wSize, wNodeCount, wIgnoreLst);
                        }
                        currentTree2 += DRIVER_TREE_SLOT_STEP;
                        currentTree += DRIVER_TREE_SLOT_STEP;
                        spE0 += 1;
                    }
                    var_s0 += 1;
                } while (var_s0 <= (int)maxRowBound);
            }
            var_s1 += 1;
        } while (var_s1 <= (int)maxColBound);
    }
    return var_s6;
}

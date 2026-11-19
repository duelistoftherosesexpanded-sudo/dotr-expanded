#include "dotr/ee/driver_scans.h"

void TaLogic_DoGetShortRoot(struct jp_9b2bd *pMovePiece, int *Count, unsigned char *Dist, struct PieceTreetag *pTree, int gX, int gZ, struct jp_9b2bd *p1stLoc, unsigned char bDepth)
{
    unsigned char Loc;
    int i;

    if (pTree != 0) {
    if ((pTree->bPiece & 0xf) == gX && (pTree->bPiece >> 4) == gZ) {
        if (bDepth < *Dist) {
            *Count = 0;
            *Dist = bDepth;
            Loc = TaLogic_GetNextPiece(pTree, 0);
            /* Host build: `+ 0` only keeps GCC's -Wtype-limits quiet; the test is the
             * original's always-false `Loc < 0` on an unsigned char. */
            if (Loc + 0 < 0) {
                p1stLoc[0].nX = gX;
                p1stLoc[0].nZ = gZ;
            } else {
                p1stLoc[0].nX = (unsigned char)Loc & 0xf;
                p1stLoc[0].nZ = (unsigned char)Loc >> 4;
            }
            p1stLoc[1].nX = 15;
            p1stLoc[1].nZ = 15;
        } else if (*Dist == bDepth) {
            (*Count)++;
            Loc = TaLogic_GetNextPiece(pTree, 0);
            if (Loc + 0 < 0) {
                pMovePiece[1].nX = gX;
                pMovePiece[1].nZ = gZ;
            } else {
                pMovePiece[1].nX = (unsigned char)Loc & 0xf;
                pMovePiece[1].nZ = (unsigned char)Loc >> 4;
            }
        }
        return;
    }
    {
        for (i = 0; i < 12; i++) {
            TaLogic_DoGetShortRoot(pMovePiece, Count, Dist, pTree->pNextP[i], gX, gZ, p1stLoc, bDepth + 1);
        }
    }
}
}

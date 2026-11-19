#include "dotr/ee/block_table.h"

/* Stores the normalised difference cameraPoint - sourcePoint into relativeDirection
 * for the block at the (only reachable) table slot 0. Returns 0 when the
 * index or block is invalid. */
int lSzView_CalcRel(int index)
{
    struct DotrBlockDelta *b;
    if (index < 0 || index > 0) return 0;
    b = gSzView_hView_delta[index];
    if (!b) return 0;
    b->relativeDirection.x = b->cameraPoint.x - b->sourcePoint.x;
    b->relativeDirection.y = b->cameraPoint.y - b->sourcePoint.y;
    b->relativeDirection.z = b->cameraPoint.z - b->sourcePoint.z;
    b->relativeDirection.unknown_w = 0;
    RhGeom_NormalizeVector(&b->relativeDirection);
    return 1;
}

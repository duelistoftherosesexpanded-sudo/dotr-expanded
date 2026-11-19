#include "dotr/ee/small_misc.h"

/* Forwards to 0x2777f0 with two inserted zero arguments and the stack
 * arguments reordered. */
int TaLogic_GetNearUnitL2(int routeTree, int unit, int col, int row, int side, int currentDistance, int canSeeFaceDown, unsigned short warnHideLvl,
                  int ownerIdArray, int numberOfValidTargets, int allocedBytes, int allocedBlocks, int ignoreList)
{
    return TaLogic_GetNearUnitPickUpTree(routeTree, 0, unit, col, row, side, currentDistance, canSeeFaceDown, warnHideLvl, 0, allocedBytes, allocedBlocks, ownerIdArray, numberOfValidTargets, ignoreList);
}

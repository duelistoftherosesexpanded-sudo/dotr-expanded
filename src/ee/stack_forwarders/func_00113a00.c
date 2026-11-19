#include "dotr/ee/stack_forwarders.h"

int TaLogic001_GetNearUnit(int unitTask, int routeTree, int unit, int col, int row, int side, int currentDistance,
                  int warnHideLevel, int unitOwnerIdArray, unsigned short numberOfValidTargets,
                  int allocatedBytes, int allocatedBlocks, unsigned char distance, int ignoreList)
{
    return TaLogic_GetNearUnit(unitTask, routeTree, unit, col, row, side, currentDistance, warnHideLevel,
                               unitOwnerIdArray, numberOfValidTargets, allocatedBytes, allocatedBlocks, distance, 0, 3000, ignoreList);
}

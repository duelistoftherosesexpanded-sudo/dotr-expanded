#include "dotr/ee/small_misc.h"

/* Depth-first search over the twelve links at +4: 1 when a non-null node
 * is reached at currentDistance == targetDistance, 0 otherwise. */
int TaLogic_IsConnectRoot(struct DotrMiscTree *tree, unsigned char currentDistance, unsigned char targetDistance)
{
    int i;
    int canReach;
    if (tree != 0) {
        if (currentDistance == targetDistance) {
            return 1;
        }
        for (i = 0; i < 12; i++) {
            canReach = TaLogic_IsConnectRoot(tree->children[i], currentDistance + 1, targetDistance);
            if (canReach == 1) {
                return canReach;
            }
        }
    }
    return 0;
}

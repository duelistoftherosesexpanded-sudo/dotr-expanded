#include "dotr/ee/lower_rules.h"

int TaLogic013_GetInvadePoint(int currentSide, int col, int row)
{
    if (row <= 2) {
        return col >= 3 ? 0x306 : 0x300; /* row 3, column 6 or column 0 */
    }
    return SzDuel_GetUnitLoc(currentSide == 0 ? 1 : 0, 0);
}

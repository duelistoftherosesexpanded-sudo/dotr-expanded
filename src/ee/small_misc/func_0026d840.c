#include "dotr/ee/small_misc.h"

int Ta_GetUnitGravityLocSearchOnZ(int unused, int target, int y, int x)
{
    (void)unused;
    if (x < target) {
        if (SzDuel_GetMapUnitID(y, x + 1) < 0) {
            return ((x + 1) << 8) | y;
        }
        return -1;
    }
    if (target < x) {
        if (SzDuel_GetMapUnitID(y, x - 1) < 0) {
            return ((x - 1) << 8) | y;
        }
        return -1;
    }
    return -1;
}

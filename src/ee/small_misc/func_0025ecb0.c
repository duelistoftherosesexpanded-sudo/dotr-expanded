#include "dotr/ee/small_misc.h"

int Ta_SearchMoveTemp(void)
{
    int x;
    int y;
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            if (SzDuel_GetMapUnitID(x, y) < 0) {
                return (y << 8) | x;
            }
        }
    }
    return -1;
}

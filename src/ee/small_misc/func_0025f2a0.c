#include "dotr/ee/small_misc.h"

int Ta_GetStackMapPiece(void)
{
    int x;
    int y;
    int result;
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            result = SzDuel_GetMapUnitID(x, y);
            if (result < 0) {
                return result;
            }
        }
    }
    return -1;
}

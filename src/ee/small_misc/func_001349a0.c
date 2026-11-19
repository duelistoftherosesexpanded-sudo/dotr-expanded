#include "dotr/ee/small_misc.h"

int TaLogic008_GetWallEmpty(int side)
{
    int y;
    int x;
    int r;

    for (y = 0; y < 7; y++) {
        if (y % 2 == 1) {
            for (x = 1; x <= 5; x++) {
                if (SzDuel_GetField(x, y) != 8) {
                    return (y << 8) | x;
                }
            }
        }
    }
    for (y = 0; y < 7; y++) {
        for (x = 0; x <= 6; x++) {
            r = SzDuel_GetMapUnitID(y, x);
            if (r >= 0 && side != func_00250360_pair(r >> 8, r & 0xff) && x < 3 &&
                SzDuel_GetField(6, 0) != 8 && SzDuel_GetField(5, 0) != 8) {
                return 6;
            }
        }
    }
    return -1;
}

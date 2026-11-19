#include "dotr/ee/pick_copies.h"

int TaLogic022_IsEmptyCntrGuard(int side, int pos, unsigned char c)
{
    int t;
    int i;
    int n;
    int key;
    int cell;

    if (c == 1) {
        n = 3;
    } else {
        n = 4;
    }
    key = (side << 8) | pos;
    for (i = 1; i <= 5; i++) {
        cell = SzDuel_GetMapUnitID(i, n);
        if (cell != 0) {
            if (cell == key) {
                return 1;
            }
            t = func_00250360_pair(cell >> 8, cell & 0xff);
            if (t == func_00250360_pair(side, pos)) {
                return 0;
            }
        }
    }
    return 1;
}

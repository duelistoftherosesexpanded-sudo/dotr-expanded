#include "dotr/ee/position_rules.h"

int TaLogic003_IsEmptyAtkLine(int side, unsigned char lineIndex)
{
    int y;
    int x;
    switch (lineIndex) {
    case 0:
        x = 2;
        break;
    case 1:
        x = 4;
        break;
    case 2:
        x = 0;
        break;
    case 3:
    default:
        x = 6;
        break;
    }
    for (y = 1; y <= 5; y++) {
        int cell = SzDuel_GetMapUnitID(x, y);
        if (cell >= 0) {
            int lo;
            int hi;
            hi = cell >> 8;
            lo = cell & 0xff;
            if (side == func_00250360_pair(hi, lo) && SzDuel_GetUnitKind(hi, lo) >> 5 == 0) {
                return 0;
            }
        }
    }
    return 1;
}

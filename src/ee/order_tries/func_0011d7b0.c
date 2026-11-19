#include "dotr/ee/order_tries.h"

int TaLogic004_IsEmptyAtkLine(int side, unsigned char lineIndex)
{
    int x;
    int lo;
    int hi;
    int cell;
    int column;
    int row;

    switch (lineIndex) {
    case 0:
        x = 2;
        lo = 2;
        hi = 4;
        break;
    case 1:
        x = 4;
        lo = 2;
        hi = 4;
        break;
    case 2:
        x = 0;
        lo = 1;
        hi = 5;
        break;
    case 3:
    default:
        x = 6;
        lo = 1;
        hi = 5;
        break;
    }
    for (; lo <= hi; lo++) {
        cell = SzDuel_GetMapUnitID(x, lo);
        if (cell >= 0) {
            row = cell >> 8;
            column = cell & 0xff;
            if (func_00250360_pair(row, column) == side && (SzDuel_GetUnitKind(row, column) >> 5) == 0) {
                return 0;
            }
        }
    }
    return 1;
}

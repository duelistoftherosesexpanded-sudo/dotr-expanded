#include "dotr/ee/grid_scans.h"

#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))

int TaLogic004_IsOpenMoveUnit(int row, int column, int x, int y, int to_x, int to_y, int kind)
{
    if ((HI(x, to_x) - LO(x, to_x)) + (HI(y, to_y) - LO(y, to_y)) != 2 || kind != 2) {
        return 0;
    }
    if (func_001df540_scan(row, column) == 0) {
        return 0;
    }
    if (SzDuel_GetUnitStatus(row, column, 0) == 0) {
        if (CheckLandAgreement2(SzDuel_GetField(x, y), row, column) != 1) {
            return 0;
        }
        return 1;
    }
    return 0;
}

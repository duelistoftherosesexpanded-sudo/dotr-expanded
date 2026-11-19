#include "dotr/ee/cell_grades.h"

int TaLogic_IsSafeTrap(int ownerId, int cx, int cy)
{
    int x;
    int y;
    int side;
    int found;
    int b;
    int a;

    side = func_00250360_pair(ownerId >> 8, ownerId & 0xff);
    if (Ta_IsActiveTrapB(side, 4) >= 0) {
        return 1;
    }
    for (x = GetCap(0, cx - 1); x <= GetCap(0, cx + 1); x++) {
        for (y = GetCap(0, cy - 1); y <= GetCap(0, cy + 1); y++) {
            if (((x < cx ? cx : x) - (cx < x ? cx : x)) + ((y < cy ? cy : y) - (cy < y ? cy : y)) == 1) {
                found = SzDuel_GetMapUnitID(x, y);
                if (found >= 0) {
                    a = found >> 8;
                    b = found & 0xff;
                    /* 813: Type Zero Magic Crusher */
                    if (SzDuel_GetUnitKind(a, b) >> 5 == 3 && SzDuel_GetUnitCardNo(a, b) != 813 && side == func_00250360_pair(a, b)) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

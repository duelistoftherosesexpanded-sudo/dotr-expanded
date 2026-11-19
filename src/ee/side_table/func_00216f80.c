#include "dotr/ee/side_table.h"

int SzData_SetTreasureFlag(int side, int currentMap, int flag)
{
    struct DotrSide *p = DOTR_SIDE(side);
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (currentMap < 0 || currentMap >= 22) {
        return 0;
    }
    if (flag) {
        p->treasureFlags[currentMap >> 3] |= 1 << (currentMap % 8);
    } else {
        p->treasureFlags[currentMap >> 3] &= ~(1 << (currentMap % 8));
    }
    return 1;
}

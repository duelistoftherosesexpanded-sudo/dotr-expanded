#include "dotr/ee/side_table.h"

int SzData_GetTreasureFlag(int a, int n)
{
    struct DotrSide *p = DOTR_SIDE(a);
    if (a < 0 || a >= 2) {
        return 0;
    }
    if (n < 0 || n >= 22) {
        return 0;
    }
    return (p->treasureFlags[n >> 3] & (1 << (n % 8))) ? 1 : 0;
}

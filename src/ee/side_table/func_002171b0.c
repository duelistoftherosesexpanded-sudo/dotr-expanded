#include "dotr/ee/side_table.h"

int SzData_GetMapEditFlag(int a)
{
    struct DotrSide *p = DOTR_SIDE(a);
    if (a < 0 || a >= 2) {
        return 0;
    }
    return p->flags.unknown_bit_5 ? 1 : 0;
}

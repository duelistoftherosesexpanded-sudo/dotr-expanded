#include "dotr/ee/side_table.h"

int SzData_SetMapEditFlag(int a, int flag)
{
    struct DotrSide *p = DOTR_SIDE(a);
    if (a < 0 || a >= 2) {
        return 0;
    }
    p->flags.unknown_bit_5 = flag ? 1 : 0;
    return 1;
}

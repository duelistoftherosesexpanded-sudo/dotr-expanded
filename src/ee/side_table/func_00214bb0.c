#include "dotr/ee/side_table.h"

int SzData_SetOption(int side, int which, int flag)
{
    struct DotrSide *p = DOTR_SIDE(side);
    switch (which) {
    case 0:
        p->flags.unknown_bit_2 = flag ? 1 : 0;
        break;
    case 1:
        p->flags.unknown_bit_3 = flag ? 1 : 0;
        break;
    case 2:
        p->flags.unknown_bit_4 = flag ? 1 : 0;
        SzSound_SetOutput(p->flags.unknown_bit_4);
        break;
    default:
        return 0;
    }
    return 1;
}

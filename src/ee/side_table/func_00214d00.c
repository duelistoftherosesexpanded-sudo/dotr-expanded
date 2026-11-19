#include "dotr/ee/side_table.h"

int SzData_GetOption(int side, int which)
{
    struct DotrSide *p = DOTR_SIDE(side);
    switch (which) {
    case 0: return p->flags.unknown_bit_2 ? 1 : 0;
    case 1: return p->flags.unknown_bit_3 ? 1 : 0;
    case 2: return p->flags.unknown_bit_4 ? 1 : 0;
    }
    return -1;
}

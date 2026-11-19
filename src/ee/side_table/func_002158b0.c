#include "dotr/ee/side_table.h"

int SzData_GetCurrentDeck(int side)
{
    struct DotrSide *p = DOTR_SIDE(side);
    if (side < 0 || side >= 2) {
        return -1;
    }
    return p->flags.unknown_bits_0_1;
}

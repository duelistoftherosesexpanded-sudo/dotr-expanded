#include "dotr/ee/side_table.h"

int SzData_GetLookFlag(int a, int id)
{
    struct DotrSide *p = DOTR_SIDE(a);
    if (a < 0 || a >= 2) {
        return 0;
    }
    if (id < 0 || id >= 854) {
        return 0;
    }
    return p->cards[id].unknown_48_bit_14 ? 1 : 0;
}

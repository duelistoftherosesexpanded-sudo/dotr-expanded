#include "dotr/ee/side_table.h"

int SzData_ClearCardInfo(struct DotrSideEntry *e)
{
    if (e == 0) {
        return 0;
    }
    e->unknown_00_bits_0_30 = 0;
    e->unknown_04_bits_0_29 = 0;
    e->unknown_04_bits_30_31 = 3;
    e->unknown_00_bit_31 = 0;
    return 1;
}

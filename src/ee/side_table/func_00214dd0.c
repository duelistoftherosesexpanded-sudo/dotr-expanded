#include "dotr/ee/side_table.h"

struct DotrSideEntry *SzData_CheckBag(int side, int cardNo, int kind, int flag)
{
    struct DotrSide *p;
    struct DotrSideEntry *e;
    int n;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    p = DOTR_SIDE(side);
    e = DOTR_SIDE_ENTRIES(p, cardNo);
    n = 9;
    while (n-- != 0) {
        if (e->unknown_00_bits_0_30 != 0) {
            if (e->unknown_04_bits_30_31 == kind) {
                if ((e->unknown_00_bit_31 != 0) == (flag == 1)) {
                    return e;
                }
            }
        }
        e++;
    }
    return 0;
}

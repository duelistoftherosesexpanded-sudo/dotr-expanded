#include "dotr/ee/side_table.h"

int SzData_GetBagCount(int a, int cardNo, int kind)
{
    struct DotrSide *p;
    struct DotrSideEntry *e;
    int count = 0;
    int n;
    if (a < 0 || a >= 2) {
        return -1;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return -1;
    }
    p = DOTR_SIDE(a);
    e = DOTR_SIDE_ENTRIES(p, cardNo);
    n = 9;
    while (n-- != 0) {
        if (e->unknown_00_bits_0_30 != 0) {
            if (kind < 0 || e->unknown_04_bits_30_31 == kind) {
                count++;
            }
        }
        e++;
    }
    return count;
}

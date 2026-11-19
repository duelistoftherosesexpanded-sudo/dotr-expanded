#include "dotr/ee/side_table.h"

/* Same body as SzData_ClearCardInfo. */
static inline int func_00214fd0_inline(struct DotrSideEntry *e)
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

int SzData_DelBag(int side, int cardNo)
{
    struct DotrSideEntry *e;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    e = SzData_CheckBag(side, cardNo, 3, 0);
    if (e == 0) {
        return 0;
    }
    func_00214fd0_inline(e);
    return 1;
}

#include "dotr/ee/side_table.h"

int SzData_AddBag(int side, int key, int cardNo)
{
    struct DotrSideRecord *r;
    struct DotrSideEntry *e;
    int n;
    if (side < 0 || side >= 2) {
        return 0;
    }
    if (key == 0) {
        key = dotr_side_select(side)->unknown_ff98;
    }
    if (key == 0) {
        return 0;
    }
    if (cardNo == 671) {
        cardNo = 999;
    }
    if (cardNo < 0 || cardNo >= 854) {
        return 0;
    }
    r = (struct DotrSideRecord *)DOTR_SIDE_ENTRIES(DOTR_SIDE(side), cardNo);
    e = r->copies;
    n = 9;
    while (n-- != 0) {
        if (e->unknown_00_bits_0_30 == 0) {
            do {
                e->unknown_04_bits_0_29 = (long)func_0019a5c8() & ~0x40000000L;
            } while (SzData_CheckDup(side, key, e->unknown_04_bits_0_29, cardNo) != 0);
            e->unknown_00_bits_0_30 = key;
            r->unknown_48_bit_14 = 1;
            SzData_PushBagNew(side, cardNo);
            return 1;
        }
        e++;
    }
    return 0;
}

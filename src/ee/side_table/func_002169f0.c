#include "dotr/ee/side_table.h"

int SzData_GetBagCountAll(int a)
{
    struct DotrSide *p = DOTR_SIDE(a);
    int count = 0;
    int id;
    if (a < 0 || a >= 2) {
        return -1;
    }
    for (id = 0; id < 854; id++) {
        struct DotrSideEntry *e = p->cards[id].copies;
        int n = 9;
        while (n-- != 0) {
            if (e->unknown_00_bits_0_30 != 0) {
                if (e->unknown_04_bits_30_31 == 3) {
                    count++;
                }
            }
            e++;
        }
    }
    return count;
}

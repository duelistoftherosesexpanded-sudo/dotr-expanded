#include "dotr/ee/side_table.h"

void lSzDuel_TradeDoSide(int side)
{
    int id;
    int pos;
    struct DotrSideEntry *entry;
    unsigned char *counts;
    unsigned char *p;
    int other;
    struct DotrSideCounts *s = &data_00317490_sidecounts;

    if (side != 0) {
        counts = s->second;
    } else {
        counts = s->first;
    }
    for (id = 0, other = side ^ 1; id < 854; id++) {
        p = &counts[id];
        while (*p != 0) {
            if (SzData_GetBagCount(other, id, -1) >= 9) {
                break;
            }
            for (pos = 0; pos < 9; pos++) {
                entry = SzData_GetCardInfo(side, id, pos);
                if (entry->unknown_00_bits_0_30 != 0 && entry->unknown_04_bits_30_31 == 3
                    && SzData_CheckDup(other, entry->unknown_00_bits_0_30, entry->unknown_04_bits_0_29, id) == 0) {
                    break;
                }
            }
            if (pos >= 9) {
                break;
            }
            SzData_AddBag2(other, entry->unknown_00_bits_0_30, entry->unknown_04_bits_0_29, id);
            SzData_ClearCardInfo(entry);
            (*p)--;
        }
    }
}

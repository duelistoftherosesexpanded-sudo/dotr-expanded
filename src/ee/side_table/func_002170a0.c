#include "dotr/ee/side_table.h"

/* Separate returns retain the observed branches after inlining. The
 * guarded coordinates and four-bit table fields keep delta away from INT_MIN. */
static inline int absolute_delta(int delta)
{
    if (delta >= 0) {
        return delta;
    } else {
        return -delta;
    }
}

int SzData_SearchTreasure(int duelist, int col, int row, int range)
{
    int i;
    if (range <= 0) return 999;
    if (duelist < 0 || duelist >= 22) return 999;
    if (col < 0 || col >= 7 || row < 0 || row >= 7) return 999;
    for (i = 0; i < 22; i++) {
        if (duelist == aTreasureData[i].duelist
                && absolute_delta(aTreasureData[i].unknown_01_bits_0_3 - col) < range
                && absolute_delta(aTreasureData[i].unknown_01_bits_4_7 - row) < range) {
            return aTreasureData[i].cardNo;
        }
    }
    return 999;
}

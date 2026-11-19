#include "dotr/ee/small_misc.h"

/* Position of the n-th (from 0) set bit, counted from bit 21 downward. Positions 0..10 are
 * returned as they are and 11..21 as (position - 11) | 0x100; -1 when there is no such bit. */
int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n)
{
    int i = 0;
    int unitCount = 0;
    for (; i < 22; i++) {
        if (((flagList >> (21 - i)) & 1) == 1) {
            if (unitCount == n) {
                if (i < 11) {
                    return i;
                } else {
                    return (i - 11) | 0x100;
                }
            }
            unitCount++;
        }
    }
    return -1;
}

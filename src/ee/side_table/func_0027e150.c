#include "dotr/ee/small_misc.h"

/* For each of the Ta_GetUnitNumFrom22bitFlagList(flagList) cells
 * Ta_GetUnitOwnerIDFromLstCount(flagList, i) names, when SzDuel_GetUnitStatus(hi, lo, 4) is zero,
 * replaces the accumulator (initially flagList) by ConvUnitIDtoFlagList_OFF(hi, lo, accumulator).
 * Returns the accumulator. */
unsigned TaLogic_RemoveHideUnit(unsigned flagList)
{
    int i;
    int n;
    unsigned acc;
    int lo;
    int hi;
    n = Ta_GetUnitNumFrom22bitFlagList(flagList);
    acc = flagList;
    for (i = 0; i < n; i++) {
        int v = Ta_GetUnitOwnerIDFromLstCount(flagList, i);
        hi = v >> 8;
        lo = v & 0xff;
        if (SzDuel_GetUnitStatus(hi, lo, 4) == 0) {
            acc = ConvUnitIDtoFlagList_OFF(hi, lo, acc);
        }
    }
    return acc;
}

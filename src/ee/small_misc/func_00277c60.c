#include "dotr/ee/small_misc.h"

/* Over the first 22 entries, lowers the level of every entry whose id matches (never raises
 * it) and returns 1 when any matched, else 0. The scan continues after a match; an 0xffff
 * entry that does not match ends it by advancing the index. The index is initialized before
 * the result. */
int TaLogic_IsAdjustUnitLst(struct DotrMiscLevelEntry *entries, int unitOwnerId, int level)
{
    int i = 0;
    int wasFound = 0;
    for (; i < 22; i++) {
        if (unitOwnerId == entries[i].unknown_00) {
            if (level < entries[i].unknown_02) {
                entries[i].unknown_02 = level;
            }
            wasFound = 1;
        } else if (entries[i].unknown_00 == 0xffff) {
            i = 22;
        }
    }
    return wasFound;
}

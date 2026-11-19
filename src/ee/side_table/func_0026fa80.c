#include "dotr/ee/state_queries.h"

/* Returns 255 without an entry; otherwise the high nibble of entry byte 2,
 * reduced by Ta_GetLDLvRev(teamID) when the entry's upper three kind bits are
 * zero and its low five bits equal those of SzDuel_GetUnitKind(teamID, 0). */
int TaHand_GetCardSummonCost2(int cardNo, int teamID)
{
    struct DotrQueryEntryBits *info = (struct DotrQueryEntryBits *)SzDuel_GetCardConstInfo(cardNo);
    int kind;
    int r;
    if (info == 0) {
        return 255;
    }
    r = info->unknown_2_high;
    if ((info->unknown_0 >> 5) == 0) {
        kind = info->unknown_0 & 0x1f;
        if (kind == (SzDuel_GetUnitKind(teamID, 0) & 0x1f)) {
            r -= Ta_GetLDLvRev(teamID);
        }
    }
    return r;
}

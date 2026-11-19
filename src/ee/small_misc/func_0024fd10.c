#include "dotr/ee/small_misc.h"

/* The packed word is formed before the four stores, as in the target. */
int Ta_IsVirus(int team, int unitArrayPos, struct DotrMiscWords4 *out)
{
    if (Ta_IsVirus_Chk(team, unitArrayPos) == 1) {
        int packed = ConvUnitIDtoFlagList(0, team, unitArrayPos);
        out->unknown_00[0] = 112;
        out->unknown_00[1] = 0;
        out->unknown_00[2] = packed;
        out->unknown_00[3] = 0;
        TaUtil_DoOpen(team, unitArrayPos, 1);
        return 1;
    }
    return 0;
}

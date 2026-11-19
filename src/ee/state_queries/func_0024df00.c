#include "dotr/ee/state_queries.h"

/* The same forward for the item at (side, pos), gated on the range query and a
 * nonzero `pos`. */
int CheckLandAgreement2(int field, int side, int pos)
{
    if ((SzDuel_GetUnitKind(side, pos) >> 5) != 0 || pos == 0) {
        return 0;
    }
    return Do_CheckLandAgreement(field, SzDuel_GetUnit(side, pos));
}

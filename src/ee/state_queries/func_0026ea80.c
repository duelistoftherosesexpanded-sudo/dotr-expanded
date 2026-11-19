#include "dotr/ee/state_queries.h"

/* Returns Ta_GetLDLvRev(side) when SzDuel_GetCardConstInfo(id) gives an entry whose first byte
 * has the low five bits of SzDuel_GetUnitKind(side, 0) and zero upper three bits; otherwise 0. */
int Ta_GetSummonRev(int side, int id)
{
    struct DotrStateEntry20 *e = SzDuel_GetCardConstInfo(id);
    unsigned char b;
    if (e == 0) {
        return 0;
    }
    b = *(unsigned char *)e;
    if ((b & 0x1f) == (SzDuel_GetUnitKind(side, 0) & 0x1f)) {
        if ((b >> 5) == 0) {
            return Ta_GetLDLvRev(side);
        }
    }
    return 0;
}

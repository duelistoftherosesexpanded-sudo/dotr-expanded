#include "dotr/ee/state_queries.h"

/* Returns 1 when Ta_IsLDRange(side, low byte, upper bits of packed) and
 * Ta_IsLDAbl(ability, side) are non-zero and kind equals the low five bits of
 * SzDuel_GetUnitConstKind(side, 0); otherwise 0. */
int Ta_IsLDRangeAvilityFromLoc(int packed, int side, int kind, int ability)
{
    if (Ta_IsLDRange(side, packed & 0xff, packed >> 8) != 0) {
        if (Ta_IsLDAbl(ability, side) != 0) {
            if (kind == (SzDuel_GetUnitConstKind(side, 0) & 0x1f)) {
                return 1;
            }
        }
    }
    return 0;
}

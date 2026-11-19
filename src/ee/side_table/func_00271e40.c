#include "dotr/ee/state_queries.h"

/* Returns 1 when the cell of (side, pos) passes Ta_IsLDRange for the owner
 * Ta_GetCurrentSide(side, pos), Ta_IsLDAbl(ability, owner) is non-zero and the low five
 * bits of SzDuel_GetUnitKind(side, pos) equal those of SzDuel_GetUnitConstKind(owner, 0). */
int Ta_IsLDRangeAvility(int side, int pos, int ability)
{
    int unitLoc;
    int kind;
    int currentSide;
    unitLoc = SzDuel_GetUnitLoc(side, pos);
    currentSide = Ta_GetCurrentSide(side, pos);
    kind = SzDuel_GetUnitKind(side, pos) & 0x1f;
    if (Ta_IsLDRange(currentSide, unitLoc & 0xff, unitLoc >> 8) != 0) {
        if (Ta_IsLDAbl(ability, currentSide) != 0) {
            if (kind == (SzDuel_GetUnitConstKind(currentSide, 0) & 0x1f)) {
                return 1;
            }
        }
    }
    return 0;
}

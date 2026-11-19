#include "dotr/ee/state_queries.h"

/* Same test as Ta_IsLDRangeAvility with the cell coordinates (col, row) supplied by
 * the caller. */
int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability)
{
    int kind;
    int currentSide;
    currentSide = Ta_GetCurrentSide(side, unitArrayPos);
    kind = SzDuel_GetUnitKind(side, unitArrayPos) & 0x1f;
    if (Ta_IsLDRange(currentSide, col, row) != 0) {
        if (Ta_IsLDAbl(ability, currentSide) != 0) {
            if (kind == (SzDuel_GetUnitConstKind(currentSide, 0) & 0x1f)) {
                return 1;
            }
        }
    }
    return 0;
}

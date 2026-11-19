#include "dotr/ee/state_queries.h"

/* Returns `side`, or its 0/1 complement when SzDuel_GetUnitStatus(side, pos, 1) (bit 1 of the
 * item's +0x57 byte, revolt) is non-zero. */
int Ta_GetCurrentSide(int side, int pos)
{
    if (SzDuel_GetUnitStatus(side, pos, 1) != 0) {
        return side == 0 ? 1 : 0;
    }
    return side;
}

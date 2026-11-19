#include "dotr/ee/state_queries.h"

/* Picks index 1 when side is 0 (for side < 2) or when SzDuel_GetTurnOwner() is 0 (for
 * side >= 2), else index 0; halves SzDuel_GetLP(index), passes it to
 * Ta_GetLPRevByNaturalEffect(44, half) and adds b to the result. */
int Ta_GetForceEffection(int side, int b)
{
    int half;
    int r;
    if (side < 2) {
        half = SzDuel_GetLP(side == 0 ? 1 : 0) >> 1;
    } else {
        half = SzDuel_GetLP(SzDuel_GetTurnOwner() == 0 ? 1 : 0) >> 1;
    }
    r = Ta_GetLPRevByNaturalEffect(44, half);
    return r + b;
}

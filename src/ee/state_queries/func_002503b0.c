#include "dotr/ee/state_queries.h"

/* The limit is written on the left of `>`; `n < limit` selects another
 * scratch register for the comparison. */
void Ta_SetUnitCurseRest(int a, int b, unsigned char n)
{
    if (n > 0) {
        if (SzDuel_GetUnitCurseRest(a, b) > n) {
            return;
        }
        SzDuel_SetUnitCurseRest(a, b, n);
        SzDuel_SetUnitCurseFlag(a, b, 1);
    } else {
        SzDuel_SetUnitCurseRest(a, b, n);
    }
}

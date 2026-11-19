#include "dotr/ee/state_queries.h"

/* `i <= 4` selects the observed scratch register for the bound; `i < 5` does not. */
int TaUtil_IsExMonster(int side, int pos)
{
    int i;
    if (SzDuel_GetUnitKind(side, pos) >> 5) {
        return -1;
    }
    for (i = 0; i <= 4; i++) {
        if (Ta_GetUnitEffect(side, pos, i)) {
            return 1;
        }
    }
    return 0;
}

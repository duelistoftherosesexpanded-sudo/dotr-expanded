#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitSpellCount(int side)
{
    int n;
    int i;
    for (i = 6, n = 0; i < 11; i++) {
        if (dotr_state_item_checked(side, i)) {
            n++;
        }
    }
    return n;
}

#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitMonsterCount(int side)
{
    int monsterCount;
    int i;
    for (i = 1, monsterCount = 0; i < 6; i++) {
        if (dotr_state_item_checked(side, i)) {
            monsterCount++;
        }
    }
    return monsterCount;
}

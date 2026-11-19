#include "dotr/ee/state_grid.h"

struct DotrStateEntry8 *SzDuel_GetUnitEffect(int side, int pos, int effInd)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) {
        return 0;
    }
    if (effInd < 0 || effInd >= 5) {
        return 0;
    }
    return &unit->cardEffects[effInd];
}

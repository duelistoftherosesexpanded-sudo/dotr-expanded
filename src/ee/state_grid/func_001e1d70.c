#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitOpenRate(int side, int pos, int pair, int openRate)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) {
        return 0;
    }
    unit->unknown_56.unknown_bits_4_5 = pair & 3;
    if (openRate >= 0) {
        unit->openRate = openRate;
    }
    return 1;
}

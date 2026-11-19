#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) {
        return 0;
    }
    unit->unknown_56.unknown_bits_2_3 = pair & 3;
    if (value >= 0) {
        unit->ctlRateM = value;
    }
    return 1;
}

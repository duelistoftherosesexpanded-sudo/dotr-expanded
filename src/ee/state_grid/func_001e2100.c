#include "dotr/ee/state_grid.h"

int SzDuel_GetUnitStatus(int side, int pos, int bit)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) return 0;
    /* bit: 0 has moved, 1 revolt, 2 showing face up, 3 showing defense, 4 face up,
     * 5 in defense. */
    switch (bit) {
    case 0: return unit->status.unknown_bit_0 ? 1 : 0;
    case 1: return unit->status.unknown_bit_1 ? 1 : 0;
    case 2: return unit->status.unknown_bit_2 ? 1 : 0;
    case 3: return unit->status.unknown_bit_3 ? 1 : 0;
    case 4: return unit->status.unknown_bit_4 ? 1 : 0;
    case 5: return unit->status.unknown_bit_5 ? 1 : 0;
    default: return 0;
    }
}

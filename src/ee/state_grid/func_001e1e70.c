#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitStatus(int side, int pos, int bit, int value)
{
    struct DotrStateItem *unit = dotr_state_item_checked(side, pos);
    if (unit == 0) return 0;
    /* bit: 0 has moved, 1 revolt, 2 showing face up, 3 showing defense, 4 face up,
     * 5 in defense. */
    switch (bit) {
    case 0:
        unit->status.unknown_bit_0 = value;
        break;
    case 1:
        unit->status.unknown_bit_1 = value;
        break;
    case 2:
        unit->status.unknown_bit_2 = value;
        unit->unknown_56.unknown_bits_4_5 = (value ? 1 : -1) & 3;
        break;
    case 3:
        unit->status.unknown_bit_3 = value;
        unit->unknown_56.unknown_bits_6_7 = (value ? 1 : -1) & 3;
        break;
    case 4:
        unit->status.unknown_bit_4 = value;
        unit->status.unknown_bit_2 = value;
        unit->unknown_56.unknown_bits_4_5 = (value ? 1 : -1) & 3;
        break;
    case 5:
        unit->status.unknown_bit_5 = value;
        unit->status.unknown_bit_3 = value;
        unit->unknown_56.unknown_bits_6_7 = (value ? 1 : -1) & 3;
        break;
    default:
        return 0;
    }
    return 1;
}

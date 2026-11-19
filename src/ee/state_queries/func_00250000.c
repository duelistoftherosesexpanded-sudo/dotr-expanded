#include "dotr/ee/state_queries.h"

/* Classifies the list value at `position`: mode 0 accepts any value, mode 1
 * values below 683 and mode 2 values from 683. The value variable is reused
 * for the mode-2 result, as the emitted register use requires. */
int Ta_IsGraveEntryHit(int side, int position, int mode)
{
    int value;
    if (SzDuel_GetGraveCount(side) < position) {
        return 0;
    }
    value = SzDuel_InqGrave(side, position);
    if (value != 999) {
        switch (mode) {
        case 0:
            return 1;
        case 1:
            return value < 683 ? 1 : 0;
        case 2:
            value = value < 683;
            if (!value) {
                value = 1;
            } else {
                value = 0;
            }
            return value;
        }
    }
    return 0;
}

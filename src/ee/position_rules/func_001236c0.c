#include "dotr/ee/position_rules.h"

/* The body of 0x123670, which the target has expanded in place here. */
static inline int spot_of(int index)
{
    switch (index) {
    case 0:
        return 0x303;
    case 1:
        return 0x304;
    default:
    case 2:
        return 0x302;
    }
}
int TaLogic005_IsEmptyDefensePoint(int index)
{
    int spot;

    spot = spot_of(index);
    if (spot >= 0 && SzDuel_GetMapUnit(spot & 0xff, spot >> 8) == 0) {
        return 1;
    }
    return 0;
}

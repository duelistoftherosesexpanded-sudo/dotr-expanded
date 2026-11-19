#include "dotr/ee/position_rules.h"

void TaLogic009_CommandDefenseUnit(struct DotrPositionOrder *order)
{
    int r = TaLogic009_GetEmptyDefensePoint();
    if (r >= 0) {
        order->kind = 0x3ff;
        order->flags.word = (r << 16) | order->flags.half;
    } else {
        order->kind = 0x5ff;
    }
}

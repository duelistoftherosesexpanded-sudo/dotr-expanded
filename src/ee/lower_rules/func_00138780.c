#include "dotr/ee/lower_rules.h"

int TaLogic_GetMinimumTask_ReviveMission__00138780(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    struct DotrRuleNibbles cells[12];
    int found;
    int index;
    int word;
    int k;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    for (k = 0; k < 12; k++) {
        cells[k].low = cells[k].high = 15;
    }
    found = TaLogic009_IsUseCopyCat(side);
    if (TaLogic_GetActionReaction(order->position >> 8, order->position & 0xff, 0, 0, 6, 1) == 5) {
        found = -1;
    }
    if (found >= 0) {
        index = (found >> 16) & 0xffff;
        word = TaLogic_GetGraveEntryAfterScreen(index, found & 0xff, 0);
        result->position = order->position;
        result->kind = 12;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 1);
        result->shouldFlipUp = 1;
        result->loc = (index << 8) | word;
        func_0027b1b0_order(order, mode);
        return 0;
    }
    if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) != 0) {
        SzDuel_SetUnitStatus(order->position >> 8, order->position & 0xff, 0, 1);
        result->position = order->position;
        result->kind = 14;
        return 0;
    }
    return func_001374d0_rest(order, side, result, mode);
}

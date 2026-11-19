#include "dotr/ee/lower_rules.h"

/* TaLogic_GetMinimumTask_ReviveMission (copy at 0x14aab0): asks TaLogic013_IsUseRevive(side) for
 * a grave entry; the answer is dropped when the field under the unit is 9 or when
 * TaLogic_GetActionReaction gives 5. A kept answer makes the result kind 12 with
 * (high halfword << 8) | TaLogic_GetGraveEntryAfterScreen(high halfword, low byte, 1).
 * Otherwise a unit with status bit 4 has it replaced by bit 0 and gets kind 14, and anything
 * else goes to the fallback TaLogic_GetMinimumTask_RunMission__00149690. */
int TaLogic_GetMinimumTask_ReviveMission__0014aab0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    struct DotrRuleNibbles cells[12];
    int loc;
    int found;
    int word;
    int index;
    int k;

    loc = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    for (k = 0; k < 12; k++) {
        cells[k].low = cells[k].high = 15;
    }
    found = TaLogic013_IsUseRevive(side);
    if (SzDuel_GetField(loc & 0xff, loc >> 8) == 9) {
        found = -1;
    }
    if (TaLogic_GetActionReaction(order->position >> 8, order->position & 0xff, 0, 0, 6, 0) == 5) {
        found = -1;
    }
    if (found >= 0) {
        index = (found >> 16) & 0xffff;
        word = TaLogic_GetGraveEntryAfterScreen(index, found & 0xff, 1);
        result->position = order->position;
        result->kind = 12;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
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
    return func_00149690_rest(order, side, result, mode);
}

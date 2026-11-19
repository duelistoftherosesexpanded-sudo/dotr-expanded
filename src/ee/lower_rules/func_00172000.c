#include "dotr/ee/lower_rules.h"

int TaLogic_GetMinimumTask_RevoltMission__00172000(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    struct DotrRuleNibbles cells[12];
    int count;
    int found;
    int limit;
    int i;
    int k;

    SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    for (k = 0; k < 12; k++) {
        cells[k].low = cells[k].high = 15;
    }
    count = 0;
    found = TaLogicCom_GetRevoltObj__0016b3c0(side);
    if (TaLogic_GetActionReaction(order->position >> 8, order->position & 0xff, 0, 0, 6, 0) == 5) {
        found = -1;
    }
    if (found < 0) {
        goto other;
    }
    limit = (found >> 16) & 0xffff;
    if (limit != 0xffff) {
        for (i = 1; i < 11; i++) {
            if (func_001df540_word(side == 0 ? 1 : 0, i) != 0) {
                count++;
            }
            if (i == limit) {
                i = 11;
            }
        }
        result->position = order->position;
        result->kind = 13;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        result->shouldFlipUp = 1;
        result->loc = count - 1;
        func_0027b1b0_order(order, mode);
        return 0;
    }
other:
    if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) != 0) {
        SzDuel_SetUnitStatus(order->position >> 8, order->position & 0xff, 0, 1);
        result->position = order->position;
        result->kind = 14;
        return 0;
    }
    return func_00170940_rest(order, side, result, mode);
}

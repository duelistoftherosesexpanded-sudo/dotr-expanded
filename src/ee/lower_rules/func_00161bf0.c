#include "dotr/ee/lower_rules.h"
int TaLogic_GetMinimumTask_ReviveMission__00161bf0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, int mode)
{
    struct DotrRuleNibbles cells[12];
    int r;
    int found;
    unsigned *entry;
    int kind;
    int k;
    kind = (SzDuel_GetUnitKind(order->position >> 8, order->position & 0xff) >> 5) & 0xff;
    if (kind == 0) {
        entry = SzDuel_GetUnitEffect(order->position >> 8, order->position & 0xff, 3);
    } else {
        entry = SzDuel_GetUnitEffect(order->position >> 8, order->position & 0xff, 0);
    }
    r = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    for (k = 0; k < 12; k++) {
        cells[k].low = cells[k].high = 15;
    }
    if (entry != 0) {
        if (kind != 0) {
            found = TaLogic019_IsUseRevive(side);
            if (SzDuel_GetField(r & 0xff, r >> 8) == 9) {
                found = -1;
            }
            if (TaLogic_GetActionReaction(order->position >> 8, order->position & 0xff, 0, 0, 6, 0) == 5) {
                found = -1;
            }
        } else {
            found = TaLogic019_IsUseSaint(side);
        }
        if (found >= 0) {
            r = (found >> 16) & 0xffff;
            kind = TaLogic_GetGraveEntryAfterScreen(r, found & 0xff, *entry & 0x3f);
            result->position = order->position;
            result->kind = 12;
            result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
            result->shouldFlipUp = 1;
            if (((*entry >> 6) & 3) != 3) {
                result->loc = (side << 8) | kind;
            } else {
                result->loc = (r << 8) | kind;
            }
            func_0027b1b0_order(order, mode);
            return 0;
        }
    }
    if (kind != 0) {
        if (SzDuel_GetUnitStatus(order->position >> 8, order->position & 0xff, 4) != 0) {
            SzDuel_SetUnitStatus(order->position >> 8, order->position & 0xff, 0, 1);
            result->position = order->position;
            result->kind = 14;
            return 0;
        }
    }
    return func_001607d0_rest(order, side, result, mode);
}

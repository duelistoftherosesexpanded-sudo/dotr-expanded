#include "dotr/ee/atk_missions.h"
#define SPOT (mode == 1 ? ((unsigned)order->flags.word >> 16) & 0xffff : order->flags.half)
#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))
/* This unit's copy of TaLogic_GetWarnHideLvl (out of line at 0x11aa10), expanded in place at
 * each of its three uses, as in the original code. */
static inline int warn_hide_lvl(int side)
{
    if (func_001df470_side(side) > 1500) {
        if (SzDuel_GetUnitMonsterCount(side) >= 3) {
            return 0;
        }
        return 0;
    }
    return 0;
}
int TaLogic_GetMinimumTask_AtkMission__0011aa70(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode)
{
    struct DotrRuleNibbles cells[12];
    int here;
    int count;
    int y1;
    int x1;
    int found;
    int open;
    int k;
    int v;
    struct DotrRuleNibbles *q;
    int best_i;
    int there;
    int y;
    int x;
    struct DotrRuleNibbles *p;
    int i;
    int best;
    if (func_001df540_word(SPOT >> 8, SPOT & 0xff) == 0) {
        func_0027b1b0_mode(order, 1);
        return -1;
    }
    open = func_0011c140_order(order->position >> 8, order->position & 0xff, order);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(SPOT >> 8, SPOT & 0xff);
    for (k = 0, q = cells; k < 12; k++, q++) {
        q->low = q->high = 15;
    }
    if (func_002464f0_rule(func_001df540_word(order->position >> 8, order->position & 0xff), x1 = here & 0xff, y1 = here >> 8) == 0) {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        if (TaLogic_GetUnitWarning(x1, y1, side, 3, 1, warn_hide_lvl(side)) >= SzDuel_GetUnitAP(order->position >> 8, order->position & 0xff, 0)) {
            result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        } else {
            result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
        }
        func_0011c3e0_mode(order, mode);
        return 0;
    }
    func_0027a090_rule(order->position >> 8, order->position & 0xff, cells, x1, y1, x = there & 0xff, y = there >> 8,
        SPOT, 0, 30, warn_hide_lvl(side));
    count = func_0027afc0_rule(cells);
    best_i = 255;
    if (count > 0) {
        for (found = 0, best = 0xffff, i = 0; i < count; i++) {
            if (TaLogic003_IsOpenMoveUnit(order->position >> 8, order->position & 0xff, x1, y1, x, y) == 1) {
                best_i = i;
                found = 1;
                cells[i].low = x;
                cells[i].high = y;
                i = count;
            } else {
                v = warn_hide_lvl(side);
                p = &cells[i];
                v = TaLogic_GetUnitWarning(p->low, p->high, side, 3, 1, v);
                if (v < best) {
                    best = v;
                    best_i = i;
                } else if (v == best) {
                    if ((HI(p->low, x) - LO(p->low, x)) +
                        (HI(p->high, y) - LO(p->high, y)) <
                        (HI(cells[best_i].low, x) - LO(cells[best_i].low, x)) +
                        (HI(cells[best_i].high, y) - LO(cells[best_i].high, y))) {
                        best = v;
                        best_i = i;
                    }
                }
            }
        }
    } else {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->shouldFlipUp = open;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
        func_0027b1b0_mode(order, mode);
        return 0;
    }
    if (best_i != 255) {
        result->position = order->position;
        result->kind = 6;
        result->loc = cells[best_i].high << 8 | cells[best_i].low;
        result->shouldFlipUp = found;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
        func_0011c3e0_mode(order, mode);
    } else {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->shouldFlipUp = found;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
        func_0011c3e0_mode(order, mode);
        return 0;
    }
    return 0;
}

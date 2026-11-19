#include "dotr/ee/lower_rules.h"

#define SPOT (mode == 1 ? ((unsigned)order->flags.word >> 16) & 0xffff : order->flags.half)
#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))

int TaLogic_GetMinimumTask_TrapMission__00153550(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode)
{
    struct DotrRuleNibbles cells[12];
    int here;
    int count;
    int k;
    int v;
    struct DotrRuleNibbles *q;
    int best;
    int best_i;
    int there;
    struct DotrRuleNibbles *p;
    int y;
    int x;
    int i;
    unsigned char flag;

    flag = func_00153ab0_flag();
    if (func_001df540_word(SPOT >> 8, SPOT & 0xff) == 0) {
        func_0027b1b0_order(order, 1);
        return -1;
    }
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(SPOT >> 8, SPOT & 0xff);
    if (SPOT == 0xffff) {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        return 0;
    }
    if (func_002464f0_rule(func_001df540_word(order->position >> 8, order->position & 0xff), here & 0xff, here >> 8) == 0 ||
        TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xff, 0) == 1) {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        return 0;
    }
    p = cells;
    for (k = 0, q = p; k < 12; k++, q++) {
        q->low = q->high = 15;
    }
    func_0027a090_rule(order->position >> 8, order->position & 0xff, cells, here & 0xff, here >> 8, x = there & 0xff, y = there >> 8,
        SPOT, 0, 30, func_00153ed0_key(side));
    count = func_0027afc0_rule(cells);
    best_i = 255;
    if (count > 0) {
        for (best = 0, i = 0; i < count; i++, p++) {
            if (SzDuel_GetMapUnit(p->low, p->high) == 0) {
                v = TaLogic_GetUnitWarning(p->low, p->high, side, (unsigned char)(flag ? 5 : 2), 1, func_00153ed0_key(side));
                if (best < v) {
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
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        return 0;
    }
    if (best_i != 255) {
        result->position = order->position;
        result->kind = 6;
        result->loc = cells[best_i].high << 8 | cells[best_i].low;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
    } else {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        return 0;
    }
    return 0;
}

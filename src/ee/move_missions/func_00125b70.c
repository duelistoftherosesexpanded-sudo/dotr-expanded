#include "dotr/ee/move_missions.h"
#define SPOT (m == 1 ? ((unsigned)order->flags.word >> 16) & 0xffff : order->flags.half)
#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))
/* The unit's TaLogic_GetWarnHideLvl (no out-of-line copy is assumed for this unit;
 * the test is expanded in place in the original code). The caller keeps only the low byte. */
static inline int warn_hide_lvl(int side)
{
    if (func_001df470_side(side) > 1000) {
        if (SzDuel_GetUnitMonsterCount(side) >= 3) {
            return 500;
        }
        return 1000;
    }
    return 2000;
}
int TaLogic_GetMinimumTask_MoveMission__00125b70(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode, unsigned char flag)
{
    struct DotrRuleNibbles cells[12];
    int here;
    int count;
    int k;
    int id;
    int v;
    struct DotrRuleNibbles *q;
    int best;
    int best_i;
    int m;
    struct DotrRuleNibbles *p;
    int i;
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    /* MATCHING DEVICE: the copy puts the mode in s2, ahead of p and i; the calls still read the
     * stored byte. */
    m = mode;
    if (SPOT == 0xffff) {
        if ((unsigned)here == SPOT) {
            result->position = order->position;
            result->kind = 6;
            result->loc = here;
            result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, flag, 0);
            result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
            func_00126940_mode(order, mode);
            return 0;
        }
    }
    p = cells;
    for (k = 0, q = p; k < 12; k++, q++) {
        q->low = q->high = 15;
    }
    id = SzDuel_GetMapUnitID(SPOT & 0xff, SPOT >> 8);
    if (id < 0) {
        id = 0xffff;
    }
    TaLogic_GetRoot(order->position >> 8, order->position & 0xff, cells, here & 0xff, here >> 8, SPOT & 0xff, SPOT >> 8, id & 0xffff, 7);
    count = func_0027afc0_rule(cells);
    if (count > 0) {
        best_i = 255;
        best = 0xffff;
        for (i = 0; i < count; i++, p++) {
            if (SzDuel_GetMapUnit(p->low, p->high) == 0) {
                v = TaLogic_GetUnitWarning(p->low, p->high, side, 2, 1, warn_hide_lvl(side));
                if (v < best) {
                    best = v;
                    best_i = i;
                } else if (v == best) {
                    if ((HI(p->low, SPOT & 0xff) - LO(p->low, SPOT & 0xff)) +
                        (HI(p->high, SPOT >> 8) - LO(p->high, SPOT >> 8)) <
                        (HI(cells[best_i].low, SPOT & 0xff) - LO(cells[best_i].low, SPOT & 0xff)) +
                        (HI(cells[best_i].high, SPOT >> 8) - LO(cells[best_i].high, SPOT >> 8))) {
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
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, flag, 0);
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
        func_00126940_mode(order, mode);
        return 0;
    }
    if (best_i != 255) {
        result->position = order->position;
        result->kind = 6;
        result->loc = cells[best_i].high << 8 | cells[best_i].low;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, flag, 0);
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
    } else {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, flag, 0);
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
    }
    func_00126940_mode(order, mode);
    return 0;
}





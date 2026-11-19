#include "dotr/ee/atk_missions.h"
#define SPOT (mode == 1 ? ((unsigned)order->flags.word >> 16) & 0xffff : order->flags.half)
#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))
/* This logic's copy of the TaLogic_GetWarnHideLvl body (no out-of-line copy with these constants
 * is known), expanded in place at each of the three uses in the original code. */
static inline int warn_hide_lvl(int side)
{
    if (func_001df470_side(side) > 1500) {
        if (SzDuel_GetUnitMonsterCount(side) >= 4) {
            return 0;
        }
        return 0;
    }
    return 0;
}
/* The open-move test, expanded in place in the original code (compare TaLogic003_IsOpenMoveUnit at
 * 0x11c2e0, which has no card test). The caller keeps the unit's two position bytes in locals of its
 * own; passing the expressions directly compiles to the same code with other saved registers. */
static inline int open_move(int a, int b, int x1, int y1, int x2, int y2)
{
    int ylo, yhi, xlo, xhi;

    ylo = y1 < y2 ? y1 : y2;
    yhi = y1 > y2 ? y1 : y2;
    xlo = x1 < x2 ? x1 : x2;
    xhi = x1 > x2 ? x1 : x2;
    if ((xhi - xlo) + (yhi - ylo) != 2) {
        return 0;
    }
    if (func_001df540_word(a, b) == 0) {
        return 0;
    }
    if (SzDuel_GetUnitCardNo(a, b) == 417) {
        return 0;
    }
    if (SzDuel_GetUnitStatus(a, b, 0) != 0 || CheckLandAgreement2(SzDuel_GetField(x1, y1), a, b) != 1) {
        return 0;
    }
    return 1;
}
int TaLogic_GetMinimumTask_AtkMission__0010fff0(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode)
{
    struct DotrRuleNibbles cells[12];
    int here;
    int best_i;
    int count;
    int found;
    int best;
    int a;
    int b;
    int open;
    int k;
    int v;
    struct DotrRuleNibbles *q;
    int there;
    int y1;
    int x1;
    int y;
    int x;
    struct DotrRuleNibbles *p;
    int i;
    if (func_001df540_word(SPOT >> 8, SPOT & 0xff) == 0) {
        func_0027b1b0_mode(order, 1);
        return -1;
    }
    open = TaLogic_IsOpenUnit__0010ee00(order->position >> 8, order->position & 0xff, (unsigned *)order);
    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    there = SzDuel_GetUnitLoc(SPOT >> 8, SPOT & 0xff);
    for (k = 0, q = cells; k < 12; k++, q++) {
        q->low = q->high = 15;
    }
    if (func_002464f0_rule(func_001df540_word(order->position >> 8, order->position & 0xff), x1 = here & 0xff, y1 = here >> 8) == 0) {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        if (TaLogic_GetUnitWarning(x1, y1, side, 3, 1, warn_hide_lvl(side)) >= TaUtil_GetUnitAP(order->position >> 8, order->position & 0xff, x1, y1)) {
            result->atkDefPosition = 1;
        } else {
            result->atkDefPosition = 0;
        }
        return 0;
    }
    func_0027a090_rule(order->position >> 8, order->position & 0xff, cells, x1, y1, x = there & 0xff, y = there >> 8,
        SPOT, 0, 30, warn_hide_lvl(side));
    count = func_0027afc0_rule(cells);
    if (count > 0) {
        best = 0xffff;
        best_i = 255;
        for (found = 0, i = 0; i < count; i++) {
            b = order->position & 0xff;
            a = order->position >> 8;
            if (open_move(a, b, x1, y1, x, y) == 1) {
                found = 1;
                best_i = i;
                cells[i].low = x;
                cells[i].high = y;
                i = count;
            } else {
                v = warn_hide_lvl(side);
                p = &cells[i];
                v = TaLogic_GetUnitWarning(p->low, p->high, side, 2, 1, v);
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
        return 0;
    }
    if (best_i != 255) {
        result->position = order->position;
        result->kind = 6;
        result->loc = cells[best_i].high << 8 | cells[best_i].low;
        result->shouldFlipUp = found;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
    } else {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->shouldFlipUp = found;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
        return 0;
    }
    return 0;
}

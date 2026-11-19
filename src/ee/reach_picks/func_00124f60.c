#include "dotr/ee/reach_picks.h"

#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))

/* The reach is spelled as an expression: the original keeps one spilled copy of it and
 * reuses r's register. */
#define range (r & 0xf)
int TaLogic_GetMinimumTask_RunMission__00124f60(struct DotrRuleOrder *order, int side, struct DotrRuleResult *result, unsigned char mode)
{
    struct DotrRuleNibbles cells[12];
    int r;
    int j;
    unsigned char x0;
    int v;
    unsigned char y0;
    unsigned char y1;
    /* MATCHING DEVICE: the row word j << 8 is a variable of its own; written as
     * j << 8 | i, it shares a register with y0 too late. */
    int w;
    int y;
    int x;
    int i;
    int best_pos;
    int best;
    int here;
    int k;
    struct DotrRuleNibbles *q;
    unsigned char x1;
    unsigned char flag;

    here = SzDuel_GetUnitLoc(order->position >> 8, order->position & 0xff);
    for (k = 0, q = cells; k < 12; k++, q++) {
        q->low = q->high = 15;
    }
    if (TaLogic_GetUnitWarning(x = here & 0xff, y = here >> 8, order->position, 2, func_001254a0_free(side) & 0xffff, 0) == 0) {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
        func_00126940_mode(order, mode);
        return 0;
    }
    if ((r = func_002464f0_rule(func_001df540_word(order->position >> 8, order->position & 0xff), x, y)) == 0) {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
        func_00126940_mode(order, mode);
        return 0;
    }
    best_pos = 255;
    best = 0xffff;
    x0 = GetCap(0, x - range);
    x1 = (x + range >= 7) ? 6 : x + range;
    y0 = GetCap(0, y - range);
    y1 = (y + range >= 7) ? 6 : y + range;
    if ((order->position & 0xff) == 0) {
        flag = TaLogic_IsAttackedNow(order->position >> 8, order->position & 0xff, 0);
    } else {
        flag = 0;
    }
    for (i = x0; i <= x1; i++) {
        for (j = y0, w = j << 8; j <= y1; w += 256, j++) {
            if ((HI(x, i) - LO(x, i)) + (HI(y, j) - LO(y, j)) <= range) {
                if (TaLogic_IsRunPassRoot(order->position >> 8, order->position & 0xff, x, y, i, j, order, flag) == 1) {
                    if (TaLogic_IsAttackedNow2(order->position >> 8, order->position & 0xff, i, j, 0) == 0 ||
                        (unsigned char)order->position == 0) {
                        v = TaLogic_GetUnitWarning(i, j, order->position, 2, func_001254a0_free(side) & 0xffff, 0);
                        if (v < best) {
                            best = v;
                            best_pos = w | i;
                        } else if (v == best) {
                            if (SzDuel_GetMapUnit(i, j) == 0 && SzDuel_GetMapUnit(best_pos & 0xff, best_pos >> 8) != 0) {
                                best = v;
                                best_pos = w | i;
                            }
                        }
                    }
                }
            }
        }
    }
    if (best_pos != 255) {
        result->position = order->position;
        result->kind = 6;
        result->loc = best_pos;
        if (best_pos == here) {
            result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        } else {
            result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 0, 0);
        }
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
    } else {
        result->position = order->position;
        result->kind = 6;
        result->loc = here;
        result->atkDefPosition = TaLogic_IsDefStyleUnit(order->position >> 8, order->position & 0xff, result->loc, 1, 0);
        result->shouldFlipUp = TaLogic_IsOpenUnit__00126380(order->position >> 8, order->position & 0xff, order);
    }
    func_00126940_mode(order, mode);
    return 0;
}

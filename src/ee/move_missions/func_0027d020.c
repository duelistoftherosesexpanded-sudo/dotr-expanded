#include "dotr/ee/move_missions.h"
#define SPOT (m == 1 ? ((unsigned)pUnit->flags.word >> 16) & 0xffff : pUnit->flags.half)
#define LO(a, b) ((a) < (b) ? (a) : (b))
#define HI(a, b) ((b) < (a) ? (a) : (b))
int TaLogic_GetMinimumTask_EncMission(struct DotrRuleOrder *pUnit, int nSide, struct DotrRuleResult *pTask, unsigned char Is1st, unsigned char IsView, int WarnLvl, int unused, unsigned char IsWarnHide)
{
    struct DotrRuleNibbles pntNext[12];
    int Loc;
    int count;
    int k;
    int Loc2;
    int v;
    struct DotrRuleNibbles *q;
    int MoveCand;
    int m;
    struct DotrRuleNibbles *p;
    int i;
    (void)unused;
    Loc = SzDuel_GetUnitLoc(pUnit->position >> 8, pUnit->position & 0xff);
    /* MATCHING DEVICE: the copy puts Is1st in s1; the calls still read the stored byte. */
    m = Is1st;
    Loc2 = SzDuel_GetUnitLoc(SPOT >> 8, SPOT & 0xff);
    if (SPOT == 0xffff) {
        if ((unsigned)Loc == SPOT) {
            pTask->position = pUnit->position;
            pTask->kind = 6;
            pTask->loc = Loc;
            pTask->atkDefPosition = 2;
            pTask->shouldFlipUp = 2;
            func_0027b1b0_mode(pUnit, Is1st);
            return 0;
        }
    }
    p = pntNext;
    for (k = 0, q = p; k < 12; k++, q++) {
        q->low = q->high = 15;
    }
    func_0027a090_rule(pUnit->position >> 8, pUnit->position & 0xff, pntNext, Loc & 0xff, Loc >> 8, Loc2 & 0xff, Loc2 >> 8, pUnit->flags.word, IsView, 3, WarnLvl & 0xffff);
    count = func_0027afc0_rule(pntNext);
    if (count > 0) {
        MoveCand = 255;
        WarnLvl = 0xffff;
        for (i = 0; i < count; i++, p++) {
            if (SzDuel_GetMapUnit(p->low, p->high) == 0) {
                v = func_00276cc0_enc(p->low, p->high, nSide, WarnLvl, IsWarnHide, WarnLvl);
                if (v < WarnLvl) {
                    WarnLvl = v;
                    MoveCand = i;
                } else if (v == WarnLvl) {
                    if ((HI(p->low, SPOT & 0xff) - LO(p->low, SPOT & 0xff)) +
                        (HI(p->high, SPOT >> 8) - LO(p->high, SPOT >> 8)) <
                        (HI(pntNext[MoveCand].low, SPOT & 0xff) - LO(pntNext[MoveCand].low, SPOT & 0xff)) +
                        (HI(pntNext[MoveCand].high, SPOT >> 8) - LO(pntNext[MoveCand].high, SPOT >> 8))) {
                        WarnLvl = v;
                        MoveCand = i;
                    }
                }
            }
        }
    } else {
        pTask->position = pUnit->position;
        pTask->kind = 6;
        pTask->loc = Loc;
        pTask->atkDefPosition = 2;
        pTask->shouldFlipUp = 2;
        func_0027b1b0_mode(pUnit, Is1st);
        return 0;
    }
    if (MoveCand != 255) {
        pTask->position = pUnit->position;
        pTask->kind = 6;
        pTask->loc = pntNext[MoveCand].high << 8 | pntNext[MoveCand].low;
        pTask->atkDefPosition = 2;
        pTask->shouldFlipUp = 2;
    } else {
        pTask->position = pUnit->position;
        pTask->kind = 6;
        pTask->loc = Loc;
        pTask->atkDefPosition = 2;
        pTask->shouldFlipUp = 2;
    }
    func_0027b1b0_mode(pUnit, Is1st);
    return 0;
}

#include "dotr/ee/step_picks.h"

int TaLogic_GetMinimumTask_Summon__00143ba0(int position, int side, struct DotrStepOut *minTask)
{
    struct DotrStepBlock block;
    int s;
    int r;
    int unitLoc;
    int kind;

    minTask->position = position;
    if (SzDuel_InqHandPhase(side) == 0) {
        return -1;
    }
    if (TaLogic_GetUnitNum(side, 0) == 5 && TaLogic_GetUnitNum(side, 1) == 5) {
        return -1;
    }
    kind = func_00143de0_step(side, &block);
    if (kind < 0) {
        return -1;
    }
    minTask->kind = 10; /* 10: Summon task */
    minTask->shouldFlipUp = 0; /* 0: face down */
    unitLoc = SzDuel_GetUnitLoc(position >> 8, position & 0xff);
    s = TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_001461c0_step(side));
    if (kind == 0) {
        if (func_00144450_step(side, minTask, &block) < 0) {
            goto rest;
        }
        if (s < 0) {
            if (SzDuel_GetUnitMonsterCount(side) >= 4) {
                goto rest;
            }
            r = func_00144a60_step(side, 0, &block, s);
            if (r < 0) {
                return -1;
            }
            minTask->loc = r;
            return 1;
        }
        r = func_00144a60_step(side, 0, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    } else if (kind == 2) {
        func_00144860_step(side, minTask, &block);
        r = func_00144a60_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    }
rest:
    r = func_00144250_step(side, minTask, &block, block.unknown_3c);
    if (r < 0) {
        return -1;
    }
    r = func_00144a60_step(side, r, &block, -1);
    if (r < 0) {
        return -1;
    }
    minTask->loc = r;
    return 1;
}

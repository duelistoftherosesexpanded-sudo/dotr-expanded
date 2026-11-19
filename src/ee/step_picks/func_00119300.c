#include "dotr/ee/step_picks.h"

int TaLogic_GetMinimumTask_Summon__00119300(int position, int side, struct DotrStepOut *minTask)
{
    struct DotrStepBlock block;
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
    if ((kind = func_00119a40_step(side, &block)) < 0) {
        return -1;
    }
    minTask->kind = 10; /* 10: Summon task */
    minTask->shouldFlipUp = 0; /* 0: face down */
    unitLoc = SzDuel_GetUnitLoc(position >> 8, position & 0xff);
    if (kind == 0) {
        if (func_0011a0d0_step(side, minTask, &block) < 0) {
            goto rest;
        }
        if (TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_0011aa10_step(side)) < 0) {
            if (SzDuel_GetUnitMonsterCount(side) >= 6) {
                goto rest;
            }
            r = func_00119520_step(side, kind, &block);
            if (r < 0) {
                return -1;
            }
            minTask->loc = r;
            return 1;
        }
        r = func_00119520_step(side, kind, &block);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    } else {
        TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_0011aa10_step(side));
    }
rest:
    if (func_00119d30_step(side, minTask, &block, block.unknown_3c) < 0) {
        return -1;
    }
    r = func_00119520_step(side, kind, &block);
    if (r < 0) {
        return -1;
    }
    minTask->loc = r;
    return 1;
}

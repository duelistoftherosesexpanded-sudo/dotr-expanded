#include "dotr/ee/step_picks.h"

int TaLogic_GetMinimumTask_Summon__00155770(int position, int side, struct DotrStepOut *minTask)
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
    kind = func_00155a00_step(side, &block);
    if (kind < 0) {
        return -1;
    }
    minTask->kind = 10; /* 10: Summon task */
    minTask->shouldFlipUp = 0; /* 0: face down */
    unitLoc = SzDuel_GetUnitLoc(position >> 8, position & 0xff);
    s = TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_00158780_step(side));
    if (kind == 0) {
        if (func_001564e0_step(side, minTask, &block) < 0) {
            goto rest;
        }
        if (s < 0) {
            if (SzDuel_GetUnitMonsterCount(side) >= 6) {
                goto rest;
            }
            r = func_00156b40_step(side, kind, &block, s);
            if (r < 0) {
                return -1;
            }
            minTask->loc = r;
            return 1;
        }
        r = func_00156b40_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    }
    if (kind == 2) {
        func_00156910_step(side, minTask, &block);
        r = func_00156b40_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    }
    if (kind == 3) {
        func_00155c90_step(side, minTask, &block);
        r = func_00156b40_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    }
rest:
    if (func_001562e0_step(side, minTask, &block, block.unknown_3c) < 0) {
        return -1;
    }
    r = func_00156b40_step(side, kind, &block, s);
    if (r < 0) {
        return -1;
    }
    minTask->loc = r;
    return 1;
}

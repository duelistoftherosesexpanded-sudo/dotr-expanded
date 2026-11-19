#include "dotr/ee/step_picks.h"

int TaLogic_GetMinimumTask_Summon__00132460(int position, int side, struct DotrStepOut *minTask)
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
    kind = func_001326f0_step(side, &block);
    if (kind < 0) {
        return -1;
    }
    minTask->kind = 10; /* 10: Summon task */
    minTask->shouldFlipUp = 0; /* 0: face down */
    unitLoc = SzDuel_GetUnitLoc(position >> 8, position & 0xff);
    if (kind == 0) {
        if (func_00132d00_step(side, minTask, &block) < 0) {
            goto rest;
        }
        s = TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_00134ae0_step(side));
        if (s < 0) {
            if (SzDuel_GetUnitMonsterCount(side) >= 5) {
                goto rest;
            }
            r = func_00133320_step(side, kind, &block, s);
            if (r < 0) {
                return -1;
            }
            minTask->loc = r;
            return 1;
        }
        r = func_00133320_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    } else {
        s = TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_00134ae0_step(side));
    }
rest:
    if (kind == 2) {
        func_00133130_step(side, minTask, &block);
        r = func_00133320_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    }
    if (func_00132b00_step(side, minTask, &block, block.unknown_3c) < 0) {
        return -1;
    }
    r = func_00133320_step(side, kind, &block, s);
    if (r < 0) {
        return -1;
    }
    minTask->loc = r;
    return 1;
}

#include "dotr/ee/step_picks.h"

int TaLogic_GetMinimumTask_Summon__0012d270(int position, int side, struct DotrStepOut *minTask, int limit)
{
    struct DotrStepBlock block;
    int s;
    int kind;
    int column;
    int unitLoc;
    int row;
    int r;

    minTask->position = position;
    if (SzDuel_InqHandPhase(side) == 0) {
        return -1;
    }
    if (TaLogic_GetUnitNum(side, 0) == 5 && TaLogic_GetUnitNum(side, 1) == 5) {
        return -1;
    }
    kind = func_0012d520_step(side, &block, limit);
    if (kind < 0) {
        return -1;
    }
    minTask->kind = 10; /* 10: Summon task */
    minTask->shouldFlipUp = 0; /* 0: face down */
    row = position >> 8;
    column = position & 0xff;
    unitLoc = SzDuel_GetUnitLoc(row, column);
    s = TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_00130780_step(side));
    if (kind == 0) {
        if (func_0012dde0_step(side, minTask, &block) < 0) {
            goto rest;
        }
        unitLoc = SzDuel_GetUnitLoc(row, column);
        s = TaLogic_GetNearCard2(&block, unitLoc & 0xff, unitLoc >> 8, side == 0 ? 1 : 0, 3, 31, 2, 2, 0, func_00130780_step(side));
        if (s < 0) {
            if (SzDuel_GetUnitMonsterCount(side) >= 6) {
                goto rest;
            }
            r = func_0012e3f0_step(side, 0, &block, s);
            if (r < 0) {
                return -1;
            }
            minTask->loc = r;
            return 1;
        }
        r = func_0012e3f0_step(side, 0, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    } else if (kind == 2) {
        func_0012e1f0_step(side, minTask, &block);
        r = func_0012e3f0_step(side, kind, &block, s);
        if (r < 0) {
            return -1;
        }
        minTask->loc = r;
        return 1;
    }
rest:
    r = func_0012dbe0_step(side, minTask, &block, block.unknown_3c);
    if (r < 0) {
        return -1;
    }
    r = func_0012e3f0_step(side, r, &block, s);
    if (r < 0) {
        return -1;
    }
    minTask->loc = r;
    return 1;
}



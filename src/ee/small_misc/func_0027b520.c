#include "dotr/ee/small_misc.h"

static inline unsigned int other_cell(struct DotrMiscCellPair *pair, unsigned char mode)
{
    if (mode == 1) {
        return (pair->unknown_08.word >> 16) & 0xffff;
    }
    return pair->unknown_08.half;
}

/* Looks up the cell at +0 and the mode-selected cell at +8 through
 * SzDuel_GetUnitLoc, calls GetMoveRev for the first cell's item at its looked-up
 * position, and returns whether TaLogic_IsPassRoot answers 1 for the pair. The
 * selected cell is also passed by address. */
int TaLogic_IsAtkNowToGoal(struct DotrMiscCellPair *assignedUnit, unsigned char useSecondGoal)
{
    unsigned short atkTarget;
    int loc;
    int destLoc;
    int x;
    int y;

    atkTarget = other_cell(assignedUnit, useSecondGoal);
    loc = SzDuel_GetUnitLoc(assignedUnit->unknown_00 >> 8, assignedUnit->unknown_00 & 0xff);
    {
        unsigned int low = other_cell(assignedUnit, useSecondGoal);
        unsigned int high = other_cell(assignedUnit, useSecondGoal);

        destLoc = SzDuel_GetUnitLoc(high >> 8, low & 0xff);
    }
    func_002464f0_rule(func_001df540_word(assignedUnit->unknown_00 >> 8, assignedUnit->unknown_00 & 0xff), x = loc & 0xff, y = loc >> 8);
    if (TaLogic_IsPassRoot(assignedUnit->unknown_00 >> 8, assignedUnit->unknown_00 & 0xff, x, y, destLoc & 0xff, destLoc >> 8, &atkTarget) == 1) {
        return 1;
    }
    return 0;
}

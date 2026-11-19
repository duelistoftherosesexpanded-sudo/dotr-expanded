#include "dotr/ee/small_misc.h"

/* Fills a tagged record from a packed unit (side << 8 | slot): the unit, tag 6, its location
 * (SzDuel_GetUnitLoc) and a trailing 2. The second argument is not read. */
void TaLogic_GetMinimumTask_DoNone(unsigned short *unit, int unused, struct DotrMiscTaggedPair *task)
{
    (void)unused;
    task->unitOwnerId = *unit;
    task->taskType = 6;
    task->loc = SzDuel_GetUnitLoc(*unit >> 8, *unit & 0xff);
    task->atkDefPosition = 2;
}

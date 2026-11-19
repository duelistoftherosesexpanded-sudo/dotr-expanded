#include "dotr/ee/small_misc.h"

int TaLogic_GetMinGoalCount(struct DotrMiscNibbles *pathfinderBuffer)
{
    int count;
    int i;
    for (i = 0, count = 0; i < 12; i++) {
        if (pathfinderBuffer[i].col == 15) {
            continue;
        }
        if (pathfinderBuffer[i].row == 15) {
            continue;
        }
        count++;
    }
    return count;
}

#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlGoal(int a, int b, int value)
{
    int r = 0;
    if (SzDuel_SetUnitCtlGoalC(a, b, value)) {
        if (SzDuel_SetUnitCtlGoalM(a, b, value)) {
            r = 1;
        }
    }
    return r;
}

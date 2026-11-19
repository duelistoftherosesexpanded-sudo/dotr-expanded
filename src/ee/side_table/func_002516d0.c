#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

void TaUtil_DoOpen(int side1, int pos2, int mode)
{
    if (mode == 1) {
        SzDuel_SetUnitCtlGoal(side1, pos2, 128);
        SzDuel_SetUnitCtlRateC(side1, pos2, 1, SzDuel_GetUnitCtlRateC(side1, pos2));
        SzDuel_SetUnitCtlRateM(side1, pos2, 1, SzDuel_GetUnitCtlRateM(side1, pos2));
        SzDuel_SetUnitStatus(side1, pos2, 2, 1);
        SzDuel_SetUnitStatus(side1, pos2, 4, 1);
    } else {
        SzDuel_SetUnitCtlGoalM(side1, pos2, 0);
        SzDuel_SetUnitCtlRateM(side1, pos2, -1, SzDuel_GetUnitCtlRateM(side1, pos2));
        SzDuel_SetUnitStatus(side1, pos2, 2, 0);
        SzDuel_SetUnitStatus(side1, pos2, 4, 0);
    }
}

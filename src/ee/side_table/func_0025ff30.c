#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

void TaEff_RestoreStdRate(int side, int pos)
{
    SzDuel_SetUnitCtlModeC(side, pos, 1);
    if (SzDuel_GetUnitStatus(side, pos, 4) != 0 || pos == 0) {
        SzDuel_SetUnitCtlModeM(side, pos, 7);
        SzDuel_SetUnitCtlGoalM(side, pos, 128);
        SzDuel_SetUnitCtlRateM(side, pos, 1, 128);
    } else {
        SzDuel_SetUnitCtlModeM(side, pos, 7);
        SzDuel_SetUnitCtlGoalM(side, pos, 128);
        SzDuel_SetUnitCtlRateM(side, pos, 0, 0);
    }
    SzDuel_SetUnitCtlGoalC(side, pos, 128);
    SzDuel_SetUnitCtlRateC(side, pos, 1, 128);
}

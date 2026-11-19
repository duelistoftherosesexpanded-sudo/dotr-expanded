#include "dotr/ee/small_misc.h"

void Ta_UnitFade(int side, int pos, int dir, int value, int flags)
{
    int first;
    int second;

    SzEffect_SetBusyFlag2(side << 8 | pos, 0);
    first = SzDuel_GetUnitCtlRateC(side, pos);
    second = SzDuel_GetUnitCtlRateM(side, pos);
    if (pos != 0 && SzDuel_GetUnitKind(side, pos) >> 5 != 0) {
        flags = (flags >> 1 & 1) << 1;
    }
    if (dir > 0) {
        if ((flags & 1) && second < 128) {
            SzDuel_SetUnitCtlModeM(side, pos, value);
            SzDuel_SetUnitCtlGoalM(side, pos, 128);
            if (SzDuel_GetUnitStatus(side, pos, 4) || pos == 0) {
                SzDuel_SetUnitCtlRateM(side, pos, 1, -1);
            } else {
                SzDuel_SetUnitCtlRateM(side, pos, 0, -1);
            }
        }
        if ((flags >> 1 & 1) && first < 128) {
            SzDuel_SetUnitCtlModeC(side, pos, value);
            SzDuel_SetUnitCtlGoalC(side, pos, 128);
            SzDuel_SetUnitCtlRateC(side, pos, 1, -1);
        }
    } else {
        if ((flags & 1) && second > 0) {
            SzDuel_SetUnitCtlModeM(side, pos, value);
            SzDuel_SetUnitCtlGoalM(side, pos, 0);
            SzDuel_SetUnitCtlRateM(side, pos, -1, -1);
        }
        if ((flags >> 1 & 1) && first > 0) {
            SzDuel_SetUnitCtlModeC(side, pos, value);
            SzDuel_SetUnitCtlGoalC(side, pos, 0);
            SzDuel_SetUnitCtlRateC(side, pos, -1, -1);
        }
    }
}

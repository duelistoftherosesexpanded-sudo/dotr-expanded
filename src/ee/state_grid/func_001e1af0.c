#include "dotr/ee/state_grid.h"

int SzDuel_SetUnitCtlRate(int a, int b, int pair, int value)
{
    int r = 0;
    if (SzDuel_SetUnitCtlRateC(a, b, pair, value)) {
        if (SzDuel_SetUnitCtlRateM(a, b, pair, value)) {
            r = 1;
        }
    }
    return r;
}

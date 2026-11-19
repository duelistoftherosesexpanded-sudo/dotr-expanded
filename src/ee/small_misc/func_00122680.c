#include "dotr/ee/small_misc.h"

int TaLogic005_GetUnitOnCycleRoot(int a, int b)
{
    int lo = SzDuel_GetUnitLoc(a, b) & 0xff;
    int hi = SzDuel_GetUnitLoc(a, b) >> 8;

    if (lo >= 2 && lo <= 5 && hi == 0) {
        return 0;
    }
    if (lo == 6 && hi >= 0 && hi <= 3) {
        return 1;
    }
    if (lo > 4 && lo <= 6 && hi == 4) {
        return 2;
    }
    if (lo == 4 && hi >= 4 && hi <= 5) {
        return 3;
    }
    if (lo > 0 && lo <= 4 && hi == 6) {
        return 4;
    }
    if (lo == 0 && hi > 2 && hi <= 6) {
        return 5;
    }
    if (lo >= 0 && lo <= 1 && hi == 2) {
        return 6;
    }
    if (lo == 2 && hi > 0 && hi <= 2) {
        return 7;
    }
    return -1;
}

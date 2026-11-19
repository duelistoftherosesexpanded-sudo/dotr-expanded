#include "dotr/ee/small_misc.h"

struct DotrMiscLargeEntry *SzData_GetData(int side)
{
    return &gSzData_All[gSzData_bRoseFlag[0] == side ? 0 : 1];
}

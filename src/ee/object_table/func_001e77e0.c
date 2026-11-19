#include "dotr/ee/object_table.h"

void SzEvent_Init(int value)
{
    int i;
    SzView_SetStraightLight(0, 9, 1);
    for (i = 0; i < 84; i++) {
        SzModel_SetDrawFlag(i, 0);
    }
    for (i = 0; i < 40; i++) {
        SzModel_SetDrawFlag(i + 34, 1);
    }
    for (i = 0; i < 2; i++) {
        SzModel_SetDrawFlag(i + 30, 1);
    }
    if (value != gSzEvent_nSave[0]) {
        gSzEvent_nSave[0] = value;
    }
}

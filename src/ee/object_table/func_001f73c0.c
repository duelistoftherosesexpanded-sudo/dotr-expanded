#include "dotr/ee/object_table.h"

void SzEffect_StoneProc(void)
{
    int i;
    int j;
    int v;

    for (i = 0; i < 10; i++) {
        j = i + 20;
        v = SzModel_GetStoneRatio(j);
        if (v >= 0 && v < 255) {
            v++;
            SzModel_SetStoneRatio(j, (v < 255 ? v : 255) & 0xff);
        }
    }
    for (j = 0; j < 2; j++) {
        i = j + 30;
        v = SzModel_GetStoneRatio(i);
        if (v >= 0 && v < 255) {
            v++;
            SzModel_SetStoneRatio(i, (v < 255 ? v : 255) & 0xff);
        }
    }
}

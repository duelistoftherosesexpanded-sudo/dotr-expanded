#include "dotr/ee/object_runs.h"

int SzEvent_Draw_Pipe(int cursor)
{
    int i;
    int n;
    int second;
    struct DotrRunPair *e;

    if (SzModel_GetDrawFlag(34)) {
        if (gSzEvent_nSave[0] == 2) {
            e = CastleData;
            /* i only counts the pairs; the target keeps the unused counter. */
            for (i = 0; e->second >= 0; e++, i++) {
                struct DotrRunVector v;

                second = e->second + 34;
                n = e->first + 34;
                v = data_00292470;
                v.x = e->x;
                v.z = e->z;
                if (e->first >= 0 && SzModel_IsModelDisp(n))
                    cursor = SzModel_DrawWithOutCalc_Pipe(cursor, n, (DotrRunWord)&v, 0, 0, 0, 0);
                if (e->second >= 0 && SzModel_IsModelDisp(second))
                    cursor = SzModel_DrawWithOutCalc_Pipe(cursor, second, (DotrRunWord)&v, 0, 0, 0, 0);
            }
        } else {
            for (n = 0; n < 40; n++) {
                i = n + 34;
                if (SzModel_IsModelDisp(i))
                    cursor = SzModel_DrawWithOutCalc_Pipe(cursor, i, 0, 0, 0, 0, 0);
            }
        }
    }
    return cursor;
}

#include "dotr/ee/side_table.h"

int SzDuel_DrawStageAfter_Pipe(int cursor)
{
    int i;
    struct DotrSideVector v;

    if (SzModel_GetDrawFlag(0) == 0) {
        return cursor;
    }
    for (i = 3; i < 5; i++) {
        v = data_00291120;
        v.z = i == 4 ? 180.0f : -180.0f;
        if (SzModel_IsModelDisp(i)) {
            cursor = SzModel_ExSort_Pipe(cursor, i, &v, 0, 0, 0, 0);
            v.x *= -1.0f;
            cursor = SzModel_ExSort_Pipe(cursor, i, &v, 0, 0, 0, 0);
        }
    }
    return SzDuel_DrawLine_Pipe(cursor);
}

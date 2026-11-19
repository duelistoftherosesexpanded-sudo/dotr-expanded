#include "dotr/ee/object_table.h"

int SzModel_DrawWithOutCalc_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g)
{
    int old = SzModel_GetCalcFlag(modelId);
    SzModel_SetCalcFlag(modelId, 0);
    cursor = SzModel_Draw_Pipe(cursor, modelId, c, d, e, f, g);
    SzModel_SetCalcFlag(modelId, old);
    return cursor;
}

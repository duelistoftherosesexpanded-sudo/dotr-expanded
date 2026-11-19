#include "dotr/ee/object_frees.h"

int SzModel_Draw_Pipe(int cursor, int modelId, int c, void *d, int e, int f, int g)
{
    void (*callback)(void) = 0;
    struct DotrFreeObject *p;
    struct DotrFreeDesc *header;

    if (modelId < 0 || modelId >= 84)
        return cursor;
    p = gSzModel_ahModel_free[modelId];
    if (p == 0)
        return cursor;
    if (p->modelBaseData == 0)
        return cursor;
    header = p->unknown_13c;
    if (header == 0)
        return cursor;
    /* MATCHING DEVICE: three separate tests with gotos; `a == 4 || a == 5 || a == 6`
     * folds the last two into one range test. */
    if (p->type == 4)
        goto listed;
    if (p->type == 5)
        goto listed;
    if (p->type != 6)
        goto done;
listed:
    if (header->unknown_1c != 0)
        callback = lSzModel_Draw_DefFunc;
done:
    return SzModel_DrawWithFunc_Pipe(cursor, modelId, c, d, e, f, g, callback, 0);
}

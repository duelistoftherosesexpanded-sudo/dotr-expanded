#include "dotr/ee/object_table.h"

int SzModel_GetOverlayM(int modelId)
{
    struct DotrObject *p;
    struct DotrObjectLink *q;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[modelId];
    if (p == 0) {
        return 0;
    }
    q = p->modelBaseData;
    if (q == 0) {
        return 0;
    }
    if ((q->unknown_04 & 0x100) == 0) {
        return 0;
    }
    return p->OverlayM;
}

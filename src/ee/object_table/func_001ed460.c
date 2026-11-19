#include "dotr/ee/object_table.h"

void lSzModel_EffectCB(int modelId)
{
    struct DotrObject *p;

    if (modelId < 0 || modelId >= 84) {
        return;
    }
    p = gSzModel_ahModel[modelId];
    if (p != 0) {
        *(int *)&p->unknown_1e8[4] = -1;
    }
}

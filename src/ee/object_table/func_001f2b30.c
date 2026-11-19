#include "dotr/ee/object_table.h"

/* Same body as SzModel_IsModelStandby; this caller contains it inlined. */
static inline int nonnegative_1f2(int index)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    return p->modelId >= 0 ? 1 : 0;
}

int SzModel_GetRefNum(int index)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return -1;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return -1;
    }
    if (!nonnegative_1f2(index)) {
        return -1;
    }
    return p->refBoneNum;
}

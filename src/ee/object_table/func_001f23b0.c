#include "dotr/ee/object_table.h"

int SzModel_GetRotC(int index, struct DotrObjectVector4 *out)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    if (out) {
        *out = p->rotC;
    }
    return 1;
}

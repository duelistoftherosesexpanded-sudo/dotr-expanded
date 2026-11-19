#include "dotr/ee/object_table.h"

int SzModel_SetRotC(int index, struct DotrObjectVector4 *in)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    if (in) {
        p->rotC = *in;
    } else {
        p->rotC.x = 0.0f;
        p->rotC.y = 0.0f;
        p->rotC.z = 0.0f;
        p->rotC.unknown_w = 0.0f;
    }
    return 1;
}

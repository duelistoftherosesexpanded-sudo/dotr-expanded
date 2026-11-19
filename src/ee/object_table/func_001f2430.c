#include "dotr/ee/object_table.h"

int SzModel_SetRot(int index, struct DotrObjectVector4 *in)
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
        p->rot = *in;
    } else {
        p->rot.x = 0.0f;
        p->rot.y = 0.0f;
        p->rot.z = 0.0f;
        p->rot.unknown_w = 0.0f;
    }
    return 1;
}

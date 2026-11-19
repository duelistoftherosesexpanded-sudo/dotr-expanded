#include "dotr/ee/object_table.h"

int SzModel_SetScale(int index, struct DotrObjectVector4 *in)
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
        p->scale = *in;
    } else {
        p->scale.x = 1.0f;
        p->scale.y = 1.0f;
        p->scale.z = 1.0f;
        p->scale.unknown_w = 1.0f;
    }
    return 1;
}

#include "dotr/ee/object_table.h"

int SzModel_GetIntensityOnly(int index, float *out)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    *out = p->intensity.z;
    return 1;
}

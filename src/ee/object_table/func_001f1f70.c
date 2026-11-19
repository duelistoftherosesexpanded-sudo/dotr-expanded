#include "dotr/ee/object_table.h"

int SzModel_SetIntensityOnly(int index, float value)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    p->intensity.z = value;
    return 1;
}

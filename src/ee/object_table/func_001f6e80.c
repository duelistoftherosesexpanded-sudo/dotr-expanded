#include "dotr/ee/object_table.h"

void SzEffect_SetIntensityOnly(int kind, float value)
{
    int i;
    for (i = 0; i < 84; i++) {
        if (kind < 0 || SzModel_GetModelType(i) == kind) {
            SzModel_SetIntensityOnly(i, value);
        }
    }
}

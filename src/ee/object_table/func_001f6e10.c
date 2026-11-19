#include "dotr/ee/object_table.h"

void SzEffect_SetIntensity(int targetModelType, struct DotrObjectVector3 *intensity)
{
    int i;
    for (i = 0; i < 84; i++) {
        if (targetModelType < 0 || SzModel_GetModelType(i) == targetModelType) {
            SzModel_SetIntensity(i, intensity);
        }
    }
}

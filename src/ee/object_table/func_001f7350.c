#include "dotr/ee/object_table.h"

void SzEffect_SetModelPauseForType(int targetModelType, int value)
{
    int i;

    for (i = 0; i < 84; i++) {
        if (targetModelType < 0 || targetModelType == SzModel_GetModelType(i)) {
            SzModel_SetPauseFlag(i, value);
        }
    }
}

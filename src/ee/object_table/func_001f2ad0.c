#include "dotr/ee/object_table.h"

int SzModel_SetDeformRatio(int index, int deformToSet)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    p->deformRatio = deformToSet;
    return 1;
}

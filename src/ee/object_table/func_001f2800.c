#include "dotr/ee/object_table.h"

int SzModel_SetAlpha(int index, int value)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    p->rgba.unknown_3 = value;
    return 1;
}

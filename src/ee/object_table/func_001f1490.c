#include "dotr/ee/object_table.h"

int SzModel_SetAnimParam(int index, int value, int slot)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    if (slot < 0 || slot >= 4) {
        return 0;
    }
    if (value < 0 || value >= 8) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    if (p->animationDataPointers[value] == 0) {
        return 0;
    }
    p->unknown_1e0[slot] = value;
    return 1;
}

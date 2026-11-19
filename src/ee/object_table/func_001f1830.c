#include "dotr/ee/object_table.h"

int SzModel_SetAnimDirect(int index, int slot)
{
    struct DotrObject *p;
    struct DotrObjectVector3 *a;
    signed char *previous, *current;
    float f, g;
    if (index < 0 || index >= 84) {
        return 0;
    }
    if (slot < 0 || slot >= 8) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    a = &p->unknown_198[0];
    previous = &p->unknown_1e0[1];
    current = &p->unknown_1e0[0];
    if (p->animationDataPointers[slot] != 0) {
        f = p->animationFrameCountArray[slot];
        g = f - 1.0f;
        *previous = -1;
        *current = slot;
        if (p->animDir.unknown_bits_6_7 == -1) {
            a->y = g;
        } else {
            a->y = 1.0f;
        }
        a->z = 1.0f;
        p->motionDelay = 0;
    } else {
        return 0;
    }
    return 1;
}

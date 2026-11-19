#include "dotr/ee/object_table.h"

int SzModel_SetRgba(int index, int value, struct DotrObjectQuad *in)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    if (value >= 0 && value < 4) {
        p->alphaMode.unknown_bits_0_1 = value;
    }
    if (in) {
        p->rgba = *in;
    } else {
        p->rgba.unknown_2 = 128;
        p->rgba.unknown_1 = 128;
        p->rgba.unknown_0 = 128;
        p->rgba.unknown_3 = 128;
    }
    return 1;
}

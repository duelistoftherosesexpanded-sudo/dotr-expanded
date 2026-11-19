#include "dotr/ee/object_table.h"

int SzModel_SetFilter(int index, int value, struct DotrObjectQuad *in)
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
        p->alphaMode.unknown_bits_2_3 = value;
    }
    if (in) {
        p->filters = *in;
    } else {
        p->filters.unknown_2 = 0;
        p->filters.unknown_1 = 0;
        p->filters.unknown_0 = 0;
        p->filters.unknown_3 = 0;
    }
    return 1;
}

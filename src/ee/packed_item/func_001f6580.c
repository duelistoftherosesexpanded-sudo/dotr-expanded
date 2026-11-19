#include "dotr/ee/packed_item.h"

int SzEffect_SetModelFilter2(int packed, struct DotrObjectQuad *q)
{
    return SzEffect_SetModelFilterX(packed, -1, q);
}

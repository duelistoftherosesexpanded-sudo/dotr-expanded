#include "dotr/ee/small_misc.h"

int Ta_GetLDRange(int side)
{
    return Ta_IsLDAbl(9, side) ? 5 : 3; /* 9: extended support range */
}

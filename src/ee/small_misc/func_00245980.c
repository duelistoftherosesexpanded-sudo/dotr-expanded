#include "dotr/ee/small_misc.h"

void ShAnime_GetPartsRect(unsigned char set, int row, struct DotrMiscHalfRow *out)
{
    *out = *(struct DotrMiscHalfRow *)(gpShFaceIndex[(unsigned char)set]->unknown_00 + (unsigned char)row * 8);
}

#include "dotr/ee/small_misc.h"

void ShAnime_GetPatInfo(int set, int row, struct DotrMiscByteRow *out)
{
    *out = *(struct DotrMiscByteRow *)(gpShFaceIndex[(unsigned char)set]->unknown_04 + (unsigned char)row * 8);
}

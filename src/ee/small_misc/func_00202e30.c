#include "dotr/ee/small_misc.h"

/* `/ 64 / 4` reproduces the two consecutive arithmetic shifts. */
int SzText_GetClutSize(struct DotrMiscFormat *imageHeader)
{
    int shift;
    int scale;
    int result;
    switch (imageHeader->unknown_40) {
    case 20: shift = 4; break;
    case 19: shift = 8; break;
    default: return 0;
    }
    switch (imageHeader->unknown_42) {
    case 0: scale = 4; break;
    case 2:
    case 10: scale = 2; break;
    default: return 0;
    }
    result = ((1 << shift) * scale) / 64 / 4;
    return result <= 0 ? 1 : result;
}

#include "dotr/ee/small_misc.h"

/* Moves one byte of a packed pair by `step`. Failing cases break to the
 * shared return; the upper byte uses a separate local, the lower one is
 * updated in place, and `> 6` selects the observed comparison register. */
int Ta_GetLocDistanceFromCenterOnRangeXY(int packed, int step, int mode)
{
    int low = packed & 0xff;
    int high = packed >> 8;
    int moved;
    switch (mode) {
    case 0:
        low -= step;
        if (low < 0) break;
        return high << 8 | low;
    case 1:
        moved = high - step;
        if (moved < 0) break;
        return low | moved << 8;
    case 2:
        low += step;
        if (low > 6) break;
        return high << 8 | low;
    case 3:
        moved = high + step;
        if (moved > 6) break;
        return low | moved << 8;
    default:
        return -1;
    }
    return -1;
}

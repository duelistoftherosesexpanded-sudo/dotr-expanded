#include "dotr/ee/small_misc.h"

/* Rebuilds x around bit position n using the masks Ta_ConvBitLengthToNumericLength returns:
 * with flag zero the bits above the position are shifted and merged with
 * the masked low part; otherwise the bits selected by the 11 - n mask are
 * replaced by those of the 10 - n mask. */
unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos)
{
    if (side == 0) {
        unsigned hi = (((flagList >> 11) >> (10 - pos)) & ~1) << (21 - pos);
        return hi | (flagList & Ta_ConvBitLengthToNumericLength(21 - pos));
    } else {
        unsigned m = flagList & ~Ta_ConvBitLengthToNumericLength(11 - pos);
        return m | (flagList & Ta_ConvBitLengthToNumericLength(10 - pos));
    }
}

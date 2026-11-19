#include "dotr/ee/small_misc.h"

unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos)
{
    if (teamId == 0) {
        return (flagList & 0x7ff) | (((flagList >> 11) | (1 << (10 - unitArrayPos))) << 11);
    }
    return flagList | (1 << (10 - unitArrayPos));
}

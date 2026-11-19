#include "dotr/ee/small_misc.h"

int Ta_GetAgreeOwnerIDon22bitFlagLst(int ownerId, unsigned flagList)
{
    if ((ownerId >> 8) == 0) {
        return ((flagList >> 11) >> (10 - (ownerId & 0xff))) & 1;
    }
    return (flagList >> (10 - (ownerId & 0xff))) & 1;
}

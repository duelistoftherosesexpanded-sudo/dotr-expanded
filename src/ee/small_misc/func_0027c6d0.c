#include "dotr/ee/small_misc.h"

/* ownerId is (side << 8) | lo. Returns 1 if the bit-0 query holds;
 * otherwise 0 only for kinds 3 and 4 whose bit-4 query holds, else 1. */
int TaLogic_IsPickUp(int ownerId)
{
    int lo = ownerId & 0xff;
    int side = ownerId >> 8;
    if (SzDuel_GetUnitStatus(side, lo, 0)) return 1;
    switch (SzDuel_GetUnitKind(side, lo) >> 5) {
    case 3:
    case 4:
        if (SzDuel_GetUnitStatus(side, lo, 4)) { return 0; }
        break;
    }
    return 1;
}

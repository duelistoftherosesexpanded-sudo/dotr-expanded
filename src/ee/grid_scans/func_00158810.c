#include "dotr/ee/grid_scans.h"

int TaLogic017_IsSetTrap(int side)
{
    if (SearchUnitByRange(0, 0, side, 1, 811 << 16 | 0x1c, 14) == 0) {
        if (TaLogic_GetGraveCardCount(side, 1, 811) == 0) {
            return 0;
        }
    }
    if (SearchUnitByRange(0, 0, side, 1, 805 << 16 | 0x1c, 14) == 0) {
        if (TaLogic_GetGraveCardCount(side, 1, 805) == 0) {
            return 0;
        }
    }
    if (SearchUnitByRange(0, 0, side, 1, 821 << 16 | 0x1c, 14) == 0) {
        if (TaLogic_GetGraveCardCount(side, 1, 821) == 0) {
            return 0;
        }
    }
    if (SearchUnitByRange(0, 0, side, 1, 824 << 16 | 0x1c, 14) == 0) {
        if (TaLogic_GetGraveCardCount(side, 1, 824) == 0) {
            return 0;
        }
    }
    return 1;
}

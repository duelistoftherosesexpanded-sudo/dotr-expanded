#include "dotr/ee/grid_scans.h"

int TaLogic017_GetEmptyExzPiece(void)
{
    int i;
    int spot;

    for (i = 0; i < 4; i++) {
        switch ((unsigned char)i) {
        case 0:
            spot = 0x303;
            break;
        case 1:
            spot = 0x404;
            break;
        case 2:
            spot = 0x402;
            break;
        case 3:
            spot = 0x503;
            break;
        default:
            spot = 0x503;
            break;
        }
        if (SzDuel_GetMapUnit(spot & 0xff, spot >> 8) == 0) {
            return spot;
        }
    }
    return -1;
}

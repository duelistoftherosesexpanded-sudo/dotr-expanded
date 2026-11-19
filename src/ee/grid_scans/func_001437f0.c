#include "dotr/ee/grid_scans.h"

int TaLogic012_GetLineUnitLst(unsigned short *out, int y, int side, int mask)
{
    int x;
    unsigned char n;
    int found;

    n = 0;
    for (x = 0; x < 7; x++) {
        if ((found = SzDuel_GetMapUnitID(x, y)) >= 0) {
            if ((mask & 1) == 1) {
                if (side == func_00250360_pair(found >> 8, found & 0xff)) {
                    out[n] = found;
                    n++;
                }
            }
            if (((mask >> 1) & 1) == 1) {
                if (side != func_00250360_pair(found >> 8, found & 0xff)) {
                    out[n] = found;
                    n++;
                }
            }
        }
    }
    return n;
}

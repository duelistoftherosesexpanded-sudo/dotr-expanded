#include "dotr/ee/lower_rules.h"
static inline int distance(int v)
{
    if (v >= 0) {
        return v;
    }
    return -v;
}
int TaLogic_GetQueueXY_Move1(int nSide, int nID, int nX, int nZ, int gX, int gZ)
{
    unsigned short gUnitID;
    int ret;
    int zlo, zhi, xlo, xhi, d;
    gUnitID = SzDuel_GetMapUnitID(gX, gZ);
    zlo = nZ < gZ ? nZ : gZ;
    zhi = nZ > gZ ? nZ : gZ;
    xlo = nX < gX ? nX : gX;
    xhi = nX > gX ? nX : gX;
    if ((xhi - xlo) + (zhi - zlo) == 1) {
        ret = 1;
    } else {
        d = distance(nX - gX);
        ret = 1;
        switch (d) {
        case 0:
            break;
        case 2:
            break;
        case 1:
            if (TaLogic_IsPassRoot(nSide, nID, nX, nZ, gX, nZ, &gUnitID) == 1) {
                ret = 1;
            } else {
                ret = 0;
            }
            break;
        }
    }
    return ret;
}

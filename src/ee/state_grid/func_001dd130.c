#include "dotr/ee/state_grid.h"

/* MATCHING DEVICE: the case order reproduces the original test and block
 * layout. */
struct DotrStateItem *SzDuel_GetCardInfo(int side, int pos)
{
    if (side < 0) {
        int k = SzModel_GetModelType(pos);
        if (k < 0) {
            return 0;
        }
        switch (k) {
        case 5:
            side = pos - 30;
            pos = 0;
            break;
        case 4: {
            int t = pos - 20;
            pos = t % 5 + 1;
            side = t < 5 ? 0 : 1;
        } break;
        default:
            return 0;
        }
    }
    if (side < 0 || side >= 3 || pos < 0 || pos >= 11) {
        return 0;
    }
    return &gSzDuel_aaVarInfo[side][pos];
}

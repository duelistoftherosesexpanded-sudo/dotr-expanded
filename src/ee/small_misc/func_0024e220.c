#include "dotr/ee/small_misc.h"

int Ta_GetSummonablePiece(int a, unsigned char flag)
{
    int i;
    int r;
    int x;
    int y;

    for (i = 0; i < 8; i++) {
        if ((r = Ta_GetSommonPiecePreference(a, i)) < 0) {
            continue;
        }
        if (flag == 0) {
            if (SzDuel_GetMapUnitID(r & 0xff, r >> 8) >= 0) {
                continue;
            }
            return r;
        } else {
            /* x is never read: the spelling that reproduces the saved registers. */
            x = r & 0xff;
            y = r >> 8;
#if !defined(__MWERKS__)
            (void)x;
#endif
            if (SzDuel_GetField(r & 0xff, y) == 8) {
                continue;
            }
            if (SzDuel_GetMapUnitID(r & 0xff, r >> 8) >= 0) {
                continue;
            }
            return r;
        }
    }
    return -1;
}

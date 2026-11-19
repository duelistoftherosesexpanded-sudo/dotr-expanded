#include "dotr/ee/small_misc.h"

int Ta_GetSummonablePieceWithOrder(int side, int idx, int avoidLabyrinth)
{
    int i;
    int n;
    int squareIndex;
    int x;
    int y;

    for (i = 0, n = 0; i < 8; i++) {
        if ((squareIndex = Ta_GetSommonPiecePreference(side, i)) < 0) {
            continue;
        }
        if ((avoidLabyrinth & 0xff) == 0) {
            if (SzDuel_GetMapUnitID(squareIndex & 0xff, squareIndex >> 8) >= 0) {
                continue;
            }
            if (idx == n) {
                return squareIndex;
            }
            n++;
        } else {
            /* x is never read: the spelling that reproduces the saved registers (as in
             * Ta_GetSummonablePiece). */
            x = squareIndex & 0xff;
            y = squareIndex >> 8;
#if !defined(__MWERKS__)
            (void)x;
#endif
            if (SzDuel_GetField(squareIndex & 0xff, y) == 8) { /* 8: labyrinth */
                continue;
            }
            if (SzDuel_GetMapUnitID(squareIndex & 0xff, squareIndex >> 8) >= 0) {
                continue;
            }
            if (idx == n) {
                return squareIndex;
            }
            n++;
        }
    }
    return -1;
}

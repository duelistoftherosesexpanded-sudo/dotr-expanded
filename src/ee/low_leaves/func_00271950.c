#include "dotr/ee/low_leaves.h"
int Ta_IsSquareRange(int nX, int nZ, int mX, int mZ, int R)
{
    int MinJ;
    int MinI;
    int j;
    int i;
    int MaxJ;
    int h;

    h = R >> 1;
    MinI = GetCap(0, nX - h);
    nX = nX + h >= 7 ? 6 : nX + h;
    MinJ = GetCap(0, nZ - h);
    MaxJ = nZ + h >= 7 ? 6 : nZ + h;
    for (i = MinI; i <= nX; i++) {
        for (j = MinJ; j <= MaxJ; j++) {
            if (i == mX && j == mZ) {
                return 1;
            }
        }
    }
    return 0;
}

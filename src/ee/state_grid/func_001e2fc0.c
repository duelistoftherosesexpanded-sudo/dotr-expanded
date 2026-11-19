#include "dotr/ee/state_grid.h"

void SzDuel_InitMapColor2(int a, int b, int c, int d)
{
    int x, y;
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            SzDuel_SetMapColor2(x, y, a, b, c, d);
        }
    }
}

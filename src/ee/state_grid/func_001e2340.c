#include "dotr/ee/state_grid.h"

void SzDuel_InitFieldRate(void)
{
    int x, y;
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            SzDuel_SetFieldRate(x, y, 0);
        }
    }
}

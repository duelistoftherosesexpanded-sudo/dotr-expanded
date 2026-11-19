#include "dotr/ee/small_misc.h"
void Ta_InitMazePiece(void)
{
    int x;
    int y;
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            if (SzDuel_GetField(x, y) == 8) {
                SzDuel_SetField(x, y, 7);
            }
        }
    }
}

#include "dotr/ee/board_reset.h"

int SzDuel_Initialize(void)
{
    struct DotrStateControl *control = data_00317490_control;
    int x;
    int y;
    struct DotrStateCell *cell;
    int i;
    int j;

    RhMem_FillMemory(data_00317490_control, 0, 3440);
    SzDuel_InitCardRequest();
    SzDuel_InitCardControl();
    for (y = 0; y < 7; y++) {
        for (x = 0; x < 7; x++) {
            cell = SzDuel_GetMapInfo(x, y);
            SzDuel_InitMapInfo(cell);
            cell->field = (y + x) % 10;
        }
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 11; j++) {
            SzDuel_InitCardInfo((struct DotrStateBlock *)&gSzDuel_aaVarInfo[i][j], -1);
        }
    }
    control->unknown_000[828] = 100;
    control->unknown_000[1676] = 100;
    return SzDuel_Init(0, 0, 0);
}

#include "dotr/ee/object_table.h"

/* MATCHING DEVICE: the lookup's answer is kept in the first parameter, as in the original. */
int SzEffect_SetModelPause2(int cell, int value)
{
    int row = cell >> 8;
    int column = cell & 0xff;

    if (row < 0 || row >= 3 || column < 0 || column >= 11)
        return 0;
    cell = SzDuel_GetUnitModelID(row, column);
    if (cell < 0)
        return 0;
    return SzModel_SetPauseFlag(cell, value);
}

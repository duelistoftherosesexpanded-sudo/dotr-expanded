#include "dotr/ee/state_grid.h"

void lSzBattle_CameraCB(struct DotrStateLoadRecord *record, int completed)
{
    int *words = gSzBattle_bl;

    words[2] = completed ? record->unknown_14 : 0;
}

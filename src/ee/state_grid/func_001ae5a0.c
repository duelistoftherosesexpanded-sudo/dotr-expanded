#include "dotr/ee/state_grid.h"

int SzDuel_DrawStage_Pipe(int a)
{
    if (SzModel_GetDrawFlag(0)) {
        return lSzDuel_DrawStage_Pipe(a);
    }
    return a;
}

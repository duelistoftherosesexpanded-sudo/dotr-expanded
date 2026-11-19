#include "dotr/ee/state_grid.h"

int SzBattle_DrawStage_Pipe(int a)
{
    if (SzModel_GetDrawFlag(8)) {
        return lSzBattle_DrawStage_Pipe(a);
    }
    return a;
}

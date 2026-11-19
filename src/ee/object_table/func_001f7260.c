#include "dotr/ee/object_table.h"

void SzEffect_SetPauseFor3DBattle(int pauseFlagValue)
{
    SzEffect_SetModelPauseForType(4, pauseFlagValue);
    SzEffect_SetModelPauseForType(5, pauseFlagValue);
    SzEffect_SetModelPauseForType(2, pauseFlagValue);
    SzEffect_SetModelPauseForType(3, pauseFlagValue);
    SzView_PauseRequest(-1, -1, pauseFlagValue);
}

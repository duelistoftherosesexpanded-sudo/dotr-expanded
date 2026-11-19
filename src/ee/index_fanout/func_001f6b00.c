#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogParamYForBattle(int value)
{
    SzView_SetFogParamY(0, 2, value);
    SzView_SetFogParamY(0, 3, value);
}

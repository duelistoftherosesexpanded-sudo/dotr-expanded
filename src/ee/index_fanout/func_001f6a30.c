#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogParamZForBattle(int value)
{
    SzView_SetFogParamZ(0, 2, value);
    SzView_SetFogParamZ(0, 3, value);
}

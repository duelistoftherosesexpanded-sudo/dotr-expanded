#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogParamZForDuel(int value)
{
    SzView_SetFogParamZ(0, 0, value);
    SzView_SetFogParamZ(0, 1, value);
    SzView_SetFogParamZ(0, 8, value);
}

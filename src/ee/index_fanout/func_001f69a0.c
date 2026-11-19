#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogParamZForMonster(int value)
{
    SzView_SetFogParamZ(0, 4, value);
    SzView_SetFogParamZ(0, 5, value);
}

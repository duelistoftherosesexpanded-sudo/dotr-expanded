#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogParamYForMonster(int value)
{
    SzView_SetFogParamY(0, 4, value);
    SzView_SetFogParamY(0, 5, value);
}

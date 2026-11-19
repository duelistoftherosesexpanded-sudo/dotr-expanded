#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogParamYForDuel(int value)
{
    SzView_SetFogParamY(0, 0, value);
    SzView_SetFogParamY(0, 1, value);
    SzView_SetFogParamY(0, 8, value);
}

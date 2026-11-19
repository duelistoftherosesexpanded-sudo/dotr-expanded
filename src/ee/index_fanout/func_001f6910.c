#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogColorForDuel(int value)
{
    SzView_SetFogColor(0, 0, value);
    SzView_SetFogColor(0, 1, value);
    SzView_SetFogColor(0, 8, value);
}

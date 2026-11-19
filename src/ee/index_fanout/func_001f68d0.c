#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogColorForMonster(int fogColour)
{
    SzView_SetFogColor(0, 4, fogColour);
    SzView_SetFogColor(0, 5, fogColour);
}

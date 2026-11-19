#include "dotr/ee/index_fanout.h"

void SzEffect_SetFogColorForBattle(int value)
{
    SzView_SetFogColor(0, 2, value);
    SzView_SetFogColor(0, 3, value);
}

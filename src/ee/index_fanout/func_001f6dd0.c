#include "dotr/ee/index_fanout.h"

void SzEffect_SetIntensityForBattle(int value)
{
    SzEffect_SetIntensity(2, value);
    SzEffect_SetIntensity(3, value);
}

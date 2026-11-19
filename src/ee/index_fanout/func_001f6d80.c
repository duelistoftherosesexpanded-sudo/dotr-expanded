#include "dotr/ee/index_fanout.h"

void SzEffect_SetIntensityForDuel(int value)
{
    SzEffect_SetIntensity(0, value);
    SzEffect_SetIntensity(1, value);
    SzEffect_SetIntensity(8, value);
}

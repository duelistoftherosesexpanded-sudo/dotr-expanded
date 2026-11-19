#include "dotr/ee/index_fanout.h"

void SzEffect_SetStraightLightForDuel(int value)
{
    SzView_SetStraightLight(0, 0, value);
    SzView_SetStraightLight(0, 1, value);
    SzView_SetStraightLight(0, 8, value);
}

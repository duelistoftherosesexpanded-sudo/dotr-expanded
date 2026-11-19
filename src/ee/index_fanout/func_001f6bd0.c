#include "dotr/ee/index_fanout.h"

void SzEffect_SetStraightLightForBattle(int pointLightParameters)
{
    SzView_SetStraightLight(0, 2, pointLightParameters);
    SzView_SetStraightLight(0, 3, pointLightParameters);
}

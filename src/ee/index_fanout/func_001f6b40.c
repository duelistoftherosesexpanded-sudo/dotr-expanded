#include "dotr/ee/index_fanout.h"

void SzEffect_SetStraightLightForMonster(int pointLightParameters)
{
    SzView_SetStraightLight(0, 4, pointLightParameters);
    SzView_SetStraightLight(0, 5, pointLightParameters);
}

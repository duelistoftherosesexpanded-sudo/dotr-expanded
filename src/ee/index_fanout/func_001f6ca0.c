#include "dotr/ee/index_fanout.h"

void SzEffect_SetPointLightForBattle(int pointLightParameters)
{
    SzView_SetPointLight(0, 2, pointLightParameters);
    SzView_SetPointLight(0, 3, pointLightParameters);
}

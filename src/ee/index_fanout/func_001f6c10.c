#include "dotr/ee/index_fanout.h"

void SzEffect_SetPointLightForMonster(int pointLightParameters)
{
    SzView_SetPointLight(0, 4, pointLightParameters);
    SzView_SetPointLight(0, 5, pointLightParameters);
}

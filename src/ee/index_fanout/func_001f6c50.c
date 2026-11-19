#include "dotr/ee/index_fanout.h"

void SzEffect_SetPointLightForDuel(int pointLightParameters)
{
    SzView_SetPointLight(0, 0, pointLightParameters);
    SzView_SetPointLight(0, 1, pointLightParameters);
    SzView_SetPointLight(0, 8, pointLightParameters);
}

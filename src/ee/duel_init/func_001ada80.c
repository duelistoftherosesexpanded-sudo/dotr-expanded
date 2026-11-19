#include "dotr/ee/duel_init.h"

/* The casts on the five address assignments are needed: without one the compiler folds the
 * address into each use instead of keeping it in a saved register as the original does. */
void lSzDuel_InitCom(int flag)
{
    int mode;
    int i;
    DotrInitWord intensityDuel;
    DotrInitWord intensityMonster;
    DotrInitWord light;
    float *fogY;
    DotrInitWord fogColor;

    mode = SzGame_GetMode(1);
    SzView_SetStraightLight(0, 9, 1);
    for (i = 0; i < 84; i++) {
        SzModel_SetDrawFlag(i, 0);
    }
    for (i = 0; i < 5; i++) {
        SzModel_SetDrawFlag(i, 1);
    }
    for (i = 0; i < 2; i++) {
        SzModel_SetDrawFlag(i + 5, 1);
    }
    for (i = 0; i < 10; i++) {
        SzModel_SetDrawFlag(i + 20, 1);
    }
    for (i = 0; i < 2; i++) {
        SzModel_SetDrawFlag(i + 30, 1);
    }
    for (i = 0; i < 2; i++) {
        SzModel_SetDrawFlag(i + 32, 1);
    }
    for (i = 0; i < 10; i++) {
        SzModel_SetDrawFlag(i + 74, 1);
    }
    if (mode == 1) {
        for (i = 0; i < 10; i++) {
            SzModel_SetDeform(i + 20, 1);
            SzModel_SetDeformRatio(i + 20, 32);
        }
        for (i = 0; i < 2; i++) {
            SzModel_SetDeform(i + 30, 1);
            SzModel_SetDeformRatio(i + 30, 32);
        }
    }
    if (flag) {
        intensityDuel = (DotrInitWord)gSzDuel_StageParam;
        intensityMonster = (DotrInitWord)data_0029100c;
        light = (DotrInitWord)data_00291020;
        fogY = (float *)data_002910b0;
        fogColor = (DotrInitWord)data_002910b8;
        SzEffect_SetFogColorForMonster(fogColor);
        SzEffect_SetFogColorForDuel(fogColor);
        SzEffect_SetFogParamZForMonster(0);
        SzEffect_SetFogParamZForDuel(0);
        if (fogY[0] == fogY[1]) {
            SzEffect_SetFogParamYForMonster(0);
            SzEffect_SetFogParamYForDuel(0);
        } else {
            SzEffect_SetFogParamYForMonster((DotrInitWord)fogY);
            SzEffect_SetFogParamYForDuel((DotrInitWord)fogY);
        }
        SzEffect_SetStraightLightForMonster(light);
        SzEffect_SetStraightLightForDuel(light);
        SzEffect_SetPointLightForMonster(0);
        SzEffect_SetPointLightForDuel(0);
        SzEffect_SetIntensityForMonster(intensityMonster);
        SzEffect_SetIntensityForDuel(intensityDuel);
        SzEffect_SetBlackOutForMonster(0x80);
        SzEffect_SetBlackOutForDuel(0x80);
    }
}

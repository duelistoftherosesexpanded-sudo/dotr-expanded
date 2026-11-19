#include "dotr/ee/battle_stage_draw.h"
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

/* lSzBattle_DrawStage_Pipe (0x1e55e0): animates the battle stage lighting and draws its models.
 * Compiled warm: the float-to-integer conversions are casts, apart from the explicit func_001a23b0
 * call. */
DotrStageWord lSzBattle_DrawStage_Pipe(DotrStageWord arg0) {
    struct DotrStageCamera camera;
    struct DotrStageVec3 ambient;
    struct DotrStageVec3 light;
    struct DotrStageCamera *stage;
    struct DotrStageCamera *toCamera;
    struct DotrStageCamera *fromCamera;
    struct DotrStageVec3 *toAmbient;
    struct DotrStageVec3 *fromAmbient;
    struct DotrStageVec3 *toLight;
    struct DotrStageVec3 *fromLight;
    float scale;
    float u;
    float t;
    int i;
    int id;
    int value;
    int id2;
    int j;

    scale = (unsigned char)SzEffect_GetBlackOutForBattle() / 128.0f;
    if (gSzBattle_nSave_stage == 7) {
        if (chRate_stage < 0x3C) {
            t = 0.0f;
        } else {
            t = (chRate_stage - 0x3C) / 4.0f;
        }
        u = 1.0f - t;
        toCamera = &gSzBattle_aStageParam_stage[10].camera;
        fromCamera = &gSzBattle_aStageParam_stage[7].camera;
        toAmbient = &gSzBattle_aStageParam_stage[10].ambient;
        fromAmbient = &gSzBattle_aStageParam_stage[7].ambient;
        toLight = &gSzBattle_aStageParam_stage[10].light;
        fromLight = &gSzBattle_aStageParam_stage[7].light;
        camera = data_002921f0_stage;
        camera.position[0] = toCamera->position[0] * t + fromCamera->position[0] * u;
        camera.position[1] = toCamera->position[1] * t + fromCamera->position[1] * u;
        camera.position[2] = toCamera->position[2] * t + fromCamera->position[2] * u;
        camera.position[3] = toCamera->position[3] * t + fromCamera->position[3] * u;
        camera.rotation[0] = (int)((float)toCamera->rotation[0] * t + (float)fromCamera->rotation[0] * u);
        camera.rotation[1] = (int)((float)toCamera->rotation[1] * t + (float)fromCamera->rotation[1] * u);
        camera.rotation[2] = (int)((float)toCamera->rotation[2] * t + (float)fromCamera->rotation[2] * u);
        camera.rotation[3] = (int)((float)toCamera->rotation[3] * t + (float)fromCamera->rotation[3] * u);
        camera.scale[0] = toCamera->scale[0] * t + fromCamera->scale[0] * u;
        camera.scale[1] = toCamera->scale[1] * t + fromCamera->scale[1] * u;
        camera.scale[2] = toCamera->scale[2] * t + fromCamera->scale[2] * u;
        camera.scale[3] = toCamera->scale[3] * t + fromCamera->scale[3] * u;
        ambient = data_00292280_stage;
        ambient.x = toAmbient->x * t + fromAmbient->x * u;
        ambient.y = toAmbient->y * t + fromAmbient->y * u;
        ambient.z = toAmbient->z * t + fromAmbient->z * u;
        light = data_00292290_stage;
        light.x = toLight->x * t + fromLight->x * u;
        light.y = toLight->y * t + fromLight->y * u;
        light.z = toLight->z * t + fromLight->z * u;
        SzModel_SetDrawFlag(0x10, t > 0.0f ? 1 : 0);
        SzEffect_SetStraightLightForMonster((DotrStageWord)&camera);
        SzEffect_SetStraightLightForBattle((DotrStageWord)&camera);
        SzEffect_SetIntensityForMonster((DotrStageWord)&ambient);
        SzEffect_SetIntensityForBattle((DotrStageWord)&light);
        if (SzModel_GetPauseFlag(8) == 0) {
            if ((int)(60.0f * (func_0019a5c8() / 2147483648.0f)) == 0) {
                chFlip_stage = 1;
            }
            chRate_stage += chFlip_stage;
            if (chRate_stage < 0) {
                chRate_stage = 0;
                chFlip_stage = 0;
            }
            if (chRate_stage < 0x20) {
                data_002921e0_stage = 1;
            }
            if (chRate_stage > 0x40) {
                chRate_stage = 0x40;
                chFlip_stage = -1;
                if (data_002921e0_stage != 0) {
                    if ((int)(2.0f * (func_0019a5c8() / 2147483648.0f)) != 0) {
                        SzSound_StereoPlaySE(0, 0x22, 0x23);
                    } else {
                        SzSound_StereoPlaySE(0, 0x24, 0x25);
                    }
                    data_002921e0_stage = 0;
                }
            }
        }
    }
    if (gSzBattle_nSave_stage == 5 || gSzBattle_nSave_stage == 6) {
        stage = &gSzBattle_aStageParam_stage[gSzBattle_nSave_stage].camera;
        if (SzModel_GetPauseFlag(8) == 0) {
            if (gSzBattle_nSave_stage == 6) {
                stage->rotation[1] = (stage->rotation[1] + 0x200) % 0x10000;
            } else {
                stage->rotation[2] = (stage->rotation[2] + 0x200) % 0x10000;
            }
        }
        SzEffect_SetStraightLightForMonster((DotrStageWord)stage);
        SzEffect_SetStraightLightForBattle((DotrStageWord)stage);
    }
    if (gSzBattle_nSave_stage == 8) {
        value = func_001a23b0(SzModel_GetAnimFrame(9));
        if (value == 0x14 || value == 0xDC) {
            SzSound_PlaySE(0, 0x28);
        }
    }
    SzEffect_SetIntensityOnly(2, scale);
    SzEffect_SetIntensityOnly(3, scale);
    for (i = 0; i < 12; i++) {
        id = i + 8;
        value = SzModel_GetModelNo(id) % 100;
        if (SzModel_IsModelDisp(id) != 0) {
            switch (gSzBattle_nSave_stage) {
            case 1:
                arg0 = lSzBattle_DrawWasteland_Pipe(arg0, id, value);
                break;
            case 3:
                arg0 = lSzBattle_DrawGrass_Pipe(arg0, id, value);
                break;
            case 4:
                arg0 = lSzBattle_DrawSea_Pipe(arg0, id, value);
                break;
            case 7:
                arg0 = lSzBattle_DrawNormal_Pipe(arg0, id, value);
                break;
            default:
                arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, id, 0, 0, 0, 0, 0);
                break;
            }
        }
    }
    for (j = 0; j < 1; j++) {
        id2 = j + 7;
        if (SzModel_IsModelDisp(id2) != 0) {
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, id2, 0, 0, 0, 0, 0);
        }
    }
    return arg0;
}

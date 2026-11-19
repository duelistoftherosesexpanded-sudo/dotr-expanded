#include "dotr/ee/battle_init.h"

void SzBattle_Init(int stage)
{
    struct DotrBattleBlock *bl;
    unsigned char *light;
    float *fog_y;
    int fog;
    unsigned char *fog_color;
    int i;
    struct DotrBattleStageParam *p;
    struct DotrBattleVec3 *intensity2;
    int j;
    int ofs;
    int size;
    char name[128];
    struct DotrBattleVec3 a;
    struct DotrBattleVec3 b;

    bl = (struct DotrBattleBlock *)gSzBattle_bl;
    p = &gSzBattle_aStageParam[stage];
    intensity2 = &gSzBattle_aStageParam[stage].intensity2;
    light = gSzBattle_aStageParam[stage].light;
    fog_y = gSzBattle_aStageParam[stage].fog_y;
    fog_color = gSzBattle_aStageParam[stage].fog_color;
    fog = stage == 4 ? 0 : 1;
    SzView_SetStraightLight(0, 9, 1);
    for (i = 0; i < 84; i++) {
        SzModel_SetDrawFlag(i, 0);
    }
    for (i = 0; i < 12; i++) {
        SzModel_SetDrawFlag(i + 8, 1);
    }
    for (i = 0; i < 1; i++) {
        SzModel_SetDrawFlag(i + 7, 1);
    }
    SzEffect_SetFogColorForMonster((DotrInitWord)fog_color);
    SzEffect_SetFogColorForBattle((DotrInitWord)fog_color);
    SzEffect_SetFogParamZForMonster(0);
    SzEffect_SetFogParamZForBattle(0);
    if (fog_y[0] == fog_y[1]) {
        SzEffect_SetFogParamYForMonster(0);
        SzEffect_SetFogParamYForBattle(0);
    } else {
        SzEffect_SetFogParamYForMonster((DotrInitWord)fog_y);
        SzEffect_SetFogParamYForBattle((DotrInitWord)fog_y);
    }
    SzEffect_SetStraightLightForMonster((DotrInitWord)light);
    SzEffect_SetStraightLightForBattle((DotrInitWord)light);
    SzEffect_SetPointLightForMonster(0);
    SzEffect_SetPointLightForBattle(0);
    SzEffect_SetIntensityForMonster((DotrInitWord)intensity2);
    SzEffect_SetIntensityForBattle((DotrInitWord)&p->intensity);
    SzEffect_SetBlackOutForMonster(128);
    SzEffect_SetBlackOutForBattle(128);
    for (j = 0; j < 12; j++) {
        SzModel_SetFogFlag(j + 8, fog);
    }
    if (stage == 4) {
        a = data_002921a0;
        a.x = p->intensity.x;
        a.y = p->intensity.y;
        b = data_002921b0;
        SzModel_SetIntensity(8, &a);
        SzModel_SetFogFlag(9, 1);
        SzModel_SetIntensity(12, &b);
    }
    if (stage == 2) {
        SzModel_SetFogFlag(9, 0);
    }
    if (stage != gSzBattle_nSave) {
        gSzBattle_nSave = stage;
    }
    if (bl->file != 0) {
        SzMem_Free(bl->file);
        bl->file = 0;
    }
    ofs = anOfs_STAGECAM[stage];
    size = anOfs_STAGECAM[stage + 1] - ofs;
    func_0019b648(name, data_002cf730, SzFile_GetEnv(data_002cf746));
    SzFile_ReadRequestParam(1024, 1, name, 0, size, ofs, size, lSzBattle_CameraCB, 0);
}

/* Evidence: docs/modules/game-modes.md. The callee prototypes, including their mixed
 * integer and float parameters, are assumed from the registers set at the calls.
 */
extern void SzDuel_InitMapColor(int, int, int);
extern void SzDuel_InitFieldRate(void);
extern void SzView_SetRoll(int, int);
extern void SzView_SetScreenDistance(int, float);
extern void SzView_SetCamera(int, int, int);
extern void SzView_RollCamera(int, int, int, float, float, float);
extern void SzView_SetFogParamZ(int, int, int);
extern void SzView_SetFogParamY(int, int, int);
extern void SzView_SetStraightLight(int, int, int);
extern void SzView_SetPointLight(int, int, int);
extern void SzModel_SetDeform(int, int);
extern void SzModel_SetDeformRatio(int, int);
extern void SzModel_SetDrawFlag(int, int);
extern void SzModel_SetScale(int, int);
extern void SzModel_SetRotC(int, int);
extern void SzModel_SetAnimDirect(int, int);
extern void SzModel_SetAnimDir(int, int);
extern void SzModel_SetAnimSpeed(int, float);
extern void SzModel_SetRgba(int, int, int);
extern void SzModel_SetFilter(int, int, int);
extern void SzModel_SetIntensity(int, int);
extern void SzEffect_SetBlackOutForMonster(int);
extern void SzEffect_SetBlackOutForDuel(int);
extern void SzEffect_SetBlackOutForBattle(int);

void SzGame_ComInit(void)
{
    int i;
    SzDuel_InitMapColor(0, 0, 1);
    SzDuel_InitFieldRate();
    SzView_SetRoll(0, 0);
    SzView_SetScreenDistance(0, 800.0f);
    SzView_SetCamera(0, 0, 0);
    SzView_RollCamera(0, 0, 1, 3.04341793060302734375f, 0.19634954631328582763671875f, 0.0f);
    for (i = 0; i < 10; ++i) {
        SzView_SetFogParamZ(0, i, 0);
        SzView_SetFogParamY(0, i, 0);
        SzView_SetStraightLight(0, i, 0);
        SzView_SetPointLight(0, i, 0);
    }
    for (i = 0; i < 10; ++i) {
        SzModel_SetDeform(i + 20, 0);
        SzModel_SetDeformRatio(i + 20, 0);
    }
    for (i = 0; i < 2; ++i) {
        SzModel_SetDeform(i + 30, 0);
        SzModel_SetDeformRatio(i + 30, 0);
    }
    for (i = 0; i < 84; ++i) {
        SzModel_SetDrawFlag(i, 0);
        SzModel_SetScale(i, 0);
        SzModel_SetRotC(i, 0);
        SzModel_SetDeform(i, 0);
        SzModel_SetDeformRatio(i, 0);
        SzModel_SetAnimDirect(i, 0);
        SzModel_SetAnimDir(i, 1);
        SzModel_SetAnimSpeed(i, 1.0f);
        SzModel_SetRgba(i, 0, 0);
        SzModel_SetFilter(i, 1, 0);
        SzModel_SetIntensity(i, 0);
    }
    SzEffect_SetBlackOutForMonster(128);
    SzEffect_SetBlackOutForDuel(128);
    SzEffect_SetBlackOutForBattle(128);
}

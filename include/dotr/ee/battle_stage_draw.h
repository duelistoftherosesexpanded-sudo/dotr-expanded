#ifndef DOTR_EE_BATTLE_STAGE_DRAW_H
#define DOTR_EE_BATTLE_STAGE_DRAW_H

/* The battle stage drawer: per-stage light and camera animation, then the stage models drawn by
 * stage kind.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/engine.md. */

/* The packet cursor and the light/camera addresses passed as integers are 32-bit words in
 * the game; a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrStageWord;
#else
typedef long DotrStageWord;
#endif

struct DotrStageVec3 {
    float x;
    float y;
    float z;
};

/* A light/camera record as the SzEffect_SetStraightLight* calls take it: position, rotation
 * words (0x10000 a full turn), scale, then 0x60 bytes not touched here. Assumed: field
 * meanings from their use here. */
struct DotrStageCamera {
    float position[4];
    int rotation[4];
    float scale[4];
    unsigned char pad30[0x60];
};

/* One stage's lighting record (0xc0 bytes): light colour, ambient colour, the camera. */
struct DotrStageRecord {
    struct DotrStageVec3 light;
    struct DotrStageVec3 ambient;
    unsigned char pad18[8];
    struct DotrStageCamera camera;
    unsigned char padB0[0x10];
};

/* The stage records by stage kind, the camera/ambient/light templates of the blended stage 7,
 * the current stage kind, and the state of stage 7's flicker: a frame counter (signed byte),
 * its step (signed byte) and a once-per-cycle sound flag. Assumed: names from their use. */
extern struct DotrStageRecord gSzBattle_aStageParam_stage[];
extern struct DotrStageCamera data_002921f0_stage;
extern struct DotrStageVec3 data_00292280_stage;
extern struct DotrStageVec3 data_00292290_stage;
extern int gSzBattle_nSave_stage;
extern signed char chRate_stage;
extern signed char chFlip_stage;
extern unsigned char data_002921e0_stage;

/* Callees (names from the Japanese build's symbols). Assumed: func_0019a5c8 returns a non-negative
 * random 31-bit integer; func_001a23b0 converts a float to an int; SzModel_GetPauseFlag is non-zero
 * while paused. */
extern int SzEffect_GetBlackOutForBattle(void);
extern int SzModel_SetDrawFlag(int flag, int on);
extern void SzEffect_SetStraightLightForMonster(DotrStageWord pointLightParameters);
extern void SzEffect_SetStraightLightForBattle(DotrStageWord pointLightParameters);
extern void SzEffect_SetIntensityForMonster(DotrStageWord intensityValue);
extern void SzEffect_SetIntensityForBattle(DotrStageWord light);
extern int SzModel_GetPauseFlag(int modelId);
extern int func_0019a5c8(void);
extern int SzSound_StereoPlaySE(int a0, int left, int right);
extern float SzModel_GetAnimFrame(int modelId);
extern int func_001a23b0(float value);
extern int SzSound_PlaySE(int a0, int soundEffectNumber);
extern void SzEffect_SetIntensityOnly(int index, float intensity);
extern int SzModel_GetModelNo(int modelId);
extern int SzModel_IsModelDisp(int modelId);
extern DotrStageWord lSzBattle_DrawWasteland_Pipe(DotrStageWord cursor, int model, int number);
extern DotrStageWord lSzBattle_DrawGrass_Pipe(DotrStageWord cursor, int model, int number);
extern DotrStageWord lSzBattle_DrawSea_Pipe(DotrStageWord cursor, int model, int number);
extern DotrStageWord lSzBattle_DrawNormal_Pipe(DotrStageWord cursor, int model, int number);
extern DotrStageWord SzModel_DrawWithOutCalc_Pipe(DotrStageWord cursor, int modelId, int a2, int a3, int a4, int a5,
                                                  int a6);

/* 0x1e55e0 lSzBattle_DrawStage_Pipe (name from the Japanese build's symbols): animates the
 * light and camera of the current stage kind (7: blended between records 7 and 10 by the
 * flicker counter; 5 and 6: turning), sets the black-out intensity, draws models 8..19 by
 * stage kind and model 7, and returns the new cursor. */
DotrStageWord lSzBattle_DrawStage_Pipe(DotrStageWord arg0);

#endif

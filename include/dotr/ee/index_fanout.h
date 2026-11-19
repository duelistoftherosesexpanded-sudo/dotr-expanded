#ifndef DOTR_EE_INDEX_FANOUT_H
#define DOTR_EE_INDEX_FANOUT_H

/* Wrappers that repeat one call for the index sets {4, 5}, {0, 1, 8} and
 * {2, 3}, and three byte accessors at 0x31bea0. Signatures of the called
 * routines are provisional. Evidence: docs/modules/effects.md. */
extern unsigned char data_0031bea0[];

void SzEffect_SetFogColorForMonster(int fogColour);
void SzEffect_SetFogColorForDuel(int value);
void SzEffect_SetFogColorForBattle(int value);
void SzEffect_SetFogParamZForMonster(int value);
void SzEffect_SetFogParamZForDuel(int value);
void SzEffect_SetFogParamZForBattle(int value);
void SzEffect_SetFogParamYForMonster(int value);
void SzEffect_SetFogParamYForDuel(int value);
void SzEffect_SetFogParamYForBattle(int value);
void SzEffect_SetStraightLightForMonster(int pointLightParameters);
void SzEffect_SetStraightLightForDuel(int value);
void SzEffect_SetStraightLightForBattle(int pointLightParameters);
void SzEffect_SetPointLightForMonster(int pointLightParameters);
void SzEffect_SetPointLightForDuel(int pointLightParameters);
void SzEffect_SetPointLightForBattle(int pointLightParameters);
void SzEffect_SetBlackOutForMonster(int value);
void SzEffect_SetBlackOutForDuel(int blackoutAmount);
void SzEffect_SetBlackOutForBattle(int blackOutValue);
int SzEffect_GetBlackOutForMonster(void);
int SzEffect_GetBlackOutForDuel(void);
int SzEffect_GetBlackOutForBattle(void);
void SzEffect_SetIntensityForMonster(int intensityValue);
void SzEffect_SetIntensityForDuel(int value);
void SzEffect_SetIntensityForBattle(int value);

/* Callee signatures, assumed from the call sites. */
extern void SzView_SetFogColor(int unknown, int index, int value);
extern void SzView_SetFogParamZ(int unknown, int index, int value);
extern void SzView_SetFogParamY(int unknown, int index, int value);
extern void SzView_SetStraightLight(int unknown, int index, int value);
extern void SzView_SetPointLight(int unknown, int index, int value);
extern void SzEffect_SetIntensity(int targetModelType, int intensity);

/* 0x27f890 steps the bytes at 0x31bea0 +0 and +1 up (dir > 0, while below
 * 128) or down (dir < 0, clamped at 0) by `step` through the accessors above
 * and returns 1 when neither byte could move. Parameter meanings assumed. */
int TaTuto_ControlFade(int step, int dir);

#endif

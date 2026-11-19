#ifndef DOTR_EE_EFF_109_H
#define DOTR_EE_EFF_109_H

/* Effect script 109: a sequence over four units of one side (src[2]) with cameras, effects and
 * model 0x20, which removes the four units at the end (Ta_Do_StandbyPhase requests it for a
 * complete Exzodia).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Addresses returned in 32-bit words in the game (SzDuel_GetUnit's unit record,
 * SzModel_GetCamera's camera); a host pointer needs a wider word. */
#ifdef __MWERKS__
typedef int DotrEff109Word;
#else
typedef long DotrEff109Word;
#endif

/* The request Ta_Do_Effect takes: effect command, value, cell and flag. */
struct DotrEff109Request {
    int command;
    int value;
    int cell;
    int flag;
};

/* The effect work area (arg1) as this script uses it: a fade byte at +0, the second step
 * counter at +4, effect handles from +6, effect commands from +0x20, the handle count at
 * +0xd4, the first step counter at +0x100 and a frame timer at +0x102. Assumed: field
 * meanings from their use here. */
struct DotrEff109State {
    unsigned short fade;
    unsigned char pad02[2];
    unsigned short phase;
    unsigned char handles[0x1A];
    int counters[0x2C];
    int timer2;
    unsigned char count;
    unsigned char padD5[0x2B];
    unsigned char stage;
    unsigned char pad101;
    unsigned short timer;
};

/* The leader unit record SzDuel_GetUnit returns: its card model request at +4 and its cell
 * (x, y) at +0x40/+0x41. Assumed: field meanings. */
struct DotrEff109Card {
    unsigned char pad00[4];
    unsigned char *model;
    unsigned char pad08[0x38];
    unsigned char x;
    unsigned char y;
};

struct DotrEff109Rot {
    int x;
    int y;
    int z;
    int w;
};

struct DotrEff109Vec4 {
    float x;
    float y;
    float z;
    float w;
};

/* Callees (names from the Japanese build's symbols): SzDuel_SetUnitCtlGoalC takes a signed char
 * goal, TaEffect_LoadModule a short module number, the two black-out getters return unsigned char.
 * Assumed: SzEffect_IsUse non-zero while an effect handle runs, SzEffect_GetStatus 2 when it has
 * finished its main part. */
extern void SzGame_SetAutoBlur(int level);
extern int SzDuel_FreeCardRequest(void *request);
extern unsigned char *SzDuel_GetCardRequest(void);
extern int SzDuel_GetTurnOwner(void);
extern void SzDuel_SetWindowDisp(int on);
extern void SzDuel_SetWindowDraw(int on);
extern DotrEff109Word SzDuel_GetUnit(int side, int pos);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern void SzDuel_CalcUnitPos(int x, int y, int a, void *out);
extern int SzDuel_SetUnitCtlGoalC(int side, int pos, signed char goal);
extern int SzDuel_SetUnitCtlRateC(int side, int index, int a, int b);
extern int SzDuel_SetUnitOpenRate(int side, int pos, int a, int rate);
extern void SzDuel_CloseMsgPopup(void);
extern int SzModel_Preset(int modelId, int modelType, int modelNumber);
extern int SzModel_Load(int modelId, int modelType, int modelNo, int b);
extern unsigned char *SzModel_GetEffTextureB(int modelId);
extern DotrEff109Word SzModel_GetCamera(int modelId, int index);
extern int SzModel_IsModelStandby(int model);
extern int SzModel_SetPos(int modelId, void *pos);
extern int SzModel_SetRot(int model, void *rot);
extern int SzModel_SetScale(int model, void *scale);
extern int SzEffect_IsUse(int handle);
extern int SzEffect_GetStatus(int handle);
extern int SzEffect_IsModelStandby2(int unit);
extern void SzEffect_SetBlackOutForMonster(int level);
extern void SzEffect_SetBlackOutForDuel(int blackoutAmount);
extern unsigned char SzEffect_GetBlackOutForMonster(void);
extern unsigned char SzEffect_GetBlackOutForDuel(void);
extern int SzView_FreeRequest(int a, int b);
extern int SzView_ReqCamera(int owner, int target, int type, int count, int value, void *data);
extern int SzText_AddFile(int group, int index, char *name, int srcImageAddress);
extern int SzText_FreeRequest(int group, int index);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern int SzSound_PlayBGM(int index, int flag);
extern int SzSound_IsLoadingBGM(void);
extern void Ta_UnitFade(int side, int index, int a, int b, int c);
extern void TaUtil_DoOpen(int side, int index, int flag);
extern void InitEffectTemp(void *state);
extern int Ta_Do_Effect(void *request, void *state);
extern int TaEffect_LoadModule(void *state, short moduleId);
extern int TaEffect_IsLoadModule(void *state);
extern void TaView_ReqFocusCamera(int a, int unit, int b, int c);
extern void TaView_ReqExzFocusCamera(int a, int unit, int b, int c);
extern void Ta109_BlackOutForExz(int direction);
extern int TaTuto_IsTuto(int side);

/* 0x26c170 Ta_Eff_109 (name from the Japanese build's symbols): src[1] the side, src[2] four
 * 3-bit unit indexes (bits 0..11), src[3] a flag. With arg2 >= 0 it sets the script up and
 * returns 1; later calls return 1 while running and 0 when done. */
int Ta_Eff_109(unsigned int *src, void *arg1, int arg2);

#endif

#ifndef DOTR_EE_EFF_009_H
#define DOTR_EE_EFF_009_H

/* The duel effect sequence Ta_Eff_009, a life-point effect with trap responses that steps
 * through camera, message and effect stages.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Views: the effect call (number, from, to, parameter) and the effect sequence's work (counters,
 * stage, effect ids, object count, camera stage and time). Member names follow the Japanese build's
 * debug information. Assumed: these meanings, from use. */
typedef struct DotrEff009Info {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
} DotrEff009Info;

typedef struct DotrEff009Temp {
    unsigned short wCount_1;
    unsigned short wCount_2;
    unsigned short wLvl;
    unsigned char eID[24];
    unsigned char pad1E[2];
    int ePos[24];
    unsigned char eMod[76];
    unsigned int fObj;
    unsigned int fObj2;
    unsigned char oNum;
    unsigned char padD5[11];
    unsigned char camDat[32];
    unsigned char bCamLvl;
    unsigned char pad101;
    unsigned short wCamTime;
} DotrEff009Temp;

/* Callees. SzDuel_GetUnitOpenRate is declared inside the function, without a prototype. */
extern int SzDuel_GetMsgName(int cardNo);
extern int SzDuel_GetWindowDisp(void);
extern void SzDuel_SetWindowDraw(int arg0);
extern int SzDuel_SetUnitStatus(int side, int pos, int arg2, int arg3);
extern int SzDuel_GetUnitStatus(int side, int pos, int arg2);
extern void SzDuel_OpenMsgPopup(int arg0, int arg1, int arg2, int arg3);
extern void SzDuel_CloseMsgPopup(void);
extern int SzEffect_IsUse(int arg0);
extern int SzEffect_SetCardColBusyFlag2(int arg0, int arg1);
extern int SzEffect_SetModelBusyFlag2(int arg0, int arg1);
extern void SzEffect_SetBlackOutForDuel(int blackoutAmount);
extern void TaUtil_DoOpen(int side1, int pos2, int arg2);
extern void InitEffectTemp(void *effectAnimationParams);
extern int Ta_Do_Effect(void *arg0, void *arg1);
extern int TaEffect_LoadModule(void *arg0, short moduleId);
extern int TaEffect_IsLoadModule(void *effectModule);
extern void Ta_CalcLPinEffectSeq(int side, int arg1, int arg2);
extern void TaView_RestoreOriginalView(void *arg0, int arg1);
extern void TaView_ReqFocusCamera(int arg0, int arg1, int arg2, int arg3);
extern void TaView_ReqBackFocusCamera(int arg0, int arg1, int arg2, int arg3);
extern void TaView_ReqFocusCamera2(int arg0, int arg1, int arg2);
extern void TaView_ReqFireView(int arg0, int arg1, unsigned char arg2, unsigned char arg3);
extern int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount);
extern int Ta_IsActiveTrapB(int side, int trapEffect);

/* 0x2594d0 Ta_Eff_009(info, temp, ID): with ID >= 0 sets the sequence up (blackout counter
 * 0x80, busy flags, modules 0x1fd / 0x25a / 0x223 by the opponent's trap 0x29 and the target's
 * trap 0x2e, camera); otherwise runs one frame of the stage in temp->wLvl. Returns 0 when the
 * sequence has finished, else 1 (also at once when the life-point change is 0). */
int Ta_Eff_009(void *effectAnimation, void *effectAnimationParams, int ID);

#endif

#ifndef DOTR_EE_HAND_EFFECTS_H
#define DOTR_EE_HAND_EFFECTS_H

/* Effect sequence steps at 0x26fe70-0x270258 that share the effect-call record, the effect
 * work block and the callees InitEffectTemp, TaEffect_LoadModule, Ta_Do_Effect and TaLib_c615
 * (original file SeqHand.c, from the Japanese build's debug information).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Member names and offsets from the Japanese build's debug information; the tags are
 * unnamed there. low_leaves.h declares the same call record as struct jp_11dc9. */
struct jp_8b329 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};

struct jp_8b5bb {
    int fLoad : 1;
    unsigned char bModNum : 5;
    unsigned short wModule[24];
    unsigned char bModCond[24];
};

/* 0x540 bytes in the original. camDat (+0xe0) and the members of nTasakiEfc
 * (struct jp_8b7ca, 0x430 bytes) are not used by this group and are kept as bytes. */
struct jp_8b3ce {
    unsigned short wCount_1;
    unsigned short wCount_2;
    unsigned short wLvl;
    unsigned char eID[24];
    int ePos[24];
    struct jp_8b5bb eMod;
    unsigned int fObj;
    unsigned int fObj2;
    unsigned char oNum;
    unsigned char unknown_d5[0x2b];
    unsigned char bCamLvl;
    unsigned short wCamTime;
    unsigned char unknown_104[0xc];
    int nTasakiEfc[0x10c];
};

/* Callee prototypes are assumed: the block pointer type, int results, and
 * for TaLib_c615 (0x27eb00, card_blur) three arguments; its int result is not used
 * here. */
extern void InitEffectTemp(struct jp_8b3ce *effectAnimationParams);
extern int TaEffect_LoadModule(struct jp_8b3ce *q, int moduleId);
extern int TaEffect_IsLoadModule(struct jp_8b3ce *effectModule);
extern int Ta_Do_Effect(struct jp_8b329 *p, struct jp_8b3ce *q);
extern int SzEffect_IsUse(int index);
extern int SzEffect_GetStatus(int index);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern int Ta_DelUnit2(int a, int b);
extern void SzDuel_CloseMsgPopup(void);
extern int TaLib_c615(void *efc, struct jp_8b329 *p, int n);
/* Callees of Ta_Eff_Hand_Force: signatures as their own groups declare them, with this
 * group's block pointer type where the block is passed. */
extern int Ta_GetEnchantOfs(void);
extern int Ta_GetForceEffection(int side, int b);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int SzDuel_GetTurnOwner(void);
extern int Ta_IsLDAbl(int ability, int side);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int SzEffect_SetBusyFlag2(int packed, int flag);
extern void TaUtil_DoOpen(int side1, int pos2, int mode);
extern void SzDuel_SetWindowDraw(int value);
extern void SzDuel_SetWindowBusy(int value);
extern int SzDuel_GetWindowDisp(void);
extern int SzDuel_GetUnitOpenRate(int side, int pos);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetMsgName(int cardNo);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value);
extern void TaView_ReqBackFocusCamera(unsigned char who, int packed, int add, int value);
extern void TaView_RestoreOriginalView(struct jp_8b3ce *Temp, unsigned char b);
extern int TaView_ReqFireView(int a, int b, unsigned char c, unsigned char d);
extern int Enchant(int side, int pos, int c, int teamId);

/* 0x26fe70. Prototype and locals from the Japanese build's debug information. */
int Ta_Eff_Hand_Fail(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID);
/* 0x270030. Prototype and locals from the Japanese build's debug information. */
int Ta_Eff_Hand_Fusion(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID);
/* 0x270c30. Prototype and the locals pCall, eCall, Unit2, Unit from the Japanese build's debug
 * information; the body differs there (its local temp is not needed here; `stored` is added
 * for the host build). */
int Ta_Eff_Hand_Force(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID);

/* Callees of Ta_Eff_Hand_Enc. Ta_GetEncEffect (0x249200, ta_values): an int argument
 * and an int result whose low 16 bits are used are assumed. The others as small_misc.h and
 * state_grid.h declare them. */
extern int Ta_GetEncEffect(int index);
extern int TaEnc_IsActReflexEnc(int index);
extern int SzDuel_GetUnitCtlRateC(int side, int pos);
extern int SzDuel_GetUnitCtlGoalC(int a, int b);
/* 0x270260. Prototype and the locals pCall, eCall, temp, Unit2, Unit from the Japanese build's
 * debug information; the body differs there. In this function wCount_1 holds a flag and
 * wCount_2 a packed unit position. */
int Ta_Eff_Hand_Enc(struct jp_8b329 *eInfo, struct jp_8b3ce *Temp, int ID);

#endif

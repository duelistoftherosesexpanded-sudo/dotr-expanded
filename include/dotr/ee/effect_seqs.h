#ifndef DOTR_EE_EFFECT_SEQS_H
#define DOTR_EE_EFFECT_SEQS_H

/* Effect sequence steps Ta_Eff_NNN (eInfo, Temp, ID) written with the effect-call record and
 * work block of the Japanese build's debug information: black-out fade in, module load, effects
 * started from the target flag list, fade out.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */
/* Member names and offsets from the Japanese build's debug information (Seq057.c); the
 * tags are unnamed there. The same layouts are declared as struct jp_8b329 / jp_8b3ce in
 * hand_effects.h and struct jp_11dc9 in low_leaves.h. */
struct jp_6989c {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};

struct jp_68cb4 {
    int fLoad : 1;
    unsigned char bModNum : 5;
    unsigned short wModule[24];
    unsigned char bModCond[24];
};

/* 0x540 bytes in the original. camDat (struct jp_68c4d, 0x20 bytes at +0xe0) and
 * nTasakiEfc (struct jp_68ec3, 0x430 bytes at +0x110) are not used by this group and are
 * kept as words; the unknown bytes stand for the 16-byte alignment of both. */
struct jp_69942 {
    unsigned short wCount_1;
    unsigned short wCount_2;
    unsigned short wLvl;
    unsigned char eID[24];
    int ePos[24];
    struct jp_68cb4 eMod;
    unsigned int fObj;
    unsigned int fObj2;
    unsigned char oNum;
    unsigned char unknown_d5[0xb];
    int camDat[8];
    unsigned char bCamLvl;
    unsigned short wCamTime;
    unsigned char unknown_104[0xc];
    int nTasakiEfc[0x10c];
};

/* Callee prototypes are assumed (from the argument registers and how results are
 * used) under this group's types; Ta_SetEffectDependOnFlagLst follows slot_orders.h. */
extern void SzDuel_SetWindowDraw(int value);
extern int SzDuel_GetWindowDisp(void);
extern void InitEffectTemp(struct jp_69942 *effectAnimationParams);
extern int TaEffect_LoadModule(struct jp_69942 *q, int moduleId);
extern int TaEffect_IsLoadModule(struct jp_69942 *effectModule);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern void TaView_ReqFocusCamera(unsigned char who, int packed, int add, int value);
extern void TaView_ReqStdCompleteView(unsigned char who);
extern void Ta_AllUnitFade(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID, int dir, int flag, int value);
extern void SzDuel_OpenMsgPopup(int operation, int a, int b, int value);
extern void Ta_SetEffectDependOnFlagLst(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall, int id);
extern void SzEffect_SetBlackOutForDuel(int blackoutAmount);
extern int SzEffect_IsUse(int index);
extern void SzDuel_CloseMsgPopup(void);
extern int TaUtil_CalcUnitDP(int a, int b, int op, int val);
extern int TaUtil_CalcUnitAP(int side, int pos, int operation, int val);
extern void TaView_RestoreOriginalView(struct jp_69942 *Temp, unsigned char b);
extern int Ta_IsFinishFade(void);
extern int SzDuel_GetMsgName(int cardNo);
extern int SzDuel_GetUnitDP(int side, int pos, int ignoreBoosts);
extern int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_AddDeck(int side, int cardNo);
extern void SzDuel_PackDeck(int side);
extern int SzDuel_DelUnit(int side, int unitArrayPos);
extern int Ta_Do_Effect(struct jp_6989c *eInfo, struct jp_69942 *Temp);
extern int SzEffect_GetStatus(int index);
extern int SzDuel_SetUnitKind(int side, int pos, int value);
extern int SzDuel_SetUnitAttr(int side, int pos, int value);

/* 0x268760. Prototype and local names from the Japanese build's debug information; the
 * Japanese body differs (changed for the US release), so the roles of tCount and i are
 * what this body shows. */
int Ta_Eff_057(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x25d230, 0x25d640, 0x25de70. Prototypes and local names from the Japanese build's debug
 * information (Seq029.c, Seq030.c); Ta_Eff_029's Japanese body differs. The two locals of
 * Ta_Eff_029_CNG that hold the point differences are named here from the body. */
int Ta_Eff_029_CNG(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
int Ta_Eff_029(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
int Ta_Eff_030(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x261760. Prototype and local names from the Japanese build's debug information. */
int Ta_Eff_017_MEGUMI(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x26d8f0. Prototype and local names from the Japanese build's debug information. */
int Ta_Eff_066(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x261b70. Prototype and local names from the Japanese build's debug information (the
 * Japanese body differs). */
int Ta_Eff_006_WIND(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x258490. Prototype and local names from the Japanese build's debug information (Seq006.c);
 * the callee prototypes are the ones their own groups declare. */
extern int Ta_DelUnit(int side, int pos);
extern void TaView_ReqRotCamera(unsigned char b, int delta);
int Ta_Eff_006(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x256ab0. Prototype and local names from the Japanese build's debug information (Seq003.c).
 * Ta_AllUnitFade2 is declared with the argument types this caller passes, as Ta_AllUnitFade is. */
extern void Ta_AllUnitFade2(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID, int dir, int flag, int value);
int Ta_Eff_003B(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x259d90. Prototype and local names from the Japanese build's debug information (Seq012.c);
 * the callee prototypes are the ones their own groups declare. */
void TaView_ReqTopView(void);
void TaUtil_DoOpen(int side1, int pos2, int mode);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern void Ta_SetUnitCurseRest(int a, int b, unsigned char n);
int Ta_Eff_012(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x26bf50. Prototype and local names from the Japanese build's debug information (Seq102.c; that
 * function's body differs from this one); the callee prototypes are the ones their own groups
 * declare. */
extern int SzDuel_GetUnitOpenRate(int side, int pos);
extern int SzEffect_SetBusyFlag2(int packed, int flag);
int Ta_Eff_102(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x263500. Prototype and local names from the Japanese build's debug information (Seq063.c);
 * the callee prototypes are the ones their own groups declare. */
extern int SzDuel_GetUnitLoc(int side, int pos);
extern struct jp_6a917 *SzDuel_AddUnit(int side, int col, int row, int cardNo, int e);
int Ta_Eff_063(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x26bb80. Prototype and the local names eCall, pCall, tCount, Unit and i from the Japanese build's
 * debug information (Seq056.c; that function's body differs from this one). The second counter `j`
 * of the level-0 loop is not in that list: the name is this project's. */
int Ta_Eff_056(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x268fa0. Prototype and local names from the Japanese build's debug information (Seq059.c; that
 * function's body differs from this one). tCount is declared first here: the debug order
 * (pCall, eCall, tCount, i) swaps two saved registers. */
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount);
int Ta_Eff_059(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x25aa80. Prototype and local names from the Japanese build's debug information (Seq015.c).
 * tCount and FinishCond are declared before eCall/pCall here: the debug order (pCall, eCall,
 * FinishCond, tCount, i) swaps two saved registers. */
extern int SzDuel_GetUnitKind(int side, int pos);
void TaView_ReqStdTopDownView(unsigned char flag);
int Ta_Eff_015(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x2560c0. Prototype and local names from the Japanese build's debug information (Seq000.c; the
 * Japanese body differs). tCount is declared before FinishCond here: the debug order (pCall, eCall,
 * FinishCond, tCount, i) swaps two saved registers. TaUtil_GetObjSideNum is passed the flag list. */
extern int TaUtil_GetObjSideNum(unsigned int dwObj);
extern int SzDuel_SetUnitSpellRest(int side, int pos, int value);
int Ta_Eff_000(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x263180. Prototype and local names from the Japanese build's debug information (Seq064.c; the
 * Japanese body differs). AP is declared before DP here: the debug order (pCall, eCall, Loc2, Loc,
 * DP, AP, i) swaps two saved registers in the own-side branch. The four readers take a unit
 * (side, slot) and a location split as (low byte, high part), as their own groups declare them. */
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int TaUtil_GetUnitDP(int side, int pos, int col, int row);
extern int TaUtil_GetUnitAP3(int side, int pos, int col, int row);
extern int TaUtil_GetUnitDP3(int side, int pos, int col, int row);
int Ta_Eff_064(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x25a060. Prototype and local names from the Japanese build's debug information (Seq013.c; the
 * Japanese body is the same apart from call targets). The locals are declared i, Num, tCount, Unit,
 * eCall, pCall here: the debug order (pCall, eCall, Unit, Num, tCount, i) permutes the saved
 * registers. `stored` is not in the debug information (see the source comment). */
int Ta_Eff_013(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x273460. Prototype and local names from the Japanese build's debug information (Seq112.c; the
 * Japanese body differs). tCount is declared first and Unit last here: the debug order (pCall,
 * eCall, Unit, tCount, i) swaps two saved registers. The three callees are declared as their
 * own groups declare them. */
extern int SzEffect_IsModelStandby2(int packed);
void TaEffect_SetOuchMotion(int packed);
extern void Ta_UnitFade(int side, int pos, int dir, int value, int flags);
int Ta_Eff_112(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* 0x25ef40. Local names from the Japanese build's debug information (Seq035.c, body the same
 * apart from call targets); that record lists only (Temp, ID) as parameters, but the code takes
 * them in the second and third argument registers, so the unused eInfo is assumed as in the other
 * sequences. The locals are declared i, Stack, nX, nZ, nSide, nID, Unit, Loc, eCall, pCall here:
 * the debug order (pCall, eCall, Loc, Unit, nID, nSide, Stack, nZ, nX, i) permutes the saved
 * registers. func_0019a5c8 (library block, no name placed) returns the next pseudo-random
 * number; SzDuel_MoveUnit is declared as small_misc.h has it. */
extern int func_0019a5c8(void);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int Ta_IsLDRangeAvilityWithLocation(int side, int unitArrayPos, int col, int row, int ability);
int Ta_GetStackMapPiece(void);
void Ta_InitMazePiece(void);
extern void SzDuel_MoveUnit(int a, int b, int c, int d);
int Ta_Eff_035(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_114 (0x27e840): prototype and the locals pCall, eCall from the Japanese build's debug
 * information (Seq114.c); the Japanese body differs (changed for the US release). The locals are
 * declared eCall, pCall here. The callee prototypes are the ones their own groups declare;
 * Ta_CalcCtrlRate is assumed to return the stepped counter. */
extern void SzDuel_CloseHandPopup(void);
extern void TaView_SetDestinyCamera(unsigned char flag);
extern int Ta_CalcCtrlRate(unsigned short value, int amount, int direction);
extern void SzEffect_SetBlackOutForBattle(int blackOutValue);
extern int SzSound_PlaySE(int index, int soundEffectNumber);
extern void TaView_ReqBackFocusCamera(unsigned char who, int packed, int add, int value);
int Ta_Eff_114(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_045 (0x265050): prototype and the locals pCall, eCall from the Japanese build's debug
 * information (Seq045.c); the Japanese body differs (changed for the US release). The locals are
 * declared eCall, pCall here. SzDuel_GetUnitCtlRateC as state_grid.h declares it. */
extern int SzDuel_GetUnitCtlRateC(int side, int pos);
int Ta_Eff_045(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_028 (0x25d9d0): prototype and the locals pCall, eCall, Unit, eObj, tCount and i from
 * the Japanese build's debug information (Seq028.c); the locals are declared in the reverse of
 * that order, which is what reproduces the saved-register assignment. Assumed: the six
 * arguments of SearchUnitByRange and that Ta_RemoveObjFromFlagLst2's result is unused. */
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_RemoveObjFromFlagLst2(int word, int side);
int Ta_Eff_028(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_113 (0x2738b0): prototype and the local names pCall, eCall, Loc, Num, tCount, Unit, i from
 * the Japanese build's debug information (that build's body differs; tCount is declared last here,
 * the order that reproduces the saved registers). Callees keep their own groups' prototypes under
 * this group's types; Ta_SetEffectDependOnFlagLst2/3 follow slot_orders.h. */
extern int SzDuel_GetTurnOwner(void);
extern int Ta_IsLDAbl(int ability, int side);
extern void Ta_SetRangeVisual(int x, int y, int c, int reach, int e, int mode, int visualType);
extern void Ta_SetEffectDependOnFlagLst2(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall, int eNO);
extern void Ta_SetEffectDependOnFlagLst3(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall, int eNO);
int Ta_Eff_113(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_002 (0x256520): prototype and the locals Buf, tCount, j, i from the Japanese build's debug
 * information (same body apart from call targets); eCall/pCall are named as in the other sequences.
 * Callees keep their own groups' prototypes. */
extern void TaView_ReqFocusCamera2(unsigned char b, int cell, int value);
extern int SzDuel_GetField(int x, int y);
int Ta_Eff_002(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_004 (0x257340): prototype and the local names pCall, eCall, FinishCond, eID, tCount, Unit2,
 * Unit, i from the Japanese build's debug information; the locals are declared in the reverse of
 * that list's order (the order that reproduces the saved registers). Ta_IsActiveTrapB's and
 * Ta_GetCurrentSide's prototypes are assumed as in slot_seeks.h / state_queries.h. */
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int Ta_GetCurrentSide(int side, int pos);
int Ta_Eff_004(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_025 (0x25c330): prototype and the local names pCall, eCall, FinishCond, Self, Loc, Unit,
 * tCount, i from the Japanese build's debug information (Seq025.c; that build's body differs);
 * tCount is declared before and FinishCond after Self, Loc, Unit here (the order that reproduces
 * the two stack slots). Ta_IsLDRangeAvility's and Ta_IsResistAll's prototypes are assumed as in
 * state_queries.h, TaView_ReqSkyView's as in preset_words.h. */
extern int Ta_IsLDRangeAvility(int side, int pos, int ability);
extern int Ta_IsResistAll(int side, int pos);
extern void TaView_ReqSkyView(void);
int Ta_Eff_025(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_020 (0x25b900): prototype and local names from the Japanese build's debug information
 * (Seq020.c); the locals are declared in the reverse of that order to reproduce the saved
 * registers. Ta_IsFinishDestEfc and TaView_ReqFlatCompleteView are declared with the argument
 * types this caller passes; func_00199b50 is the library formatter (printf-like by its
 * use, as in sound_file.h) and data_002b86e8 its format string, not decoded here. */
extern int Ta_IsFinishDestEfc(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall);
extern void TaView_ReqFlatCompleteView(unsigned char flag);
extern int SzDuel_GetUnitCtlGoalC(int a, int b);
extern int func_00199b50(const char *format, ...);
extern char data_002b86e8[];
int Ta_Eff_020(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);


/* Ta_Eff_001 (0x256d80): prototype and the locals pVarInfo, pCall, eCall, Num and i from the
 * Japanese build's debug information (Seq001.c). bVarKind, bOwner and bID of struct jp_6a917 are
 * the original member names at the offsets the code reads. */
extern int TaHand_GetCardKind(int id);
extern int SzDuel_GetUnitMonsterCount(int side);
extern int SzDuel_GetUnitSpellCount(int side);
int Ta_Eff_001(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_ETC (0x260200). Loads module 513, opens popup 260 and starts effect 513 on
 * eInfo->dwTo; the started effect's number is stored at eID[oNum] without advancing oNum, and
 * level 1 waits on eID[0] only. Parameter and local names follow the group's other steps, not a
 * recovered prototype. */
int Ta_Eff_ETC(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_072 (0x26cfc0): the gravity effect step around Ta_SetGravityEffect; wCount_2 is
 * the distance, stepped from 2 to 14. Local names follow the group (pCall, eCall, i);
 * SzGame_SetAutoBlur is declared with the one argument this caller passes (assumed). */
extern void SzGame_SetAutoBlur(int value);
int Ta_Eff_072(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

#endif

/* Ta_Eff_044 (0x265440): prototype and the locals pCall, eCall from the Japanese build's debug
 * information (Seq044.c); the Japanese body differs (changed for the US release). The locals are
 * declared eCall, pCall here. */
int Ta_Eff_044(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_ManageDestroyEffect (0x25f400): prototype and local names from the Japanese build's debug
 * information (SeqUtil.c, same body there); the locals are declared i, j, Unit, eObj here (the
 * reverse of the recorded order), the recorded local Ret is not used, and i is reused for the
 * unit of the second loop. `stored` only keeps the host build quiet (see the source). The four
 * callees as small_misc.h / state_grid.h declare them. */
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern int Ta_GetRandomUnit(int flagList);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_SetUnitCtlRate(int a, int b, int pair, int value);
int Ta_ManageDestroyEffect(struct jp_6989c *eInfo, struct jp_69942 *Temp, struct jp_6989c *pCall);
/* Ta_Eff_051 (0x26e1f0): prototype and the locals pCall, eCall, tCount, i from the Japanese build's debug
 * information (Seq051.c; that build's body differs). Declaring tCount first is assumed, as in Ta_Eff_059. */
int Ta_Eff_051(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
/* Ta_SetGravityEffect (0x26d3c0): prototype and the locals pCall, eCall, Reserve, eObj, Unit, Loc,
 * Num, i from the Japanese build's debug information (Seq072.c). Reserve is declared before eCall
 * here: the stack layout needs it. The three callees are declared as small_misc.h / state_queries.h
 * have them. */
extern int Ta_GetMinimumOrderUnitFromFlagList2(int word, int side);
extern unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos);
extern int Ta_RequestGravityReserve(int a, int b, int c, int *table);
int Ta_SetGravityEffect(struct jp_6989c *eInfo, struct jp_69942 *Temp, int Dist);

/* Ta_Eff_032 (0x25e710): prototype and local names from the Japanese build's debug
 * information (Seq032.c). Ta_SwapUnit is declared as small_misc.h has it. */
extern void Ta_SwapUnit(int a, int b, int c, int d);
int Ta_Eff_032(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_031 (0x25e200): prototype and the local eID from the Japanese build's debug
 * information (Seq031.c). The four callees are declared as state_grid.h / packed_item.h have them. */
extern int SzEffect_SetAnimIntr2(int packed, int value);
extern int SzDuel_GetUnitCtlRateM(int side, int pos);
extern int SzDuel_SetUnitCtlGoalM(int side, int pos, int value);
extern int SzDuel_SetUnitCtlRateM(int side, int pos, int pair, int value);
int Ta_Eff_031(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_014 (0x25a4c0): prototype and the locals pCall, eCall, eObj, i from the Japanese build's
 * debug information (Seq014.c); the locals are declared i, eObj, eCall, pCall here. TaEff_PopHand
 * (same original file) is declared with this file's structure types; its own group declares it
 * with opaque ones. The hand callees' prototypes are the ones state_records.h declares. */
extern int SzDuel_GetHandCount(int side);
extern int SzDuel_InqHand(int side, int position);
extern int SzDuel_DelHand(int index, int position);
extern int SzDuel_AddGrave(int side, int cardNo);
extern int TaEff_PopHand(struct jp_6989c *eInfo, struct jp_69942 *Temp, unsigned char dir, int x, unsigned char mode);
int Ta_Eff_014(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_058 (0x268ad0): prototype, the locals and the two structures from the Japanese build's
 * debug information (Seq058.c). struct jp_6b127 (card constants) and struct jp_6a917 (unit
 * variables) list only the members up to the ones used; the rest is padding. SzDuel_AddUnit is
 * declared elsewhere as returning nothing; its result register is used here as the new unit's
 * variables. */
struct jp_6b127 {
    unsigned char bKind;
    unsigned char bKindOfs;
    unsigned char bAttr : 4;
    unsigned char bLevel : 4;
    unsigned char bDeckCost;
    unsigned short wEffIdx;
    unsigned short wEffNo;
    unsigned short wAP : 13;
    unsigned short fPermanent : 1;
    unsigned short fGetPasswd : 1;
    unsigned short fGetRecycle : 1;
};
struct jp_6a917 {
    unsigned char unknown_00[0x3E];
    unsigned char bVarKind;
    unsigned char unknown_3F[5];
    unsigned char bOwner;
    unsigned char bID;
    unsigned char unknown_46[0x10];
    int nCtlFlipC : 2;
    int nCtlFlipM : 2;
    int nOpenFlip : 2;
    int nDefenseFlip : 2;
    int fFinish : 1;
    int fRevolt : 1;
    int fOpen : 1;
    int fDefense : 1;
    int fOpenF : 1;
    int fDefenseF : 1;
    int fPickup : 1;
    int fCardBusy : 1;
};
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);
extern int SzDuel_DelGrave(int side, int position);
extern void SzDuel_PackGrave(int side);
extern struct jp_6b127 *SzDuel_GetCardConstInfo(int cardNo);
extern int Ta_GetSummonablePieceWithOrder(int side, int i, int avoidLabyrinth);
/* The level 3 loop over the started effects counts in the local LOC, not i: with i the two values
 * kept on the stack (the index and the walking pointer) swap their slots. */
int Ta_Eff_058(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_067 (0x26dcc0): prototype and the locals pCall, eCall, i from the Japanese build's debug
 * information (Seq067.c); the locals are declared i, eCall, pCall here. The callee prototypes are
 * the ones their own groups declare. */
extern int SzDuel_SetUnitCtlGoalC(int side, int pos, int value);
extern int SzDuel_SetUnitCtlRateC(int a, int b, int pair, int value);
extern int SzDuel_GetUnitCtlGoalM(int a, int b);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
int Ta_Eff_067(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_007 (0x258760): prototype and local names from the Japanese build's debug information
 * (Seq007.c; that build's body differs). The locals are declared Self before Loc and pCall last
 * to reproduce the saved registers; fObj2 (+0xd0) is the member the code uses for the flag list.
 * Ta_RemoveObjFromFlagLst's and Ta_GetAgreeOwnerIDon22bitFlagLst's prototypes are assumed as in
 * small_misc.h. */
extern int Ta_RemoveObjFromFlagLst(int flagList, int side);
extern int Ta_GetAgreeOwnerIDon22bitFlagLst(int ownerId, unsigned flagList);
int Ta_Eff_007(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
/* Ta_Eff_100 (0x262be0): prototype and the locals pCall, eCall from the Japanese build's debug
 * information (Seq100.c). eID[0] holds the running effect, ePos[0] the "unit replaced" flag and
 * fObj2 the frame counter, as the code uses them. SzDuel_PutUnit and SzDuel_SetUnitCtlModeC as
 * state_grid.h declares them. */
extern int SzDuel_PutUnit(int a, int b, int c);
extern int SzDuel_SetUnitCtlModeC(int a, int b, int value);
int Ta_Eff_100(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
/* Ta_Eff_054 (0x26b480): prototype and the locals pCall, eCall, eObj, i from the Japanese build's
 * debug information (Seq054.c); the body there differs, so they are a hypothesis for this one.
 * Ta_IsUnitControl as small_misc.h declares it and TaView_ReqFocusCamera3 as cell_sends.h does. */
extern int Ta_IsUnitControl(int a, int b);
extern void TaView_ReqFocusCamera3(unsigned char who, int packed, int add, int value);
int Ta_Eff_054(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
/* Ta_Eff_070, Ta_Eff_071 (between Ta_Eff_BTL_002 and TaLogic_InitLogicData; names of the
 * Japanese functions at the same position and size, docs/topics/jp-build-symbols.md): assumed to
 * share the Ta_Eff signature and the eInfo/Temp layouts because they use the same offsets and
 * callees. */
int Ta_Eff_070(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
int Ta_Eff_071(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_036 (0x2695d0): prototype and the locals pCall, eCall, temp2, temp, Unit2, Unit from the
 * Japanese build's debug information (Seq036.c; that build's body differs). Ta_Eff_036_LoadEffect
 * and Ta_Eff_036_ManageSound are declared here with this group's views of eInfo and Temp
 * (preset_words.h and sound_gate.h declare them with their own views of the same storage);
 * Enchant and TaEnc_IsActReflexEnc as kind_applies.h and small_misc.h declare them. */
extern int Enchant(int side, int pos, int c, int teamId);
extern int TaEnc_IsActReflexEnc(int index);
extern int Ta_Eff_036_LoadEffect(struct jp_6989c *eInfo, struct jp_69942 *Temp);
extern void Ta_Eff_036_ManageSound(struct jp_6989c *eInfo, struct jp_69942 *Temp);
int Ta_Eff_036(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_008 (0x258e20): prototype and the locals pCall, eCall, Par, Unit2, Unit from the Japanese
 * build's debug information (Seq008.c; that build's body differs). The declaration order in the
 * source is the one that reproduces the saved registers. SzDuel_GetLP takes the side here, as
 * cell_verdicts.h's view of 0x1df470 does (lower_rules.h declares it without parameters; the two
 * views differ). */
extern int SzDuel_GetLP(int side);
int Ta_Eff_008(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_046 (0x265890): prototype and the locals eCall, Unit, LOC, tCount, j, i from the
 * Japanese build's debug information (its body differs: changed for the US release); pCall as in
 * the group's other steps. fObj2 is the frame counter of level 2, wCount_1 the number of cells to
 * wait for and wCount_2 the number finished, as the code uses them. Ta_IsFinishEffect is declared
 * with the step's Temp (small_misc.h declares it with its byte-list view of the same object) and
 * SzDuel_GetMapUnit as driver_root.h declares it; only its result being non-zero is used. */
extern int Ta_IsFinishEffect(struct jp_69942 *Temp);
extern void *SzDuel_GetMapUnit(int x, int y);
int Ta_Eff_046(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_026 (0x25c9e0): prototype, parameter and local names from the Japanese build's debug
 * information (Seq026.c; the body differs there). Ta_IsResistAllMaster's prototype is assumed as
 * state_queries.h declares it; its result is kept in wCount_2 and used as a packed unit. */
extern int Ta_IsResistAllMaster(int side, int pos);
int Ta_Eff_026(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_005 (0x257aa0): prototype, parameter and local names from the Japanese build's debug
 * information (Seq005.c; the body differs there). The local nMiss is not in that list: the
 * saved registers only reproduce with a counter of its own. It is read before any assignment when
 * level 255 meets a twelfth frame, as in the original. Ta_GetLocDistanceFromCenterOnRangeXY is
 * assumed as small_misc.h declares it. */
extern int Ta_GetLocDistanceFromCenterOnRangeXY(int packed, int step, int mode);
int Ta_Eff_005(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_BTL_000 (0x273e60): prototype and the locals pCall, eCall, temp, i from the Japanese
 * build's debug information (SeqBtl.c; the body differs there: changed for the US release).
 * SzEffect_SetBlackOutForMonster as index_fanout.h declares it and SzData_CalcExp as
 * side_table.h does (assumed prototypes). eID[oNum - 1] and ePos[oNum - 1] are the effect started
 * last, fObj2 the frame counter of level 5 and wCount_1 the black-out rate, as the code uses them. */
extern void SzEffect_SetBlackOutForMonster(int value);
extern int SzData_CalcExp(int side, int cardNo, int operation, int expChange);
int Ta_Eff_BTL_000(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

/* Ta_Eff_BTL_001 (0x2747a0): prototype and the locals pCall, eCall, i from the Japanese build's
 * debug information (SeqBtl.c; the body differs there: changed for the US release); temp is
 * this project's name. SzEffect_Sleep as pool_table.h declares it, TaTuto_IsTuto as
 * block_table.h declares it. SzDuel_GetOption3DBattle is declared in resource_support.h without
 * parameters; this caller passes the turn owner in a0, so it is declared here with that one
 * argument (assumed; the two headers cannot be included together). ePos[0] is the step of the
 * first effect, fObj2 the frame counter of level 4 and wCount_1 the black-out rate, as the code
 * uses them. */
extern int SzEffect_Sleep(int index, int flag);
extern int TaTuto_IsTuto(int side);
extern int SzDuel_GetOption3DBattle(int side);
int Ta_Eff_BTL_001(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);
/* Ta_Eff_037 (0x26a0a0): prototype and the local names pCall, eCall, temp, Efc, Unit2, Unit from the
 * Japanese build's debug information (Seq037.c; that build's body differs from this one, so the
 * roles given to the locals here are read from this body). fObj holds the packed unit of
 * the opened trap from the start call on, fObj2 the sound step of level 3 and eID[0] / ePos[0]
 * the running effect. Ta_GetEnchantOfs and Ta_GetForceEffection as state_queries.h
 * declares them, TaView_ReqFireView as small_misc.h does. */
extern int Ta_GetEnchantOfs(void);
extern int Ta_GetForceEffection(int side, int b);
extern int TaView_ReqFireView(int a, int b, unsigned char c, unsigned char d);
int Ta_Eff_037(struct jp_6989c *eInfo, struct jp_69942 *Temp, int ID);

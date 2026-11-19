#ifndef DOTR_EE_DUEL_ACT_FUNCTIONS_H
#define DOTR_EE_DUEL_ACT_FUNCTIONS_H

/* Declarations for the SzDuel_Act original-file unit (src/ee/duel_act/func_001cee60.c;
 * docs/modules/duel.md). Only the prototypes the unit uses are kept. Types and signatures are
 * assumed views, not recovered declarations. This file holds the prototypes; all of them are
 * visible to every compiler (the target and the host build both need them). */

/* Prototypes of the decompiled functions. */
s32 Chk_TrapA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 arg5);
f32 Ex3d_GetLookVector(s32 arg0);
void Ex3d_GetPointOnScreen(void *arg0, f32 fparg0, f32 fparg1);
u8 *ExGetMyOT(void);
s32 ExSetAnchor_Pipe(s32 arg0);
s32 ExSortAnchor_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ExSortPrim_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void FlushCache(int arg0);
s32 RhBase_GetVifPacketArea(void);
void RhBase_SetVifPacketArea(s32 arg0);
s32 RhBase_SetZBufferMode(s32 arg0, s32 arg1);
f32 RhGeom_GetPointsDistance(f32 *arg0, f32 *arg1);
void *RhMem_CopyMemory16(void *dest, void *source, s32 count);
s32 RhText_SetTextureFlush(s32 arg0);
void SzBattle_Entry(s32 arg0, s32 arg1);
void SzBattle_Init(s32 arg0);
s32 SzBattle_InqCamera(void);
void SzBattle_Return(s32 arg0, s32 arg1);
s32 SzData_AddBag(s32 side, s32 arg1, s32 cardNo);
void *SzData_GetData(s32 side);
s32 SzData_GetOption(s32 side, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s64 SzData_GetRank(s32 side, s32 cardNo);
#endif
s32 SzData_GetTreasureFlag(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
u16 SzData_SearchTreasure(s32 duelist, s32 col, s32 row, s32 arg3);
#endif
s32 SzData_SetLookFlag(s32 arg0, s32 arg1, s32 arg2);
s32 SzData_SetOption(s32 arg0, s32 arg1, s32 arg2);
s32 SzData_SetRank(s32 player, s32 index);
s32 SzData_SetTreasureFlag(s32 player, s32 currentMap, s32 enabled);
s32 SzDuel_Act(void);
void SzDuel_ActMapEdit(void);
int SzDuel_AddUnit2(int side, int x, int y, void *src_v, int leader);
s32 SzDuel_CalcMapColor(s32 x, s32 y);
s32 SzDuel_CalcSummonPower(s32 side, s32 operation, s32 amount);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_CalcUnitMovable(void);
#endif
void SzDuel_CalcUnitPos(s32 arg0, s32 arg1, s32 arg2, void *arg3);
s32 SzDuel_ClearCardControl(u8 *arg0);
s32 SzDuel_ClearDrawCard(void *arg0);
s32 SzDuel_ClearHandTag(s32 side);
void SzDuel_CloseMsgPopup(void);
s32 SzDuel_DelHand(s32 arg0, s32 arg1);
s32 SzDuel_DelUnit(s32 side, s32 unitArrayPos);
s32 SzDuel_DoCardControl0(void *arg0);
void SzDuel_DrawAllCard(void);
s32 SzDuel_DrawAllCard_Pipe(s32 arg0);
void SzDuel_DrawCard(s32 arg0);
s32 SzDuel_DrawCardRequest_Pipe(s32 arg0);
void SzDuel_DrawCardTest(void);
s32 SzDuel_DrawCard_Pipe(s32 arg0, s32 arg1);
s32 SzDuel_DrawHandCard_Pipe(s32 arg0, CardNode_001AF180 *object);
s32 SzDuel_DrawLeaderBase_Pipe(s32 arg0, s32 arg1, f32 *arg2, s32 arg3, s32 arg4);
void SzDuel_DrawMapEdit(void);
s32 SzDuel_DrawMapEdit_Pipe(s32 arg0);
void SzDuel_DrawUnit(void);
s32 SzDuel_DrawUnit_Pipe(s32 arg0);
s32 SzDuel_DrawWindow_ExDigit_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void SzDuel_DrawWindow_FontDigit_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 flags, s32 width, s32 number);
s32 SzDuel_DrawWindow_Font_Pipe(s32 packet, s32 x, s32 y, s32 font, s32 flags, s32 count, s8 *text, u8 *color);
void SzDuel_FinishCardSession(void);
s32 SzDuel_FinishCardSession_Pipe(s32 arg0);
s32 SzDuel_FreeCardRequest(void *arg0);
s32 SzDuel_GetCamDist(s32 arg0);
void *SzDuel_GetCardControl(s32 arg0);
s32 SzDuel_GetCardNode(void *arg0, s32 arg1);
s32 SzDuel_GetCardNodeOnScreen(void *arg0, s32 arg1, f32 fparg0, f32 fparg1);
u8 *SzDuel_GetCardRequest(void);
s32 SzDuel_GetDeckCount(s32 side);
s32 SzDuel_GetField(s32 arg0, s32 arg1);
s16 SzDuel_GetFieldRate(s32 arg0, s32 arg1);
s32 SzDuel_GetHandCount(s32 side);
s32 SzDuel_GetLP(s32 side);
void *SzDuel_GetMapInfo(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
u8 SzDuel_GetMapMovable(s32 x, s32 y);
#endif
s32 SzDuel_GetMapUnit(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzDuel_GetOption3DBattle(void);
#endif
s32 SzDuel_GetPlayMotion(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
u8 SzDuel_GetPlayer(s32 side);
s32 SzDuel_GetSummonPower(s32 side);
s32 SzDuel_GetUnit(s32 side, s32 pos);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_GetUnitCtlRateM(void);
#endif
s32 SzDuel_GetUnitLeaderAbility2(void *unit, s32 abil, s32 extraDataOut);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzDuel_GetUnitLevel(s32 side, s32 pos, s32 ignoreBoosts);
#endif
s32 SzDuel_GetUnitMonsterCount(s32 side);
s32 SzDuel_GetUnitPos(s32 arg0, s32 arg1, f32 *arg2);
s32 SzDuel_GetUnitSpellCount(s32 side);
void SzDuel_InitCardControl(void);
void SzDuel_InitCardInfo(void *unit, s32 cardId);
void SzDuel_InitCardRequest(void);
void SzDuel_InitFieldRate(void);
void SzDuel_InitMapColor2(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SzDuel_InitMapEdit(s32 arg0);
void SzDuel_InitTurn(s32 arg0, s32 arg1);
s32 SzDuel_InqGrave(s32 side, s32 arg1);
s32 SzDuel_InqHand(s32 side, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_IsUnitMovableOnLabyrinth(void);
#endif
void *SzDuel_MoveUnit(s32 arg0, s32 arg1, s32 x, s32 y);
void SzDuel_OpenMsgPopup(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SzDuel_SetBusyFlag(s32 side, s32 pos, s32 arg2);
s32 SzDuel_SetCardBusyFlag(s32 side, s32 pos, s32 arg2);
s32 SzDuel_SetField(s32 arg0, s32 arg1, s8 arg2);
s32 SzDuel_SetFieldRate(s32 arg0, s32 arg1, s32 arg2);
s32 SzDuel_SetHandTag(s32 side, s32 arg1, s32 arg2);
s32 SzDuel_SetMapColor2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_SetUnitCtlGoalM(s32 side, s32 pos, s8 arg2);
#endif
s32 SzDuel_SetUnitCtlRateM(s32 side, s32 pos, s32 arg2, s32 arg3);
void SzDuel_SetWindowDefPrioZ(s32 arg0);
void SzDuel_SetupCard(s32 arg0, s32 arg1);
s32 SzDuel_SetupCard2_Pipe(s32 arg0, s32 unit, void *arg2);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzDuel_SetupDuel(s32 arg0, s32 arg1, s32 arg2, void *arg3);
#endif
void SzDuel_StartCardSession(void);
s32 SzDuel_StartCardSession_Pipe(s32 arg0);
s32 SzDuel_StartWindowSessionWithPalette_Pipe(s32 arg0, s32 arg1);
void SzDuel_TermMapEdit(void);
s32 SzDuel_TrimUnitDir(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SzDuel_TrimUnitDir1(s32 arg0, s32 arg1);
s32 SzDuel_TrimUnitDir2(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SzDuel_Xax2New(s32 arg0);
s32 SzEffect_Destroy2(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzEffect_Entry(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
#endif
u8 SzEffect_GetBlackOutForMonster(void);
u8 *SzEffect_GetModuleInfo(s32 arg0, s32 arg1);
s32 SzEffect_GetStatus(s32 arg0);
s32 SzEffect_IsUse(s32 arg0);
s32 SzEffect_IsUse2(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzEffect_LoadModule(s32 kind, s32 card, s32 mode, void *unused);
#endif
void SzEffect_SetBlackOutForDuel(s32 blackoutAmount);
void SzEffect_SetBlackOutForMonster(s32 arg0);
void SzEffect_SetIntensityOnly(s32 arg0, f32 fparg0);
void SzEffect_SetPauseFor3DBattle(s32 arg0);
void SzEffect_SetPointLightForBattle(s32 pointLightParameters);
void SzEffect_SetPointLightForMonster(s32 pointLightParameters);
s32 SzEffect_Sleep(s32 arg0, s32 arg1);
s32 SzFile_InqRequestEx(void);
void SzGame_ComInit(void);
s32 SzGame_GetMenuNum(s32 arg0);
s64 SzGame_GetMode(s32 arg0);
void SzGame_Goto(s32 arg0, s32 arg1);
void SzGame_Pop(void);
void SzGame_ReqBGM(int n, int b);
void SzGame_SetAutoBlur(s32 arg0);
s32 SzGame_SetMenuNum(s32 arg0, s32 arg1);
s32 SzModel_BattleSeq(s32 arg0, s32 arg1, s32 arg2);
s32 SzModel_BattleSeq2(s32 arg0, s32 arg1, s32 arg2);
int SzModel_ExSort_Pipe(int cursor, int index, int c, int d, int e, int f, int g);
s32 SzModel_GetAlpha(s32 arg0);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s8 SzModel_GetAnim(s32 modelId);
#endif
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s64 SzModel_GetAnimDir(s32 modelId);
#endif
f32 SzModel_GetAnimFrame(s32 modelId);
s32 SzModel_GetCenter(s32 index, void *out);
s32 SzModel_GetDrawFlag(s32 arg0);
s32 SzModel_IsModelDisp(s32 modelId);
s32 SzModel_IsModelStandby(s32 arg0);
s32 SzModel_LoadMerge(s32 modelType, s32 arg1, s32 arg2);
s32 SzModel_LoadOverlayB(s32 modelId);
s32 SzModel_SetAlpha(s32 arg0, u8 arg1);
s32 SzModel_SetAnimDir(s32 modelId, s64 animDir);
s32 SzModel_SetAnimDirect(s32 arg0, s32 arg1);
s32 SzModel_SetAnimIntr(s32 modelId, s32 animIndex);
s32 SzModel_SetDrawFlag(s32 arg0, s32 arg1);
s32 SzModel_SetFilter(s32 arg0, s32 arg1, void *arg2);
s32 SzMsg_DrawRatioX_Pipe(s32 arg0, s32 arg1, s64 arg2, s64 arg3, f32 scaleX, f32 scaleY, s32 arg4, void (*arg5)(void));
s32 SzMsg_DrawRect_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, u8 *arg5);
s32 SzMsg_DrawX_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void SzMsg_Free(s32 arg0);
void *SzMsg_GetInfo(s32 arg0);
s32 SzMsg_LoadFont_Pipe(s32 arg0, s32 msgInfoIndex, s32 stringIndex, s32 arg3, s32 arg4);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzPad_Check(s32 arg0, s32 arg1, s32 arg2);
#endif
u8 SzPad_GetAnalogInfo(s32 arg0, s32 arg1);
s32 SzPad_IsAnalog(s32 arg0);
s32 SzSound_IsLoadingBGM(void);
s32 SzSound_PlayBGM(s32 arg0, s32 arg1);
s32 SzSound_PlaySE(s32 arg0, s32 soundEffectNumber);
s32 SzText_FreeRequest(s32 textureFlags, s32 textureId);
void *SzText_GetReadyItem(s32 arg0, s32 arg1);
s32 SzText_SleepRequest(s32 category, s32 id, s32 enabled);
s32 SzView_FreeRequest(s32 arg0, s32 arg1);
s32 SzView_GetCP(s32 arg0);
f32 *SzView_GetLook(s32 arg0);
s32 SzView_GetMode(s32 arg0);
s32 SzView_GetSP(s32 arg0);
void SzView_IV2V(void *arg0, void *arg1);
void *SzView_InqRequest(s32 arg0, s32 arg1);
s32 SzView_PauseRequest(s32 arg0, s32 arg1, s32 arg2);
s32 SzView_ReqCamera(s32 owner, s32 target, s32 type, s32 count, s32 value, void *data);
s32 SzView_RollCamera(s32 arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1, f32 fparg2);
s32 SzView_SetCP(s32 arg0, void *arg1);
s32 SzView_SetCamera(s32 index, void *sourcePoint, void *cameraPoint);
s32 SzView_SetInterpCameraMotion(s32 index, void *motion, s32 duration, s32 frame, s32 parent, s32 mirror);
s32 SzView_SetLookSP(s32 index, f32 *src);
s32 SzView_SetMode(s32 arg0, s32 arg1);
s32 SzView_SetViewCamera(s32 index, s32 otherIndex, f32 *target, s32 mode, f32 weight);
int TaSlot_SetSlotInitialDat(int nSide, void *pSlotOrder_v);
s32 TaTuto_IsTuto(s32 side);
s32 TaUnit_AddUnitEfc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void *arg4);
s32 Ta_DoUncTrapB(void *arg0);
s32 Ta_Do_CompMove(s32 team, s32 unitArrayPos, s32 col, s32 row, s32 arg4);
int Ta_Do_DuelEnd(int nSide, unsigned char bCond);
s32 Ta_Do_EndPhase(s32 phase, s32 *res);
s32 Ta_Do_HandDestroyEfc(void *arg0, s32 arg1, s32 arg2);
int Ta_Do_StandbyPhase(int PrNo, void *eInfo_v);
int Ta_FinishMove(int nSide, int nID, int nX, int nZ, void *eInfo_v, int PrNo);
int Ta_GetComboResult(int nSide, int nCardNo, int nCardNo2);
s32 Ta_GetSummonCost(void);
s32 Ta_IsGraveEntryHit(s32 side, s32 arg1, s32 arg2);
s32 Ta_IsHandHeap(s32 nSide0, s32 nID0, s32 nSide1, s32 nID1, s32 *eInfo);
s32 Ta_IsTrapBInDecideOperation(s32 arg0, s32 arg1, s32 arg2, s32 *arg3);
s32 Ta_IsUnitControl(s32 arg0, s32 arg1);
s32 Ta_IsUnitHide(s32 unit, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 Ta_ManageHandEffect(s32 *handResult);
#endif
s32 Ta_Manage_Effect(u32 *arg0, u16 *arg1, s32 arg2);
s32 Ta_Open(s32 x, s32 y, void *arg2, s32 phase);
s32 Ta_SetDestinyEffect(s32 side, s32 *arg1);
void Taku_Camp_Go_Rensyu_Duel(s32 arg0);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void Taku_Camp_What_KATIMAKE(int win);
#endif
s32 func_0019a5c8(void);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s64 func_001a22c0(f32 arg_a);
#endif
s32 func_001a23b0(f32 arg_a);
f32 func_001a6c30(f32 x, f32 y);
void lSzDuel_CalcMapMovable(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7, u8 *arg8, s32 count);
void lSzDuel_CalcMapSummonEnable(int side, int cx, int cy);
void lSzDuel_DrawCustomDuel_Exit_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(s32 id, s32 player, s32 x, s32 y);
s32 lSzDuel_DrawDialog_Window_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);
s32 lSzDuel_DrawMapEdit_Graph_Pipe(s32 arg0, s32 x, s32 y);
s32 lSzDuel_DrawWindow_CommandGuide_Pipe(s32 arg0, s32 arg1, s64 arg2, s64 arg3);
s32 lSzDuel_DrawWindow_ExBoard_Pipe(s32 packet, s32 colorIndex, s32 x, s32 y, s32 w, s32 h, s32 shadow, s32 style);
s32 lSzDuel_DrawWindow_ExIcon_Pipe(s32 packet, u16 *src, s32 x, s32 y, s32 background, s32 image, s32 border, s32 shrink, s32 flipX, s32 flipY, s32 bright, u8 *rgb, s32 blend, u8 alpha);
s32 lSzDuel_DrawWindow_FieldPiece_Pipe(s32 arg0, s32 colorIndex, s32 card, s32 x, s32 y, f32 scale);
s32 lSzDuel_DrawWindow_Frame_Pipe(s32 packet, s32 colorIndex, s32 icons, s32 style, s32 x, s32 y, s32 cols, s32 rows, s64 bright);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 lSzDuel_GetControl(s32 arg0, s32 arg1);
#endif
void lSzDuel_InitCom(s32 arg0);
s32 lSzDuel_InitHandCancel(s32 arg0);
s32 lSzDuel_InitHandCombo(s32 side);
s32 lSzDuel_InitHandDark(s32 arg0, s32 arg1);
s32 lSzDuel_InitHandInfo2(s32 arg0, s32 unitArrayPos, s32 arg2, s32 arg3, s32 arg4);
void lSzDuel_InitHandUp(s32 side, s32 draw);
void lSzDuel_LockCamera(s32 arg0, s32 arg1, s32 arg2);
void lSzDuel_LockCamera2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 lSzDuel_SetCardCtl(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 lSzDuel_SetCardCtl2(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void mwOverlayInit(s32 arg0, s32 arg1);
s32 strlen(const char *s);

/* Functions declared again under the names the unit's code and its C library built-in calls
   use. */
s32 strlen(const char *s);
void mwOverlayInit(s32 arg0, s32 arg1);
void FlushCache(int arg0);
s32 SzGame_GetMenuNum(s32 arg0);
s32 SzGame_SetMenuNum(s32 arg0, s32 arg1);
void SzGame_SetAutoBlur(s32 arg0);
void SzGame_Goto(s32 arg0, s32 arg1);
void SzGame_Pop(void);
void SzGame_ComInit(void);
void SzGame_ReqBGM(int n, int b);
s64 SzGame_GetMode(s32 arg0);
void lSzDuel_InitCom(s32 arg0);
s32 SzDuel_StartWindowSessionWithPalette_Pipe(s32 arg0, s32 arg1);
s32 SzDuel_DrawHandCard_Pipe(s32 arg0, CardNode_001AF180 *object);
s32 lSzDuel_DrawDialog_Window_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);
s32 lSzDuel_DrawWindow_CommandGuide_Pipe(s32 arg0, s32 arg1, s64 arg2, s64 arg3);
s32 lSzDuel_DrawWindow_FieldPiece_Pipe(s32 arg0, s32 colorIndex, s32 card, s32 x, s32 y, f32 scale);
s32 lSzDuel_DrawWindow_ExIcon_Pipe(s32 packet, u16 *src, s32 x, s32 y, s32 background, s32 image, s32 border, s32 shrink, s32 flipX, s32 flipY, s32 bright, u8 *rgb, s32 blend, u8 alpha);
s32 lSzDuel_DrawWindow_Frame_Pipe(s32 packet, s32 colorIndex, s32 icons, s32 style, s32 x, s32 y, s32 cols, s32 rows, s64 bright);
s32 lSzDuel_DrawWindow_ExBoard_Pipe(s32 packet, s32 colorIndex, s32 x, s32 y, s32 w, s32 h, s32 shadow, s32 style);
s32 SzDuel_DrawWindow_ExDigit_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void SzDuel_DrawWindow_FontDigit_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 flags, s32 width, s32 number);
s32 SzDuel_DrawWindow_Font_Pipe(s32 packet, s32 x, s32 y, s32 font, s32 flags, s32 count, s8 *text, u8 *color);
s32 lSzDuel_DrawCustomDuel_Menu_SelectMapEdit_Pipe(s32 id, s32 player, s32 x, s32 y);
void lSzDuel_DrawCustomDuel_Exit_Pipe(s32 arg0, s32 arg1, s32 arg2);
void SzDuel_InitMapEdit(s32 arg0);
void SzDuel_TermMapEdit(void);
void SzDuel_ActMapEdit(void);
void SzDuel_DrawMapEdit(void);
s32 SzDuel_DrawMapEdit_Pipe(s32 arg0);
s32 lSzDuel_DrawMapEdit_Graph_Pipe(s32 arg0, s32 x, s32 y);
s32 SzDuel_ClearDrawCard(void *arg0);
void SzDuel_DrawAllCard(void);
s32 SzDuel_DrawAllCard_Pipe(s32 arg0);
void SzDuel_InitCardRequest(void);
s32 SzDuel_FreeCardRequest(void *arg0);
u8 *SzDuel_GetCardRequest(void);
s32 SzDuel_DrawCardRequest_Pipe(s32 arg0);
void SzDuel_InitCardControl(void);
s32 SzDuel_ClearCardControl(u8 *arg0);
void *SzDuel_GetCardControl(s32 arg0);
s32 SzDuel_DoCardControl0(void *arg0);
void SzDuel_DrawUnit(void);
s32 SzDuel_DrawUnit_Pipe(s32 arg0);
void SzDuel_DrawCardTest(void);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 lSzDuel_GetControl(s32 arg0, s32 arg1);
#endif
s32 SzDuel_Act(void);
void lSzDuel_InitHandUp(s32 side, s32 draw);
s32 lSzDuel_InitHandCancel(s32 arg0);
s32 lSzDuel_InitHandCombo(s32 side);
s32 lSzDuel_InitHandDark(s32 arg0, s32 arg1);
s32 lSzDuel_InitHandInfo2(s32 arg0, s32 unitArrayPos, s32 arg2, s32 arg3, s32 arg4);
s32 lSzDuel_SetCardCtl(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 lSzDuel_SetCardCtl2(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void lSzDuel_LockCamera(s32 arg0, s32 arg1, s32 arg2);
void lSzDuel_LockCamera2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void SzDuel_InitTurn(s32 arg0, s32 arg1);
void SzDuel_StartCardSession(void);
s32 SzDuel_StartCardSession_Pipe(s32 arg0);
void SzDuel_FinishCardSession(void);
s32 SzDuel_FinishCardSession_Pipe(s32 arg0);
void SzDuel_SetWindowDefPrioZ(s32 arg0);
s32 SzDuel_GetCardNode(void *arg0, s32 arg1);
s32 SzDuel_GetCardNodeOnScreen(void *arg0, s32 arg1, f32 fparg0, f32 fparg1);
void SzDuel_SetupCard(s32 arg0, s32 arg1);
s32 SzDuel_SetupCard2_Pipe(s32 arg0, s32 unit, void *arg2);
void SzDuel_DrawCard(s32 arg0);
s32 SzDuel_DrawCard_Pipe(s32 arg0, s32 arg1);
s32 SzDuel_DrawLeaderBase_Pipe(s32 arg0, s32 arg1, f32 *arg2, s32 arg3, s32 arg4);
void *SzDuel_GetMapInfo(s32 arg0, s32 arg1);
void SzDuel_InitCardInfo(void *unit, s32 cardId);
s32 SzDuel_TrimUnitDir(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SzDuel_TrimUnitDir1(s32 arg0, s32 arg1);
s32 SzDuel_TrimUnitDir2(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzDuel_SetupDuel(s32 arg0, s32 arg1, s32 arg2, void *arg3);
#endif
s32 SzDuel_Xax2New(s32 arg0);
s32 SzDuel_GetCamDist(s32 arg0);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzDuel_GetOption3DBattle(void);
#endif
s32 SzDuel_GetPlayMotion(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
u8 SzDuel_GetPlayer(s32 side);
s32 SzDuel_GetSummonPower(s32 side);
s32 SzDuel_CalcSummonPower(s32 side, s32 operation, s32 amount);
s32 SzDuel_GetDeckCount(s32 side);
s32 SzDuel_InqGrave(s32 side, s32 arg1);
s32 SzDuel_InqHand(s32 side, s32 arg1);
s32 SzDuel_GetHandCount(s32 side);
s32 SzDuel_DelHand(s32 arg0, s32 arg1);
s32 SzDuel_ClearHandTag(s32 side);
s32 SzDuel_SetHandTag(s32 side, s32 arg1, s32 arg2);
s32 SzDuel_GetLP(s32 side);
s32 SzDuel_GetUnit(s32 side, s32 pos);
int SzDuel_AddUnit2(int side, int x, int y, void *src_v, int leader);
s32 SzDuel_DelUnit(s32 side, s32 unitArrayPos);
void *SzDuel_MoveUnit(s32 arg0, s32 arg1, s32 x, s32 y);
s32 SzDuel_GetUnitMonsterCount(s32 side);
s32 SzDuel_GetUnitSpellCount(s32 side);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzDuel_GetUnitLevel(s32 side, s32 pos, s32 ignoreBoosts);
#endif
s32 SzDuel_GetUnitLeaderAbility2(void *unit, s32 abil, s32 extraDataOut);
void SzDuel_CalcUnitPos(s32 arg0, s32 arg1, s32 arg2, void *arg3);
s32 SzDuel_GetUnitPos(s32 arg0, s32 arg1, f32 *arg2);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_CalcUnitMovable(void);
#endif
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_IsUnitMovableOnLabyrinth(void);
#endif
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_SetUnitCtlGoalM(s32 side, s32 pos, s8 arg2);
#endif
s32 SzDuel_SetUnitCtlRateM(s32 side, s32 pos, s32 arg2, s32 arg3);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzDuel_GetUnitCtlRateM(void);
#endif
s32 SzDuel_SetField(s32 arg0, s32 arg1, s8 arg2);
s32 SzDuel_GetField(s32 arg0, s32 arg1);
void SzDuel_InitFieldRate(void);
s32 SzDuel_SetFieldRate(s32 arg0, s32 arg1, s32 arg2);
s16 SzDuel_GetFieldRate(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
u8 SzDuel_GetMapMovable(s32 x, s32 y);
#endif
void lSzDuel_CalcMapSummonEnable(int side, int cx, int cy);
void lSzDuel_CalcMapMovable(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7, u8 *arg8, s32 count);
void SzDuel_InitMapColor2(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 SzDuel_SetMapColor2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 SzDuel_CalcMapColor(s32 x, s32 y);
s32 SzDuel_GetMapUnit(s32 arg0, s32 arg1);
s32 SzDuel_SetBusyFlag(s32 side, s32 pos, s32 arg2);
s32 SzDuel_SetCardBusyFlag(s32 side, s32 pos, s32 arg2);
void SzDuel_OpenMsgPopup(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void SzDuel_CloseMsgPopup(void);
void SzBattle_Init(s32 arg0);
s32 SzBattle_InqCamera(void);
void SzBattle_Entry(s32 arg0, s32 arg1);
void SzBattle_Return(s32 arg0, s32 arg1);
s32 SzModel_LoadMerge(s32 modelType, s32 arg1, s32 arg2);
int SzModel_ExSort_Pipe(int cursor, int index, int c, int d, int e, int f, int g);
s32 SzModel_BattleSeq(s32 arg0, s32 arg1, s32 arg2);
s32 SzModel_BattleSeq2(s32 arg0, s32 arg1, s32 arg2);
s32 SzModel_LoadOverlayB(s32 modelId);
s32 SzModel_IsModelStandby(s32 arg0);
s32 SzModel_IsModelDisp(s32 modelId);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s8 SzModel_GetAnim(s32 modelId);
#endif
s32 SzModel_SetAnimIntr(s32 modelId, s32 animIndex);
s32 SzModel_SetAnimDirect(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s64 SzModel_GetAnimDir(s32 modelId);
#endif
s32 SzModel_SetAnimDir(s32 modelId, s64 animDir);
f32 SzModel_GetAnimFrame(s32 modelId);
s32 SzModel_GetCenter(s32 index, void *out);
s32 SzModel_GetAlpha(s32 arg0);
s32 SzModel_SetAlpha(s32 arg0, u8 arg1);
s32 SzModel_SetFilter(s32 arg0, s32 arg1, void *arg2);
s32 SzModel_SetDrawFlag(s32 arg0, s32 arg1);
s32 SzModel_GetDrawFlag(s32 arg0);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzEffect_LoadModule(s32 kind, s32 card, s32 mode, void *unused);
#endif
u8 *SzEffect_GetModuleInfo(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void SzEffect_Entry(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
#endif
s32 SzEffect_Destroy2(s32 arg0, s32 arg1);
s32 SzEffect_Sleep(s32 arg0, s32 arg1);
s32 SzEffect_IsUse(s32 arg0);
s32 SzEffect_IsUse2(s32 arg0, s32 arg1);
s32 SzEffect_GetStatus(s32 arg0);
void SzEffect_SetPointLightForMonster(s32 pointLightParameters);
void SzEffect_SetPointLightForBattle(s32 pointLightParameters);
void SzEffect_SetBlackOutForMonster(s32 arg0);
void SzEffect_SetBlackOutForDuel(s32 blackoutAmount);
u8 SzEffect_GetBlackOutForMonster(void);
void SzEffect_SetIntensityOnly(s32 arg0, f32 fparg0);
void SzEffect_SetPauseFor3DBattle(s32 arg0);
s32 SzView_GetSP(s32 arg0);
s32 SzView_GetCP(s32 arg0);
s32 SzView_SetCP(s32 arg0, void *arg1);
s32 SzView_SetCamera(s32 index, void *sourcePoint, void *cameraPoint);
void *SzView_InqRequest(s32 arg0, s32 arg1);
s32 SzView_PauseRequest(s32 arg0, s32 arg1, s32 arg2);
s32 SzView_FreeRequest(s32 arg0, s32 arg1);
s32 SzView_ReqCamera(s32 owner, s32 target, s32 type, s32 count, s32 value, void *data);
f32 *SzView_GetLook(s32 arg0);
s32 SzView_SetLookSP(s32 index, f32 *src);
s32 SzView_RollCamera(s32 arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1, f32 fparg2);
s32 SzView_SetInterpCameraMotion(s32 index, void *motion, s32 duration, s32 frame, s32 parent, s32 mirror);
s32 SzView_SetViewCamera(s32 index, s32 otherIndex, f32 *target, s32 mode, f32 weight);
s32 SzView_GetMode(s32 arg0);
s32 SzView_SetMode(s32 arg0, s32 arg1);
void SzView_IV2V(void *arg0, void *arg1);
s32 SzMsg_LoadFont_Pipe(s32 arg0, s32 msgInfoIndex, s32 stringIndex, s32 arg3, s32 arg4);
s32 SzMsg_DrawRect_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, u8 *arg5);
s32 SzMsg_DrawRatioX_Pipe(s32 arg0, s32 arg1, s64 arg2, s64 arg3, f32 scaleX, f32 scaleY, s32 arg4, void (*arg5)(void));
s32 SzMsg_DrawX_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void *SzMsg_GetInfo(s32 arg0);
void SzMsg_Free(s32 arg0);
s32 SzFile_InqRequestEx(void);
void *SzText_GetReadyItem(s32 arg0, s32 arg1);
s32 SzText_SleepRequest(s32 category, s32 id, s32 enabled);
s32 SzText_FreeRequest(s32 textureFlags, s32 textureId);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 SzPad_Check(s32 arg0, s32 arg1, s32 arg2);
#endif
s32 SzPad_IsAnalog(s32 arg0);
u8 SzPad_GetAnalogInfo(s32 arg0, s32 arg1);
f32 Ex3d_GetLookVector(s32 arg0);
void Ex3d_GetPointOnScreen(void *arg0, f32 fparg0, f32 fparg1);
u8 *ExGetMyOT(void);
s32 ExSortPrim_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ExSetAnchor_Pipe(s32 arg0);
s32 ExSortAnchor_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 RhBase_GetVifPacketArea(void);
void RhBase_SetVifPacketArea(s32 arg0);
s32 RhBase_SetZBufferMode(s32 arg0, s32 arg1);
void *RhMem_CopyMemory16(void *dest, void *source, s32 count);
f32 RhGeom_GetPointsDistance(f32 *arg0, f32 *arg1);
s32 RhText_SetTextureFlush(s32 arg0);
void *SzData_GetData(s32 side);
s32 SzData_SetOption(s32 arg0, s32 arg1, s32 arg2);
s32 SzData_GetOption(s32 side, s32 arg1);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s64 SzData_GetRank(s32 side, s32 cardNo);
#endif
s32 SzData_SetRank(s32 player, s32 index);
s32 SzData_AddBag(s32 side, s32 arg1, s32 cardNo);
s32 SzData_SetLookFlag(s32 arg0, s32 arg1, s32 arg2);
s32 SzData_GetTreasureFlag(s32 arg0, s32 arg1);
s32 SzData_SetTreasureFlag(s32 player, s32 currentMap, s32 enabled);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
u16 SzData_SearchTreasure(s32 duelist, s32 col, s32 row, s32 arg3);
#endif
s32 SzSound_PlaySE(s32 arg0, s32 soundEffectNumber);
s32 SzSound_PlayBGM(s32 arg0, s32 arg1);
s32 SzSound_IsLoadingBGM(void);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
void Taku_Camp_What_KATIMAKE(int win);
#endif
void Taku_Camp_Go_Rensyu_Duel(s32 arg0);
int Ta_FinishMove(int nSide, int nID, int nX, int nZ, void *eInfo_v, int PrNo);
s32 Chk_TrapA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 arg5);
s32 TaUnit_AddUnitEfc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void *arg4);
s32 Ta_Do_CompMove(s32 team, s32 unitArrayPos, s32 col, s32 row, s32 arg4);
s32 Ta_IsGraveEntryHit(s32 side, s32 arg1, s32 arg2);
s32 Ta_IsUnitControl(s32 arg0, s32 arg1);
int TaSlot_SetSlotInitialDat(int nSide, void *pSlotOrder_v);
s32 Ta_Open(s32 x, s32 y, void *arg2, s32 phase);
s32 Ta_Manage_Effect(u32 *arg0, u16 *arg1, s32 arg2);
s32 Ta_IsUnitHide(s32 unit, s32 arg1);
int Ta_Do_StandbyPhase(int PrNo, void *eInfo_v);
s32 Ta_IsTrapBInDecideOperation(s32 arg0, s32 arg1, s32 arg2, s32 *arg3);
s32 Ta_DoUncTrapB(void *arg0);
s32 Ta_IsHandHeap(s32 nSide0, s32 nID0, s32 nSide1, s32 nID1, s32 *eInfo);
s32 Ta_GetSummonCost(void);
#ifdef __MWERKS__ /* host: SzDuel_Act redeclares it in block scope with another type */
s32 Ta_ManageHandEffect(s32 *handResult);
#endif
int Ta_GetComboResult(int nSide, int nCardNo, int nCardNo2);
s32 Ta_Do_HandDestroyEfc(void *arg0, s32 arg1, s32 arg2);
s32 Ta_SetDestinyEffect(s32 side, s32 *arg1);
s32 Ta_Do_EndPhase(s32 phase, s32 *res);
int Ta_Do_DuelEnd(int nSide, unsigned char bCond);
s32 TaTuto_IsTuto(s32 side);
#endif

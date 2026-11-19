#ifndef DOTR_EE_FAKE_SHADOW_FUNCTIONS_H
#define DOTR_EE_FAKE_SHADOW_FUNCTIONS_H

/* Declarations for the lSzBattle_DrawFakeShadow original-file unit
 * (src/ee/fake_shadow/func_001e6f90.c; docs/modules/engine.md). Only the prototypes the unit uses
 * are kept. Types and signatures are assumed views, not recovered declarations. This file holds the
 * prototypes; all of them are visible to every compiler (the target and the host build both need
 * them). */


/* Prototypes of the decompiled functions. */
s32 Ex3d_GetPointsAverage(s32 count, void *arg1, void *arg2);
s32 Ex3d_GetSurfaceNormal(s32 count, void *arg1, f32 *normal);
s32 Ex3d_GetViewVector(f32 *origin, s32 count, f32 *points, f32 *out);
void Ex3d_IntersectP(void *arg0, void *arg1, void *arg2, void *arg3);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s32 Ex3d_NodeL2V(void *matrix, s32 count, void *vertices, s32 arg3, void *arg4, s32 flags, f32 *depth);
#endif
void Ex3d_NodeV2S(void *matrix, s32 count, void *vertices);
s32 ExAssignAlpha(void *arg0, s64 arg1);
s32 ExAssignTexture(void *arg0, void *arg1, s32 arg2);
void *ExDitherMode_Pipe(void *arg0, s32 arg1);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
int ExSortNode_Pipe(int cursor, void *header_v, int count, s32 vertices_v, void *chain_v, int z);
#endif
s32 ExSortPrim_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s32 RhBase_AssignRenderFrame(s32 arg0, s32 arg1, s32 arg2);
#endif
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
void *RhBase_ClearRenderFrame(u8 *packet, u8 *color);
#endif
void *RhBase_DisableRenderFrame(void *arg0);
void *RhBase_EnableRenderFrame(void *arg0);
void RhBase_FreeRenderFrame(void);
s32 RhBase_GetTextureInfoForRender(void *arg0, void *arg1);
void *RhBase_MaskFBuffer(void *arg0, u32 arg1);
s32 RhBase_SetZBufferMode(s32 arg0, s32 arg1);
f32 RhGeom_GetPointsDistance(f32 *arg0, f32 *arg1);
void RhGeom_GetRotXYZMatrix(void *dst, void *src);
f32 RhGeom_GetVectorScalor(f32 *arg0);
f32 RhGeom_InnerProduct(f32 *arg0, f32 *arg1);
void RhGeom_InvertMatrix(void *dst, void *src);
void RhGeom_MulMatrixVectorArray();
void RhGeom_PopMatrix(void);
void RhGeom_PushMatrix(s32 arg0);
s8 *RhMem_FillMemory(s8 *arg0, s8 arg1, s32 arg2);
s32 RhText_SetTextureFlush(s32 arg0);
s32 RhView_GetCurView(void *arg0);
void RhView_GetWorldViewMatrix(void *arg0);
s32 RhView_Modify(void *arg0);
s32 RhView_PopFakeMirror(void *arg0);
s32 RhView_PopStack(void);
s32 RhView_PushFakeMirror(f32 *point, f32 *normal, void *output);
s32 RhView_PushStack(void);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s32 RhView_SendViewParameters(s32 arg0);
#endif
s32 RhView_SetScreen(f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3, f32 fparg4);
s32 SzBattle_DrawStage_Pipe(s32 arg0);
void SzBattle_Entry(s32 arg0, s32 arg1);
void SzBattle_Init(s32 arg0);
void SzBattle_Initialize(void);
s32 SzBattle_InqCamera(void);
s32 SzBattle_InqEntry(s32 index);
void SzBattle_Return(s32 arg0, s32 arg1);
void SzDuel_CalcUnitPos(s32 arg0, s32 arg1, s32 arg2, void *arg3);
void SzDuel_CloseHandCursor(void);
s32 SzDuel_GetUnit(s32 side, s32 pos);
s32 SzDuel_SetBusyFlag(s32 side, s32 pos, s32 arg2);
void SzDuel_SetIgnoreJudge(s64 arg0);
s32 SzDuel_SetUnitCtlGoalM(s32 side, s32 pos, s8 arg2);
s32 SzDuel_SetUnitCtlRateM(s32 side, s32 pos, s32 arg2, s32 arg3);
s32 SzEffect_ClearRefID2(s32 arg0, s32 arg1);
u8 SzEffect_GetBlackOutForBattle(void);
void SzEffect_SetBlackOutForBattle(s32 blackOutValue);
void SzEffect_SetBlackOutForMonster(s32 arg0);
void SzEffect_SetFogColorForBattle(s32 arg0);
void SzEffect_SetFogColorForMonster(s32 fogColour);
void SzEffect_SetFogParamYForBattle(s32 arg0);
void SzEffect_SetFogParamYForMonster(s32 arg0);
void SzEffect_SetFogParamZForBattle(s32 arg0);
void SzEffect_SetFogParamZForMonster(s32 arg0);
void SzEffect_SetIntensityForBattle(s32 arg0);
void SzEffect_SetIntensityForMonster(s32 intensityValue);
void SzEffect_SetIntensityOnly(s32 arg0, f32 fparg0);
void SzEffect_SetPointLightForBattle(s32 pointLightParameters);
void SzEffect_SetPointLightForMonster(s32 pointLightParameters);
void SzEffect_SetStraightLightForBattle(s32 pointLightParameters);
void SzEffect_SetStraightLightForMonster(s32 pointLightParameters);
u8 *SzFile_GetEnv(s32 unused0);
s32 SzFile_ReadRequestParam(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s64 arg8);
s32 SzGame_GetFrameCount(void);
void SzMem_Free(s32 unused0);
void SzModel_CancelMotionBlur(s32 arg0);
s32 SzModel_DrawWithOutCalc_Pipe(s32 arg0, s32 modelId, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 SzModel_GetAlpha(s32 arg0);
f32 SzModel_GetAnimFrame(s32 modelId);
s32 SzModel_GetAnimFrameMax(s32 arg0, s32 arg1);
s32 SzModel_GetCenter(s32 index, void *out);
s32 SzModel_GetDrawFlag(s32 arg0);
s32 SzModel_GetIntensity(s32 arg0, f32 *arg1);
s32 SzModel_GetIntensityOnly(s32 arg0, f32 *arg1);
s16 SzModel_GetModelNo(s32 arg0);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s8 SzModel_GetModelType(s32 arg0);
#endif
s32 SzModel_GetPauseFlag(s32 arg0);
s32 SzModel_GetPos(s32 arg0, void *arg1);
s32 SzModel_GetRgba(s32 arg0, void *arg1);
s32 SzModel_GetRot(s32 arg0, void *arg1);
s32 SzModel_IsModelDisp(s32 modelId);
s32 SzModel_SetAnimDir(s32 modelId, s64 animDir);
s32 SzModel_SetAnimDirect(s32 arg0, s32 arg1);
void SzModel_SetAnimFrame(s32 arg0, f32 value);
s32 SzModel_SetDeform(s32 arg0, s32 arg1);
s32 SzModel_SetDeformRatio(s32 arg0, s8 arg1);
s32 SzModel_SetDrawFlag(s32 arg0, s32 arg1);
s32 SzModel_SetFilter(s32 arg0, s32 arg1, void *arg2);
s32 SzModel_SetFogFlag(s32 arg0, s64 arg1);
s32 SzModel_SetIntensity(s32 arg0, f32 *arg1);
s32 SzModel_SetIntensityOnly(s32 arg0, f32 fparg0);
s32 SzModel_SetPos(s32 index, void *src);
s32 SzModel_SetRgba(s32 arg0, s32 arg1, void *arg2);
s32 SzModel_SetRot(s32 arg0, void *arg1);
s32 SzModel_SetScale(s32 idx, void *src);
s32 SzSound_PlaySE(s32 arg0, s32 soundEffectNumber);
s32 SzSound_StereoPlaySE(s32 arg0, s32 arg1, s32 arg2);
s32 SzSound_StopSE(s32 arg0, s32 arg1);
s32 SzSound_TransferSEGrp(s32 arg0, s32 arg1, s32 arg2);
s32 SzView_GetSP(s32 arg0);
s32 SzView_GetStraightLightVector(s32 arg0, s32 arg1, s32 arg2, void *arg3);
s32 SzView_InqStraightLight(s32 arg0, s32 arg1);
s32 SzView_SetStraightLight(s32 arg0, s32 arg1, s32 arg2);
s32 func_0019a5c8(void);
s32 func_0019b648(char *str, const char *fmt, ...);
s32 func_001a23b0(f32 arg_a);
void lSzBattle_CameraCB(void *arg0, s32 arg1);
s32 lSzBattle_DrawFakeMirror(s32 arg0, s32 arg1);
s32 lSzBattle_DrawFakeShadow(s32 arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1);
s32 lSzBattle_DrawGrass_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawNormal_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawSea_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawStage_Pipe(s32 arg0);
s32 lSzBattle_DrawWasteland_Pipe(s32 arg0, s32 arg1, s32 arg2);
void lSzDuel_OpenHandCursor_Pipe(s32 packet, s32 angle, s32 x, s32 y);

/* Functions declared again under the names the unit's code and its C library built-in calls
   use. */
s32 SzGame_GetFrameCount(void);
s32 SzDuel_GetUnit(s32 side, s32 pos);
void SzDuel_CalcUnitPos(s32 arg0, s32 arg1, s32 arg2, void *arg3);
s32 SzDuel_SetUnitCtlGoalM(s32 side, s32 pos, s8 arg2);
s32 SzDuel_SetUnitCtlRateM(s32 side, s32 pos, s32 arg2, s32 arg3);
s32 SzDuel_SetBusyFlag(s32 side, s32 pos, s32 arg2);
void lSzDuel_OpenHandCursor_Pipe(s32 packet, s32 angle, s32 x, s32 y);
void SzDuel_CloseHandCursor(void);
void SzDuel_SetIgnoreJudge(s64 arg0);
void SzBattle_Initialize(void);
void SzBattle_Init(s32 arg0);
void lSzBattle_CameraCB(void *arg0, s32 arg1);
s32 SzBattle_InqCamera(void);
s32 SzBattle_InqEntry(s32 index);
void SzBattle_Entry(s32 arg0, s32 arg1);
void SzBattle_Return(s32 arg0, s32 arg1);
s32 SzBattle_DrawStage_Pipe(s32 arg0);
s32 lSzBattle_DrawStage_Pipe(s32 arg0);
s32 lSzBattle_DrawWasteland_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawGrass_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawSea_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawNormal_Pipe(s32 arg0, s32 arg1, s32 arg2);
s32 lSzBattle_DrawFakeMirror(s32 arg0, s32 arg1);
s32 lSzBattle_DrawFakeShadow(s32 arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1);
void SzModel_CancelMotionBlur(s32 arg0);
s32 SzModel_DrawWithOutCalc_Pipe(s32 arg0, s32 modelId, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 SzModel_IsModelDisp(s32 modelId);
s32 SzModel_SetAnimDirect(s32 arg0, s32 arg1);
s32 SzModel_SetAnimDir(s32 modelId, s64 animDir);
f32 SzModel_GetAnimFrame(s32 modelId);
void SzModel_SetAnimFrame(s32 arg0, f32 value);
s32 SzModel_GetAnimFrameMax(s32 arg0, s32 arg1);
s32 SzModel_GetIntensity(s32 arg0, f32 *arg1);
s32 SzModel_GetIntensityOnly(s32 arg0, f32 *arg1);
s32 SzModel_SetIntensity(s32 arg0, f32 *arg1);
s32 SzModel_SetIntensityOnly(s32 arg0, f32 fparg0);
s32 SzModel_GetPos(s32 arg0, void *arg1);
s32 SzModel_GetCenter(s32 index, void *out);
s32 SzModel_SetPos(s32 index, void *src);
s32 SzModel_GetRot(s32 arg0, void *arg1);
s32 SzModel_SetRot(s32 arg0, void *arg1);
s32 SzModel_SetScale(s32 idx, void *src);
s32 SzModel_GetRgba(s32 arg0, void *arg1);
s32 SzModel_SetRgba(s32 arg0, s32 arg1, void *arg2);
s32 SzModel_GetAlpha(s32 arg0);
s32 SzModel_SetFilter(s32 arg0, s32 arg1, void *arg2);
s16 SzModel_GetModelNo(s32 arg0);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s8 SzModel_GetModelType(s32 arg0);
#endif
s32 SzModel_SetDeform(s32 arg0, s32 arg1);
s32 SzModel_SetDeformRatio(s32 arg0, s8 arg1);
s32 SzModel_SetDrawFlag(s32 arg0, s32 arg1);
s32 SzModel_GetDrawFlag(s32 arg0);
s32 SzModel_SetFogFlag(s32 arg0, s64 arg1);
s32 SzModel_GetPauseFlag(s32 arg0);
s32 SzEffect_ClearRefID2(s32 arg0, s32 arg1);
void SzEffect_SetFogColorForMonster(s32 fogColour);
void SzEffect_SetFogColorForBattle(s32 arg0);
void SzEffect_SetFogParamZForMonster(s32 arg0);
void SzEffect_SetFogParamZForBattle(s32 arg0);
void SzEffect_SetFogParamYForMonster(s32 arg0);
void SzEffect_SetFogParamYForBattle(s32 arg0);
void SzEffect_SetStraightLightForMonster(s32 pointLightParameters);
void SzEffect_SetStraightLightForBattle(s32 pointLightParameters);
void SzEffect_SetPointLightForMonster(s32 pointLightParameters);
void SzEffect_SetPointLightForBattle(s32 pointLightParameters);
void SzEffect_SetBlackOutForMonster(s32 arg0);
void SzEffect_SetBlackOutForBattle(s32 blackOutValue);
u8 SzEffect_GetBlackOutForBattle(void);
void SzEffect_SetIntensityForMonster(s32 intensityValue);
void SzEffect_SetIntensityForBattle(s32 arg0);
void SzEffect_SetIntensityOnly(s32 arg0, f32 fparg0);
s32 SzView_GetStraightLightVector(s32 arg0, s32 arg1, s32 arg2, void *arg3);
s32 SzView_InqStraightLight(s32 arg0, s32 arg1);
s32 SzView_SetStraightLight(s32 arg0, s32 arg1, s32 arg2);
s32 SzView_GetSP(s32 arg0);
s32 SzFile_ReadRequestParam(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s64 arg8);
u8 *SzFile_GetEnv(s32 unused0);
s32 Ex3d_GetSurfaceNormal(s32 count, void *arg1, f32 *normal);
s32 Ex3d_GetPointsAverage(s32 count, void *arg1, void *arg2);
s32 Ex3d_GetViewVector(f32 *origin, s32 count, f32 *points, f32 *out);
void Ex3d_IntersectP(void *arg0, void *arg1, void *arg2, void *arg3);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s32 Ex3d_NodeL2V(void *matrix, s32 count, void *vertices, s32 arg3, void *arg4, s32 flags, f32 *depth);
#endif
void Ex3d_NodeV2S(void *matrix, s32 count, void *vertices);
s32 ExSortPrim_Pipe(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
int ExSortNode_Pipe(int cursor, void *header_v, int count, s32 vertices_v, void *chain_v, int z);
#endif
void *ExDitherMode_Pipe(void *arg0, s32 arg1);
s32 ExAssignAlpha(void *arg0, s64 arg1);
s32 ExAssignTexture(void *arg0, void *arg1, s32 arg2);
void *RhBase_MaskFBuffer(void *arg0, u32 arg1);
s32 RhBase_SetZBufferMode(s32 arg0, s32 arg1);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s32 RhBase_AssignRenderFrame(s32 arg0, s32 arg1, s32 arg2);
#endif
void *RhBase_EnableRenderFrame(void *arg0);
void *RhBase_DisableRenderFrame(void *arg0);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
void *RhBase_ClearRenderFrame(u8 *packet, u8 *color);
#endif
void RhBase_FreeRenderFrame(void);
s32 RhBase_GetTextureInfoForRender(void *arg0, void *arg1);
s8 *RhMem_FillMemory(s8 *arg0, s8 arg1, s32 arg2);
void RhGeom_PushMatrix(s32 arg0);
void RhGeom_PopMatrix(void);
void RhGeom_InvertMatrix(void *dst, void *src);
void RhGeom_GetRotXYZMatrix(void *dst, void *src);
void RhGeom_MulMatrixVectorArray();
f32 RhGeom_GetVectorScalor(f32 *arg0);
f32 RhGeom_GetPointsDistance(f32 *arg0, f32 *arg1);
f32 RhGeom_InnerProduct(f32 *arg0, f32 *arg1);
s32 RhView_GetCurView(void *arg0);
s32 RhView_SetScreen(f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3, f32 fparg4);
s32 RhView_Modify(void *arg0);
s32 RhView_PushStack(void);
s32 RhView_PopStack(void);
s32 RhView_PushFakeMirror(f32 *point, f32 *normal, void *output);
s32 RhView_PopFakeMirror(void *arg0);
void RhView_GetWorldViewMatrix(void *arg0);
#ifdef __MWERKS__ /* host: lSzBattle_DrawFakeShadow redeclares it in block scope with another type */
s32 RhView_SendViewParameters(s32 arg0);
#endif
s32 RhText_SetTextureFlush(s32 arg0);
s32 SzSound_TransferSEGrp(s32 arg0, s32 arg1, s32 arg2);
s32 SzSound_PlaySE(s32 arg0, s32 soundEffectNumber);
s32 SzSound_StereoPlaySE(s32 arg0, s32 arg1, s32 arg2);
s32 SzSound_StopSE(s32 arg0, s32 arg1);
void SzMem_Free(s32 unused0);

#endif

/* Host smoke test for SzDuel_Act (0x1cee60), the duel's per-frame state machine.
 * Fixtures are host data, not game data; stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
/* Built without -Werror: the C converts 32-bit EE words to pointers, which warns on a
 * 64-bit host. The path below executes none of those conversions. strlen is the C library's
 * (not called on this path). */
#include <assert.h>
#include "dotr/ee/duel_act.h"

/* The vector type the unit defines next to SzDuel_Act (same tag and members). */
struct Vec4_001CEE60 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};
typedef struct Vec4_001CEE60 Vec4_001CEE60;

/* Data the function reads (host storage; the target links them at their addresses). */
Vec4_001CEE60 data_00291510;
Vec4_001CEE60 data_00291520;
Vec4_001CEE60 data_00291530;
u16 gSzDuel_Camera[1024];
Duel_00317490 data_00317490;
u8 turnOwner;
u8 Player_GameState;
u8 gSzDuel_aCardControl_bytes[6 * 0xa0];

/* Call log. */
static const char *calls[64];
static int call_count;
static void dotr_act_note(const char *name)
{
    if (call_count < 64) {
        calls[call_count] = name;
    }
    call_count++;
}
static int same(const char *a, const char *b)
{
    while (*a != 0 && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}
static int called(const char *name)
{
    int i, n = 0;
    for (i = 0; i < call_count && i < 64; i++) {
        n += same(calls[i], name);
    }
    return n;
}

/* Callees on the tested path. */
static u8 module_info;
static s32 init_turn_args[2];
u8 *SzEffect_GetModuleInfo(s32 arg0, s32 arg1)
{
    dotr_act_note("SzEffect_GetModuleInfo");
    assert(arg0 == 0 && arg1 == 0x26c);
    return &module_info;
}
s32 SzFile_InqRequestEx(void) { dotr_act_note("SzFile_InqRequestEx"); return 0; }
void SzDuel_InitFieldRate(void) { dotr_act_note("SzDuel_InitFieldRate"); }
void SzDuel_InitTurn(s32 arg0, s32 arg1)
{
    dotr_act_note("SzDuel_InitTurn");
    init_turn_args[0] = arg0;
    init_turn_args[1] = arg1;
}

/* Every other callee, declared as the unit sees it (the header, or SzDuel_Act's block-scope
 * declarations where they differ): records the call, returns 0. */
s32 Chk_TrapA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4, s32 arg5) { dotr_act_note("Chk_TrapA"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; return 0; }
f32 Ex3d_GetLookVector(s32 arg0) { dotr_act_note("Ex3d_GetLookVector"); (void)arg0; return 0; }
void Ex3d_GetPointOnScreen(void *arg0, f32 fparg0, f32 fparg1) { dotr_act_note("Ex3d_GetPointOnScreen"); (void)arg0; (void)fparg0; (void)fparg1; }
void FlushCache(int arg0) { dotr_act_note("FlushCache"); (void)arg0; }
s32 func_0019a5c8(void) { dotr_act_note("func_0019a5c8"); return 0; }
s32 func_001a22c0(f32 p0) { dotr_act_note("func_001a22c0"); (void)p0; return 0; }
s32 func_001a23b0(f32 arg_a) { dotr_act_note("func_001a23b0"); (void)arg_a; return 0; }
f32 func_001a6c30(f32 x, f32 y) { dotr_act_note("func_001a6c30"); (void)x; (void)y; return 0; }
void lSzDuel_CalcMapMovable(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u8 *arg7, u8 *arg8, s32 count) { dotr_act_note("lSzDuel_CalcMapMovable"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; (void)arg7; (void)arg8; (void)count; }
void lSzDuel_CalcMapSummonEnable(int side, int cx, int cy) { dotr_act_note("lSzDuel_CalcMapSummonEnable"); (void)side; (void)cx; (void)cy; }
u16 lSzDuel_GetControl() { dotr_act_note("lSzDuel_GetControl"); return 0; }
void lSzDuel_InitCom(s32 arg0) { dotr_act_note("lSzDuel_InitCom"); (void)arg0; }
s32 lSzDuel_InitHandCancel(s32 arg0) { dotr_act_note("lSzDuel_InitHandCancel"); (void)arg0; return 0; }
s32 lSzDuel_InitHandCombo(s32 arg0) { dotr_act_note("lSzDuel_InitHandCombo"); (void)arg0; return 0; }
s32 lSzDuel_InitHandDark(s32 arg0, s32 arg1) { dotr_act_note("lSzDuel_InitHandDark"); (void)arg0; (void)arg1; return 0; }
s32 lSzDuel_InitHandInfo2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) { dotr_act_note("lSzDuel_InitHandInfo2"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; return 0; }
void lSzDuel_InitHandUp(s32 arg0, s32 arg1) { dotr_act_note("lSzDuel_InitHandUp"); (void)arg0; (void)arg1; }
void lSzDuel_LockCamera(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("lSzDuel_LockCamera"); (void)arg0; (void)arg1; (void)arg2; }
void lSzDuel_LockCamera2(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) { dotr_act_note("lSzDuel_LockCamera2"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; }
s32 lSzDuel_SetCardCtl(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) { dotr_act_note("lSzDuel_SetCardCtl"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return 0; }
s32 lSzDuel_SetCardCtl2(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) { dotr_act_note("lSzDuel_SetCardCtl2"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return 0; }
void mwOverlayInit(s32 arg0, s32 arg1) { dotr_act_note("mwOverlayInit"); (void)arg0; (void)arg1; }
f32 RhGeom_GetPointsDistance(f32 *arg0, f32 *arg1) { dotr_act_note("RhGeom_GetPointsDistance"); (void)arg0; (void)arg1; return 0; }
void *RhMem_CopyMemory16(void *dest, void *source, s32 count) { dotr_act_note("RhMem_CopyMemory16"); (void)dest; (void)source; (void)count; return 0; }
void SzBattle_Entry(s32 arg0, s32 arg1) { dotr_act_note("SzBattle_Entry"); (void)arg0; (void)arg1; }
void SzBattle_Init(s32 arg0) { dotr_act_note("SzBattle_Init"); (void)arg0; }
s32 SzBattle_InqCamera(void) { dotr_act_note("SzBattle_InqCamera"); return 0; }
void SzBattle_Return(s32 arg0, s32 arg1) { dotr_act_note("SzBattle_Return"); (void)arg0; (void)arg1; }
s32 SzData_AddBag(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzData_AddBag"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzData_GetOption(s32 arg0, s32 arg1) { dotr_act_note("SzData_GetOption"); (void)arg0; (void)arg1; return 0; }
s32 SzData_GetRank() { dotr_act_note("SzData_GetRank"); return 0; }
s32 SzData_GetTreasureFlag(s32 arg0, s32 arg1) { dotr_act_note("SzData_GetTreasureFlag"); (void)arg0; (void)arg1; return 0; }
s32 SzData_SearchTreasure() { dotr_act_note("SzData_SearchTreasure"); return 0; }
s32 SzData_SetLookFlag(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzData_SetLookFlag"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzData_SetOption(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzData_SetOption"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzData_SetRank(s32 player, s32 index) { dotr_act_note("SzData_SetRank"); (void)player; (void)index; return 0; }
s32 SzData_SetTreasureFlag(s32 player, s32 flag, s32 enabled) { dotr_act_note("SzData_SetTreasureFlag"); (void)player; (void)flag; (void)enabled; return 0; }
int SzDuel_AddUnit2(int side, int x, int y, void *src_v, int leader) { dotr_act_note("SzDuel_AddUnit2"); (void)side; (void)x; (void)y; (void)src_v; (void)leader; return 0; }
s32 SzDuel_CalcSummonPower(s32 index, s32 operation, s32 value) { dotr_act_note("SzDuel_CalcSummonPower"); (void)index; (void)operation; (void)value; return 0; }
s32 SzDuel_CalcUnitMovable() { dotr_act_note("SzDuel_CalcUnitMovable"); return 0; }
void SzDuel_CalcUnitPos(s32 arg0, s32 arg1, s32 arg2, void *arg3) { dotr_act_note("SzDuel_CalcUnitPos"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; }
s32 SzDuel_ClearCardControl(u8 *arg0) { dotr_act_note("SzDuel_ClearCardControl"); (void)arg0; return 0; }
s32 SzDuel_ClearDrawCard(void *arg0) { dotr_act_note("SzDuel_ClearDrawCard"); (void)arg0; return 0; }
s32 SzDuel_ClearHandTag(s32 arg0) { dotr_act_note("SzDuel_ClearHandTag"); (void)arg0; return 0; }
void SzDuel_CloseMsgPopup(void) { dotr_act_note("SzDuel_CloseMsgPopup"); }
s32 SzDuel_DelHand(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_DelHand"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_DelUnit(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_DelUnit"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_DoCardControl0(void *arg0) { dotr_act_note("SzDuel_DoCardControl0"); (void)arg0; return 0; }
s32 SzDuel_GetCamDist(s32 arg0) { dotr_act_note("SzDuel_GetCamDist"); (void)arg0; return 0; }
s32 SzDuel_GetDeckCount(s32 arg0) { dotr_act_note("SzDuel_GetDeckCount"); (void)arg0; return 0; }
s32 SzDuel_GetField(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_GetField"); (void)arg0; (void)arg1; return 0; }
s16 SzDuel_GetFieldRate(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_GetFieldRate"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetHandCount(s32 arg0) { dotr_act_note("SzDuel_GetHandCount"); (void)arg0; return 0; }
s32 SzDuel_GetLP(s32 arg0) { dotr_act_note("SzDuel_GetLP"); (void)arg0; return 0; }
void *SzDuel_GetMapInfo(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_GetMapInfo"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetMapMovable() { dotr_act_note("SzDuel_GetMapMovable"); return 0; }
s32 SzDuel_GetMapUnit(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_GetMapUnit"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetOption3DBattle() { dotr_act_note("SzDuel_GetOption3DBattle"); return 0; }
u8 SzDuel_GetPlayer(s32 arg0) { dotr_act_note("SzDuel_GetPlayer"); (void)arg0; return 0; }
s32 SzDuel_GetPlayMotion(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) { dotr_act_note("SzDuel_GetPlayMotion"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; (void)arg5; (void)arg6; return 0; }
s32 SzDuel_GetSummonPower(s32 arg0) { dotr_act_note("SzDuel_GetSummonPower"); (void)arg0; return 0; }
s32 SzDuel_GetUnit(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_GetUnit"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_GetUnitCtlRateM() { dotr_act_note("SzDuel_GetUnitCtlRateM"); return 0; }
s32 SzDuel_GetUnitLeaderAbility2(void *arg0, s32 arg1, s32 arg2) { dotr_act_note("SzDuel_GetUnitLeaderAbility2"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzDuel_GetUnitLevel() { dotr_act_note("SzDuel_GetUnitLevel"); return 0; }
s32 SzDuel_GetUnitMonsterCount(s32 arg0) { dotr_act_note("SzDuel_GetUnitMonsterCount"); (void)arg0; return 0; }
s32 SzDuel_GetUnitSpellCount(s32 arg0) { dotr_act_note("SzDuel_GetUnitSpellCount"); (void)arg0; return 0; }
s32 SzDuel_InqGrave(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_InqGrave"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_InqHand(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_InqHand"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_IsUnitMovableOnLabyrinth() { dotr_act_note("SzDuel_IsUnitMovableOnLabyrinth"); return 0; }
void *SzDuel_MoveUnit(s32 arg0, s32 arg1, s32 x, s32 y) { dotr_act_note("SzDuel_MoveUnit"); (void)arg0; (void)arg1; (void)x; (void)y; return 0; }
void SzDuel_OpenMsgPopup(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { dotr_act_note("SzDuel_OpenMsgPopup"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; }
s32 SzDuel_SetBusyFlag(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzDuel_SetBusyFlag"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzDuel_SetCardBusyFlag(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzDuel_SetCardBusyFlag"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzDuel_SetFieldRate(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzDuel_SetFieldRate"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzDuel_SetHandTag(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzDuel_SetHandTag"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzDuel_SetUnitCtlGoalM() { dotr_act_note("SzDuel_SetUnitCtlGoalM"); return 0; }
s32 SzDuel_SetUnitCtlRateM(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { dotr_act_note("SzDuel_SetUnitCtlRateM"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }
void SzDuel_SetupDuel() { dotr_act_note("SzDuel_SetupDuel"); }
s32 SzDuel_TrimUnitDir(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { dotr_act_note("SzDuel_TrimUnitDir"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }
s32 SzDuel_TrimUnitDir1(s32 arg0, s32 arg1) { dotr_act_note("SzDuel_TrimUnitDir1"); (void)arg0; (void)arg1; return 0; }
s32 SzDuel_TrimUnitDir2(s32 arg0, s32 arg1, s32 arg2, s32 arg3) { dotr_act_note("SzDuel_TrimUnitDir2"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }
s32 SzEffect_Destroy2(s32 arg0, s32 arg1) { dotr_act_note("SzEffect_Destroy2"); (void)arg0; (void)arg1; return 0; }
s32 SzEffect_Entry(s32 p0, s32 p1, s32 p2, s32 p3, s32 p4) { dotr_act_note("SzEffect_Entry"); (void)p0; (void)p1; (void)p2; (void)p3; (void)p4; return 0; }
s32 SzEffect_GetStatus(s32 arg0) { dotr_act_note("SzEffect_GetStatus"); (void)arg0; return 0; }
s32 SzEffect_IsUse(s32 arg0) { dotr_act_note("SzEffect_IsUse"); (void)arg0; return 0; }
s32 SzEffect_IsUse2(s32 arg0, s32 arg1) { dotr_act_note("SzEffect_IsUse2"); (void)arg0; (void)arg1; return 0; }
s32 SzEffect_LoadModule() { dotr_act_note("SzEffect_LoadModule"); return 0; }
void SzEffect_SetBlackOutForDuel(s32 arg0) { dotr_act_note("SzEffect_SetBlackOutForDuel"); (void)arg0; }
void SzEffect_SetBlackOutForMonster(s32 arg0) { dotr_act_note("SzEffect_SetBlackOutForMonster"); (void)arg0; }
void SzEffect_SetPauseFor3DBattle(s32 arg0) { dotr_act_note("SzEffect_SetPauseFor3DBattle"); (void)arg0; }
void SzEffect_SetPointLightForBattle(s32 arg0) { dotr_act_note("SzEffect_SetPointLightForBattle"); (void)arg0; }
void SzEffect_SetPointLightForMonster(s32 arg0) { dotr_act_note("SzEffect_SetPointLightForMonster"); (void)arg0; }
s32 SzEffect_Sleep(s32 arg0, s32 arg1) { dotr_act_note("SzEffect_Sleep"); (void)arg0; (void)arg1; return 0; }
void SzGame_ComInit(void) { dotr_act_note("SzGame_ComInit"); }
s32 SzGame_GetMenuNum(s32 arg0) { dotr_act_note("SzGame_GetMenuNum"); (void)arg0; return 0; }
void SzGame_Goto(s32 arg0, s32 arg1) { dotr_act_note("SzGame_Goto"); (void)arg0; (void)arg1; }
void SzGame_Pop(void) { dotr_act_note("SzGame_Pop"); }
void SzGame_ReqBGM(int n, int b) { dotr_act_note("SzGame_ReqBGM"); (void)n; (void)b; }
void SzGame_SetAutoBlur(s32 arg0) { dotr_act_note("SzGame_SetAutoBlur"); (void)arg0; }
s32 SzGame_SetMenuNum(s32 arg0, s32 arg1) { dotr_act_note("SzGame_SetMenuNum"); (void)arg0; (void)arg1; return 0; }
s32 SzModel_BattleSeq(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzModel_BattleSeq"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzModel_BattleSeq2(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzModel_BattleSeq2"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzModel_GetAlpha(s32 arg0) { dotr_act_note("SzModel_GetAlpha"); (void)arg0; return 0; }
s32 SzModel_GetAnim() { dotr_act_note("SzModel_GetAnim"); return 0; }
s32 SzModel_GetAnimDir() { dotr_act_note("SzModel_GetAnimDir"); return 0; }
f32 SzModel_GetAnimFrame(s32 arg0) { dotr_act_note("SzModel_GetAnimFrame"); (void)arg0; return 0; }
s32 SzModel_GetCenter(s32 index, void *out) { dotr_act_note("SzModel_GetCenter"); (void)index; (void)out; return 0; }
s32 SzModel_IsModelDisp(s32 arg0) { dotr_act_note("SzModel_IsModelDisp"); (void)arg0; return 0; }
s32 SzModel_IsModelStandby(s32 arg0) { dotr_act_note("SzModel_IsModelStandby"); (void)arg0; return 0; }
s32 SzModel_LoadMerge(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzModel_LoadMerge"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzModel_LoadOverlayB(s32 arg0) { dotr_act_note("SzModel_LoadOverlayB"); (void)arg0; return 0; }
s32 SzModel_SetAlpha(s32 arg0, u8 arg1) { dotr_act_note("SzModel_SetAlpha"); (void)arg0; (void)arg1; return 0; }
s32 SzModel_SetAnimDir(s32 arg0, s64 arg1) { dotr_act_note("SzModel_SetAnimDir"); (void)arg0; (void)arg1; return 0; }
s32 SzModel_SetAnimDirect(s32 arg0, s32 arg1) { dotr_act_note("SzModel_SetAnimDirect"); (void)arg0; (void)arg1; return 0; }
s32 SzModel_SetAnimIntr(s32 arg0, s32 arg1) { dotr_act_note("SzModel_SetAnimIntr"); (void)arg0; (void)arg1; return 0; }
u16 SzPad_Check() { dotr_act_note("SzPad_Check"); return 0; }
u8 SzPad_GetAnalogInfo(s32 arg0, s32 arg1) { dotr_act_note("SzPad_GetAnalogInfo"); (void)arg0; (void)arg1; return 0; }
s32 SzPad_IsAnalog(s32 arg0) { dotr_act_note("SzPad_IsAnalog"); (void)arg0; return 0; }
s32 SzSound_IsLoadingBGM(void) { dotr_act_note("SzSound_IsLoadingBGM"); return 0; }
s32 SzSound_PlayBGM(s32 arg0, s32 arg1) { dotr_act_note("SzSound_PlayBGM"); (void)arg0; (void)arg1; return 0; }
s32 SzSound_PlaySE(s32 arg0, s32 arg1) { dotr_act_note("SzSound_PlaySE"); (void)arg0; (void)arg1; return 0; }
s32 SzText_FreeRequest(s32 arg0, s32 arg1) { dotr_act_note("SzText_FreeRequest"); (void)arg0; (void)arg1; return 0; }
void *SzText_GetReadyItem(s32 arg0, s32 arg1) { dotr_act_note("SzText_GetReadyItem"); (void)arg0; (void)arg1; return 0; }
s32 SzText_SleepRequest(s32 category, s32 id, s32 enabled) { dotr_act_note("SzText_SleepRequest"); (void)category; (void)id; (void)enabled; return 0; }
s32 SzView_FreeRequest(s32 arg0, s32 arg1) { dotr_act_note("SzView_FreeRequest"); (void)arg0; (void)arg1; return 0; }
f32 *SzView_GetLook(s32 arg0) { dotr_act_note("SzView_GetLook"); (void)arg0; return 0; }
s32 SzView_GetMode(s32 arg0) { dotr_act_note("SzView_GetMode"); (void)arg0; return 0; }
s32 SzView_GetSP(s32 arg0) { dotr_act_note("SzView_GetSP"); (void)arg0; return 0; }
void *SzView_InqRequest(s32 arg0, s32 arg1) { dotr_act_note("SzView_InqRequest"); (void)arg0; (void)arg1; return 0; }
void SzView_IV2V(void *arg0, void *arg1) { dotr_act_note("SzView_IV2V"); (void)arg0; (void)arg1; }
s32 SzView_PauseRequest(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("SzView_PauseRequest"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzView_ReqCamera(s32 owner, s32 target, s32 type, s32 count, s32 value, void *data) { dotr_act_note("SzView_ReqCamera"); (void)owner; (void)target; (void)type; (void)count; (void)value; (void)data; return 0; }
s32 SzView_RollCamera(s32 arg0, s32 arg1, s32 arg2, f32 fparg0, f32 fparg1, f32 fparg2) { dotr_act_note("SzView_RollCamera"); (void)arg0; (void)arg1; (void)arg2; (void)fparg0; (void)fparg1; (void)fparg2; return 0; }
s32 SzView_SetCamera(s32 arg0, void *arg1, void *arg2) { dotr_act_note("SzView_SetCamera"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 SzView_SetCP(s32 arg0, void *arg1) { dotr_act_note("SzView_SetCP"); (void)arg0; (void)arg1; return 0; }
s32 SzView_SetInterpCameraMotion(s32 index, void *motion, s32 duration, s32 frame, s32 parent, s32 mirror) { dotr_act_note("SzView_SetInterpCameraMotion"); (void)index; (void)motion; (void)duration; (void)frame; (void)parent; (void)mirror; return 0; }
s32 SzView_SetLookSP(s32 index, f32 *src) { dotr_act_note("SzView_SetLookSP"); (void)index; (void)src; return 0; }
s32 SzView_SetMode(s32 arg0, s32 arg1) { dotr_act_note("SzView_SetMode"); (void)arg0; (void)arg1; return 0; }
s32 SzView_SetViewCamera(s32 index, s32 otherIndex, f32 *target, s32 mode, f32 weight) { dotr_act_note("SzView_SetViewCamera"); (void)index; (void)otherIndex; (void)target; (void)mode; (void)weight; return 0; }
s32 Ta_Do_CompMove(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) { dotr_act_note("Ta_Do_CompMove"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; return 0; }
int Ta_Do_DuelEnd(int nSide, unsigned char bCond) { dotr_act_note("Ta_Do_DuelEnd"); (void)nSide; (void)bCond; return 0; }
s32 Ta_Do_EndPhase(s32 phase, s32 *res) { dotr_act_note("Ta_Do_EndPhase"); (void)phase; (void)res; return 0; }
s32 Ta_Do_HandDestroyEfc(void *arg0, s32 arg1, s32 arg2) { dotr_act_note("Ta_Do_HandDestroyEfc"); (void)arg0; (void)arg1; (void)arg2; return 0; }
int Ta_Do_StandbyPhase(int PrNo, void *eInfo_v) { dotr_act_note("Ta_Do_StandbyPhase"); (void)PrNo; (void)eInfo_v; return 0; }
s32 Ta_DoUncTrapB(void *arg0) { dotr_act_note("Ta_DoUncTrapB"); (void)arg0; return 0; }
int Ta_FinishMove(int nSide, int nID, int nX, int nZ, void *eInfo_v, int PrNo) { dotr_act_note("Ta_FinishMove"); (void)nSide; (void)nID; (void)nX; (void)nZ; (void)eInfo_v; (void)PrNo; return 0; }
int Ta_GetComboResult(int nSide, int nCardNo, int nCardNo2) { dotr_act_note("Ta_GetComboResult"); (void)nSide; (void)nCardNo; (void)nCardNo2; return 0; }
s32 Ta_GetSummonCost(void) { dotr_act_note("Ta_GetSummonCost"); return 0; }
s32 Ta_IsGraveEntryHit(s32 arg0, s32 arg1, s32 arg2) { dotr_act_note("Ta_IsGraveEntryHit"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 Ta_IsHandHeap(s32 nSide0, s32 nID0, s32 nSide1, s32 nID1, s32 *eInfo) { dotr_act_note("Ta_IsHandHeap"); (void)nSide0; (void)nID0; (void)nSide1; (void)nID1; (void)eInfo; return 0; }
s32 Ta_IsTrapBInDecideOperation(s32 arg0, s32 arg1, s32 arg2, s32 *arg3) { dotr_act_note("Ta_IsTrapBInDecideOperation"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; return 0; }
s32 Ta_IsUnitControl(s32 arg0, s32 arg1) { dotr_act_note("Ta_IsUnitControl"); (void)arg0; (void)arg1; return 0; }
s32 Ta_IsUnitHide(s32 arg0, s32 arg1) { dotr_act_note("Ta_IsUnitHide"); (void)arg0; (void)arg1; return 0; }
void Taku_Camp_What_KATIMAKE(s8 p0) { dotr_act_note("Taku_Camp_What_KATIMAKE"); (void)p0; }
s32 Ta_Manage_Effect(u32 *arg0, u16 *arg1, s32 arg2) { dotr_act_note("Ta_Manage_Effect"); (void)arg0; (void)arg1; (void)arg2; return 0; }
s32 Ta_ManageHandEffect() { dotr_act_note("Ta_ManageHandEffect"); return 0; }
s32 Ta_Open(s32 x, s32 y, void *arg2, s32 phase) { dotr_act_note("Ta_Open"); (void)x; (void)y; (void)arg2; (void)phase; return 0; }
s32 Ta_SetDestinyEffect(s32 arg0, s32 *arg1) { dotr_act_note("Ta_SetDestinyEffect"); (void)arg0; (void)arg1; return 0; }
int TaSlot_SetSlotInitialDat(int nSide, void *pSlotOrder_v) { dotr_act_note("TaSlot_SetSlotInitialDat"); (void)nSide; (void)pSlotOrder_v; return 0; }
s32 TaUnit_AddUnitEfc(s32 arg0, s32 arg1, s32 arg2, s32 arg3, void *arg4) { dotr_act_note("TaUnit_AddUnitEfc"); (void)arg0; (void)arg1; (void)arg2; (void)arg3; (void)arg4; return 0; }

int main(void)
{
    u8 *duel = data_00317490.b;

    /* Duel start: set-up step 1 waits for the effect module (state 2 or more), then step 2 with
     * player 1's byte +0x33c below 100 and no file request pending starts the first turn. */
    turnOwner = 1;
    duel[0x350 + 0x33c] = 10;
    duel[0xd52] = 1;
    duel[0xd57] = 1;
    module_info = 2;
    assert(SzDuel_Act() == 0);
    assert(duel[0xd57] == 0x15);
    assert(duel[0xd34] == 0x80);
    assert(called("SzEffect_GetModuleInfo") == 1);
    assert(called("SzFile_InqRequestEx") == 1);
    assert(called("SzDuel_InitFieldRate") == 1);
    assert(called("SzDuel_InitTurn") == 1);
    assert(init_turn_args[0] == 1 && init_turn_args[1] == 1);
    assert(call_count < 64);
    return 0;
}

/* Synthetic host tests for the eff_009 group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/eff_009.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

static int focus2, modules;

int SzDuel_GetMsgName(int arg0) { return 0; }
int SzDuel_GetWindowDisp(void) { return 0; }
void SzDuel_SetWindowDraw(int arg0) {}
int SzDuel_GetUnitOpenRate(int a, int b) { return 0; }
int SzDuel_SetUnitStatus(int arg0, int arg1, int arg2, int arg3) { return 0; }
int SzDuel_GetUnitStatus(int arg0, int arg1, int arg2) { return 0; }
void SzDuel_OpenMsgPopup(int arg0, int arg1, int arg2, int arg3) {}
void SzDuel_CloseMsgPopup(void) {}
int SzEffect_IsUse(int arg0) { return 0; }
int SzEffect_SetCardColBusyFlag2(int arg0, int arg1) { return 0; }
int SzEffect_SetModelBusyFlag2(int arg0, int arg1) { return 0; }
void SzEffect_SetBlackOutForDuel(int arg0) {}
void TaUtil_DoOpen(int arg0, int arg1, int arg2) {}
void InitEffectTemp(void *arg0) { memset(arg0, 0, sizeof(DotrEff009Temp)); }
int Ta_Do_Effect(void *arg0, void *arg1) { return 0; }
int TaEffect_LoadModule(void *arg0, short arg1) { modules++; return 0; }
int TaEffect_IsLoadModule(void *arg0) { return 0; }
void Ta_CalcLPinEffectSeq(int arg0, int arg1, int arg2) {}
void TaView_RestoreOriginalView(void *arg0, int arg1) {}
void TaView_ReqFocusCamera(int arg0, int arg1, int arg2, int arg3) {}
void TaView_ReqBackFocusCamera(int arg0, int arg1, int arg2, int arg3) {}
void TaView_ReqFocusCamera2(int arg0, int arg1, int arg2) { focus2++; }
void TaView_ReqFireView(int arg0, int arg1, unsigned char arg2, unsigned char arg3) {}
int Ta_GetLPRevByNaturalEffect(int arg0, int arg1) { return 500; }
int Ta_IsActiveTrapB(int arg0, int arg1) { return -1; }

int main(void)
{
    /* Set-up call (ID 0), no trap active: the blackout counter starts at 0x80, module 0x1fd is
     * loaded and the camera focuses on the source. */
    DotrEff009Info info = {9, 0x0102, 1, 0x1f4};
    DotrEff009Temp temp;
    assert(Ta_Eff_009(&info, &temp, 0) == 1);
    assert(temp.wCount_1 == 0x80 && modules == 1 && focus2 == 1);
    return 0;
}

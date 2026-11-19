/* Synthetic host tests for the manage_act_efc group. Fixtures are host data, not game data;
 * stubbed callees have assumed semantics. */
/* Smoke test only (docs/DECOMPILATION.md, Tests). */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "dotr/ee/manage_act_efc.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int SzDuel_GetTurnOwner(void) { return 0; }
int SzDuel_GetUnitKind(int arg0, int arg1) { return 0; }
void *SzDuel_GetUnitEffect(int arg0, int arg1, int index) { return 0; }
int SzDuel_GetUnitLoc(int arg0, int arg1) { return 0; }
int ConvUnitIDtoFlagList(unsigned int arg0, int arg1, int arg2) { return 0; }
int ConvUnitIDtoFlagList_OFF(unsigned int arg0, int arg1, int arg2) { return 0; }
int SearchUnitByRange(int arg0, int arg1, int arg2, int arg3, int arg4, int arg5) { return 0; }
int Ta_GetEOwnerSide(int arg0, int arg1) { return 0; }
int Ta_GetCurrentSide(int arg0, int arg1) { return 0; }
int Ta_GetSummonablePiece(int arg0, int arg1) { return 0; }
int Ta_GetUnitNum(int arg0, unsigned char arg1) { return 0; }
int Ta_GetUnitNumFrom22bitFlagList(int arg0) { return 0; }
int Ta_GetAgreeOwnerIDon22bitFlagLst(int arg0, unsigned int arg1) { return 0; }
int Ta_GetRandomUnit(int arg0) { return 0; }
int Ta_GetMinimumOrderUnitFromFlagList(int mask) { return 0; }
int Ta_GetMaximumOrderUnitFromFlagList(int mask) { return 0; }
int Ta_IsUnitControl(int arg0, int arg1) { return 0; }
int TaUtil_GetGraveCardNum(int arg0, int arg1) { return 0; }
void TaUtil_DoOpen(int arg0, int arg1, int arg2) {}
int Ta_SearchUncTypeNaturalEffectUserNum(int arg0) { return 0; }
int Ta_IsActiveTrapB(int arg0, int arg1) { return 0; }
int TaLogic_GetGraveCardCount(int arg0, int arg1, int arg2) { return 0; }
int SzDuel_SetUnitDelayCount(int a0, int a1, int a2) { return 0; }
int SzDuel_GetUnitCardNo(int a0, int a1) { return 0; }

int main(void)
{
    /* Effect group 2 (id 0x200), parameter 8 with target 5 in the high half: the call record
     * is effect 2 from (z << 8) | x to 5 with parameter 8, and the function returns 1. */
    DotrActEfcInfo info = {0x200, (5u << 16) | 8};
    DotrActEfcEffect effect;
    DotrActEfcKind kind = {7, 7};
    assert(Ta_Manage_ActEfc(&info, 0, 1, 0, 0, 3, 4, &effect, &kind, 0) == 1);
    assert(effect.eNO == 2 && effect.dwFrom == 0x403 && effect.dwTo == 5 && effect.dwPar == 8);
    assert(kind.bType == 0 && kind.bKind == 0);
    return 0;
}

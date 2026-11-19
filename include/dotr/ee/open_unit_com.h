#ifndef DOTR_EE_OPEN_UNIT_COM_H
#define DOTR_EE_OPEN_UNIT_COM_H

/* TaLogicCom_IsOpenUnit file-local twins (0x111320, 0x16b610): whether the AI should flip a
 * face-down unit, by unit kind and effect kind.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees (types assumed; names from the Japanese build's symbols). One SzDuel_GetUnitCardNo
 * call relies on the caller's own side and pos still being in the argument registers; here it
 * passes (side, pos) explicitly, and SzDuel_GetUnitCardNo, SzDuel_GetUnitCurseRest,
 * SzDuel_GetGraveCount and SzData_GetRank are prototyped with int results, which compiles to
 * the same bytes. */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetGraveCount(int side);
extern int SzData_GetRank(int side, int cardNo);

extern int SzDuel_GetSummonPower(int side);
extern int SzDuel_InqHandPhase(int side);
extern int SzDuel_InqGrave(int side, int index);
extern int SzDuel_GetHandCount(int side);
extern int SzDuel_GetLP(int side);
extern int SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern void *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetField(int x, int y);
extern void TaUtil_GetUnitLst(int sideToCheck, int compMode, short *units);
extern int SearchUnitByRange(int col, int row, int side, int compMode, int searchMode, int range);
extern int Ta_GetSummonablePiece(int side, int kind);
extern int Ta_GetUnitNum(int side, unsigned char kind);
extern int Ta_GetUnitNumFrom22bitFlagList(int flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int ordinal);
extern int Ta_IsUnitControl(int side, int id);
extern void *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int TaUtil_IsExMonster(int side, int pos);
extern int TaUtil_GetUnitAP(int side, int pos, int col, int row);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);
extern int TaHand_GetCardKind(int card);
extern int TaLogic_GetMaxApUnit(unsigned short *ownerIdArray);
extern int TaLogic_IsAttackedNow(int a, int b, int c);
extern void TaLogic_GetUnitCtlLst(int side, unsigned short *list, int kind, unsigned char flag);
extern void TaLogic_GetUnitCtlLst2(int side, unsigned short *list, int kind, int a, unsigned char flag);
extern int TaLogic_GetLandChange(int unit, int x, int y, int land, int a, int radius);
extern int TaLogic_GetActionReaction(int side, int id, int side2, int pos2, int c, int d);
extern int TaLogic_RemoveHideUnit(int flagList);

/* The file-local helpers each copy calls (names from the Japanese build's symbols). */
extern int TaLogicCom_GetRevoltObj__001110d0(int side);
extern int TaLogicCom_IsUseObjEffect__001123c0(int side, int sidesToMatch, int searchMode);
extern int TaLogicCom_GetUseXYRangeSpell__00112600(int side, int *effect);
extern int TaLogicCom_IsDoCerem__00112850(int side, int card);
extern int TaLogicCom_IsExzParts__00112960(int card);
extern int TaLogic_IsTactixExz__00112980(int side);
extern int TaLogic_IsExzodia__00112a40(int side);
extern int TaLogicCom_GetRevoltObj__0016b3c0(int side);
extern int TaLogicCom_IsUseObjEffect__0016c6b0(int side, int sideComp, int searchMode);
extern int TaLogicCom_GetUseXYRangeSpell__0016cbb0(int side, int *effect);
extern int TaLogicCom_IsDoCerem__0016ce00(int side, int cardNo);
extern int TaLogicCom_IsExzParts__0016cf10(int card);
extern int TaLogic_IsTactixExz__0016cf30(int side);
extern int TaLogic_IsExzodia__0016cff0(int side);

/* 0x111320 and 0x16b610 TaLogicCom_IsOpenUnit (names from the Japanese build's symbols): 0 when
 * the unit's status bit 4 is set, 1 to open it, 2 not to. Assumed: the second copy's third
 * parameter is unused. */
int TaLogicCom_IsOpenUnit__00111320(int side, int pos);
int TaLogicCom_IsOpenUnit__0016b610(int side, int pos, void *info);

#endif

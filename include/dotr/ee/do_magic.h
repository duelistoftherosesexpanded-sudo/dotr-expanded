#ifndef DOTR_EE_DO_MAGIC_H
#define DOTR_EE_DO_MAGIC_H

/* Ta_Do_Magic (0x252ec0, Magic.c): the per-effect-type step that fills the effect record for a
 * unit's magic.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

#include "dotr/ee/magic_open.h"

/* The structure layouts and Ta_DelUnit, Ta_GetCurrentSide and SzDuel_GetUnitLoc come from
 * magic_open.h. Assumed prototypes as declared elsewhere; SearchUnitByRange and
 * Ta_GetSacrificeUnit take the argument counts seen at the call sites; func_0019a5c8 returns
 * the library block's next pseudo-random number (no original name). Ta_SetEffectObjByField
 * keeps small_misc's declaration; Ta_Do_Magic passes its effect record through a cast. */
struct DotrMiscBitWords;
extern int SearchUnitByRange(int, int, int, int, int, int);
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern unsigned ConvUnitIDtoFlagList_OFF(unsigned flagList, int side, int pos);
extern int Ta_RemoveObjFromFlagLst2(int word, int side);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int TaUtil_GetGraveCardNum(int side, int kind);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_GetField(int x, int y);
extern int func_0019a5c8(void);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern void Ta_SetEffectObjByField(struct DotrMiscBitWords *out, int *source);
extern int Ta_GetUnitNum(int side, unsigned char kind);
extern int Ta_GetEOwnerSide(int side, int mode);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int SzDuel_GetSummonPower(int side);
extern void Ta_GetCeremSacrifice(int *Buf, int Obj);
extern int Ta_GetSacrificeUnit(int *sacrifices, int side);

#endif

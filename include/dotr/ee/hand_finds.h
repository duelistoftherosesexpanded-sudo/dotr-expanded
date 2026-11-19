#ifndef DOTR_EE_HAND_FINDS_H
#define DOTR_EE_HAND_FINDS_H

/* TaLogic functions that walk the graveyard slots of a side and of the other side
 * (SzDuel_GetGraveCount / SzDuel_InqGrave, 0x1dee50 / 0x1dee00) and answer
 * owner << 16 | slot, or -1.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees, with the declarations other groups already use. 0x24ca40
 * (SearchUnitByRange): six int arguments and an int result are assumed. */
extern int SzDuel_GetUnitMonsterCount(int side);
extern int TaLogic_GetGraveCardCount(int side, int flags, int id);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int position);

/* 0x117680. -1 when SzDuel_GetUnitMonsterCount(side) is 5 or more, when neither
 * TaLogic_GetGraveCardCount(side, 3, 0) nor TaLogic_GetGraveCardCount(side, 3, 2) is positive (the second is
 * asked only when the first is not), or when
 * SearchUnitByRange(0, 0, side, 1, 0x0179001c, 14) is non-zero. Otherwise the first
 * slot holding id 2 in side's graveyard, then in the other side's
 * (side == 0 ? 1 : 0), then the same two walks for id 0; answers
 * owner << 16 | slot, or -1 when no slot holds either.
 * Assumed: int side and result. */
int TaLogic001_IsUseRevive(int side);

#endif

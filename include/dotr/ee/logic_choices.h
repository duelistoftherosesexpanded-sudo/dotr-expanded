#ifndef DOTR_EE_LOGIC_CHOICES_H
#define DOTR_EE_LOGIC_CHOICES_H

/* TaLogic per-opponent decisions over the duel state: whether logic 011 uses the Life
 * Hourglass, and where logic 018 moves a virus unit.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* Callees of TaLogic011_IsUseLifeHourGlass (all declared elsewhere too): life points, graveyard
 * count and entries, the card's attack/defence word (attack in the high half; unsigned here, so the
 * shift is logical) and kind, a summon order's place (negative when none) and the side's board
 * monster count. Assumed: these types. */
extern int SzDuel_GetLP(int side);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int index);
extern unsigned int TaHand_GetCardAPDP(int id);
extern int TaHand_GetCardKind(int id);
extern int Ta_GetSummonablePieceWithOrder(int side, int i, int avoidLabyrinth);
extern int SzDuel_GetUnitMonsterCount(int side);

/* 0x142040 TaLogic011_IsUseLifeHourGlass (name from the Japanese build's symbols): 1 or 0. */
int TaLogic011_IsUseLifeHourGlass(int side);

/* Callees of TaLogic018_GetVirusMove (all declared elsewhere too), with the arguments this caller
 * passes: the unit's packed location, the unit handle (only passed on; an int),
 * the move range (low four bits used), a clamp, the field kind, the route test (writes a
 * halfword through its last argument), the attack test, the warning value and its key, and
 * the map unit (only tested against null). Assumed: these types. */
struct jp_9a533;
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnit(int side, int pos);
extern int GetMoveRev(int unit, int x, int y);
extern int GetCap(int limit, int value);
extern int SzDuel_GetField(int x, int y);
extern int TaLogic_IsPassRoot(int side, int pos, int col, int row, int destCol, int destRow, short *atkTargetPtr);
extern int TaLogic_IsAttackedNow2(int a, int b, int x, int y, int e);
extern int TaLogic_GetUnitWarning(int nX, int nZ, int nOwnerID, unsigned char bWarnRange, unsigned short bWarnLvl,
                                  unsigned char fView);
extern int TaLogic_GetWarnHideLvl__0015d120(int side);
extern struct jp_9a533 *SzDuel_GetMapUnit(int x, int y);

/* 0x159930 TaLogic018_GetVirusMove (name from the Japanese build's symbols): the target cell
 * as (row << 8) | column, 0x303 when none. */
int TaLogic018_GetVirusMove(int a, int b);

#endif

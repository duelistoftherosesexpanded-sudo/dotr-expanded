#ifndef DOTR_EE_EFC_MONSTER_H
#define DOTR_EE_EFC_MONSTER_H

/* A monster unit's AI command choice from its effects, actions and the board.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* One entry of the 22-entry candidate list the near-unit searches fill: a card or position
 * halfword, an index byte and a flag byte. Assumed: field meanings. */
struct DotrEfcMonsterCandidate {
    unsigned short card;
    unsigned char index;
    unsigned char flags;
};

/* Callees (names from the Japanese build's symbols). The function declares several inside its body
 * without parameters (SzDuel_GetUnitCardNo, TaLogic_GetSommonPiecePreference__00170610,
 * SzDuel_GetUnitCurseRest, SzDuel_GetGraveCount, SzData_GetRank, TaLogic_GetNearUnitL2,
 * TaLogic_GetNearUnitL3, TaLogic_InitIgnoreLst): the prototypes here pass the same words, with int
 * results. Ta_GetUnitEffect returns the unit's effect record of a kind (1, 2, 3) or null: word 0
 * holds the effect id above bit 8 and a parameter in bits 0..5, word 1 a value in its upper half.
 * Assumed: argument meanings from the calls here. */
extern int Ta_IsUnitControl(int y, int x);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnit(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern void TaLogic_InitIgnoreLst(void *work);
extern int SzData_CheckDeck(int side, int deckNo, int cardNo);
extern int SzData_GetCurrentDeck(int side);
extern int TaLogic_GetSommonPiecePreference__00170610(int side, int index);
extern void *Ta_GetUnitEffect(int side, int pos, int kind);
extern int SearchUnitByRange(int col, int row, int side, int compMode, int searchMode, int range);
extern int Ta_GetCurrentSide(int side, int pos);
extern int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
extern void TaLogic022_CommandAtkUnit(void *command);
extern int TaLogic022_IsEmptyCntrGuard(int side, int pos, int c);
extern void TaLogic022_CommandLineUnit(unsigned short *command, int side);
extern int TaLogic_RemoveHideUnit(int flagList);
extern int Ta_GetUnitNumFrom22bitFlagList(int flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int ordinal);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetGraveCount(int side);
extern int SzDuel_InqGrave(int side, int index);
extern int TaHand_GetCardKind(int card);
extern int SzData_GetRank(int side, int cardNo);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int TaLogic_CommandGunnerUnit(unsigned short *command, int a);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int effect);
extern int TaLogic_GetNearUnitL2(int cursor, int card, int col, int row, int side, int attempt, int canSeeFaceDown, int level,
                                 struct DotrEfcMonsterCandidate *list, unsigned short *length, short *first,
                                 short *second, void *work);
extern int TaLogic_GetNearUnitL3(int card, int x, int y, struct DotrEfcMonsterCandidate *list, int chase, int key,
                                 int open, int def, int view, int level, int length);
extern int TaLogic_GetWarnHideLvl__00172310(int side);

/* 0x16e510 TaLogic022_CommandEfcMonster (name from the Japanese build's symbols): fills the
 * command record (halfword 0: the unit as y << 8 | x; halfword 2: the command; word at +8:
 * a target position in its upper half). */
void TaLogic022_CommandEfcMonster(unsigned short *actionUnit);

#endif

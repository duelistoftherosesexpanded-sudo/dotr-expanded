#ifndef DOTR_EE_KIND_PAIRS_H
#define DOTR_EE_KIND_PAIRS_H

/* Functions that relate two small kind numbers through a switch (0x24ea70: no storage, no
 * callees) and a caller that applies it to two cells (0x2679c0).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The first argument is compared unsigned against 6 (so it is unsigned or an enum); the
 * second argument's type and both meanings are assumed. */
extern int ChkAttrAdv(unsigned int attrA, int attrB);

/* Callees of Ta_IsVisualAttrAdv, as the state_records, state_queries and state_grid
 * headers declare them. The item is only passed on, so it stays opaque here. */
struct DotrStateItem;
extern int SzDuel_GetTurnOwner(void);
extern int Ta_GetCurrentSide(int side, int pos);
extern int SzDuel_GetUnitKind(int side, int pos);
extern struct DotrStateItem *SzDuel_GetUnit(int side, int pos);
extern int Ta_IsUnitHide2(struct DotrStateItem *item);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetUnitAttr(int side, int pos);
extern int Ta_SearchUncTypeNaturalEffectUserNum(int kind);

/* Four int arguments read as two (a, b) cell pairs; the meaning of both pairs is assumed. */
extern int Ta_IsVisualAttrAdv(int a, int b, int c, int d);

#endif

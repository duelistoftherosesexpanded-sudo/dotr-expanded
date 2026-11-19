#ifndef DOTR_EE_SACRIFICE_PICK_H
#define DOTR_EE_SACRIFICE_PICK_H

/* Ta_GetSacrificeUnit (0x254890): tries the six orders of three flag lists through
 * Ta_GetScrificeOrder and keeps the lowest-scoring answer.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Callees, defined in small_misc and state_queries; assumed prototypes as declared there. */
extern int Ta_GetScrificeOrder(int *sacrifices, int side);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int Ta_GetAPPlusDP(int side, int pos);

/* 0x254890 Ta_GetSacrificeUnit (name from the Japanese build's symbols). Assumed: three words
 * at sacrifices, side forwarded unread; the answer of the order callee is
 * returned, -1 when no order qualifies. */
int Ta_GetSacrificeUnit(int *sacrifices, int side);

#endif

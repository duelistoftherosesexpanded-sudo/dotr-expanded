#ifndef DOTR_EE_RESOURCE_SUPPORT_H
#define DOTR_EE_RESOURCE_SUPPORT_H

#include "dotr/ee/resource_process.h"

/* Direct dependencies of the resource chain: allocation wrappers, a text copy, card
 * kind/type/attribute and index mappings, and small option queries.
 * Structure layouts and prototypes are inferred, not the original declarations. Addresses
 * are modelled as 32-bit words, as in the callers' record fields.
 * Evidence: docs/modules/duel.md.
 */
extern unsigned short AlternateCardIds[854];
extern unsigned short gSzDuel_CardDefaultCamDist[854];

extern int func_00197990(int alignment, int byte_count);
extern void func_00197c18(int address);
extern int SzData_GetOption(int side, int unknown_1);

int SzDuel_Kind2Type(int kind);
int SzDuel_Kind2Attr(int type, int fallback);
int SzDuel_New2Xax(int cardNo);
int SzDuel_Type2Attr(int value, int fallback);
int SzDuel_GetMsgName(int cardNo);
int SzDuel_GetCamDist(int cardId);
int SzDuel_GetOption3DBattle(void);

#endif

#ifndef DOTR_EE_ATK_EFC_H
#define DOTR_EE_ATK_EFC_H

/* 0x261f20 Manage_AtkEfc and its callee 0x262190 Do_AtkEfc (AtkEfc.c by the Japanese build's debug
 * information): attack-effect evaluation for a pair of units into one result record.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Structure layouts, member names and the prototype below come from the
 * Japanese build's debug information (jp_<n> tags are unnamed there). */
struct jp_570b5 {
    unsigned int Do_APDP : 1;
    unsigned int fDestroy_1 : 1;
    unsigned int fDestroy_2 : 1;
    int POfs1;
    int POfs2;
    int AttrAdv;
    unsigned int fGuard1 : 1;
    unsigned int fGuard2 : 1;
    unsigned int fRemove : 1;
    unsigned int fNoEq1 : 1;
    unsigned int fNoEq2 : 1;
    unsigned int fLand1 : 1;
    unsigned int fLand2 : 1;
};
struct jp_5730a {
    unsigned int dwID;
    unsigned int dwParam;
};

/* Callees: 0x1e0830 and 0x24ea70 as in kind_pairs.h, 0x250740 in the word
 * view of slot_seeks.h (its result is read here as struct jp_5730a). */
extern int SzDuel_GetUnitAttr(int side, int pos);
extern int ChkAttrAdv(unsigned int attrA, int attrB);
extern unsigned *func_00250740_word(int a, int b, int n);
/* Do_AtkEfc (defined in atk_efc_do): prototype from the Japanese build's debug information. */
unsigned int Do_AtkEfc(unsigned int eID, unsigned int eParam, struct jp_570b5 *pDat, int nSide_Do, int ID_Do, int nSide_Done, int ID_Done, unsigned int fAtkSide);

/* 0x261f20: stores ChkAttrAdv of the two units' attributes in pDat->AttrAdv,
 * runs Do_AtkEfc for each unit that has an effect entry (second call with the
 * units swapped and 1), copying bit 24 of each result to fLand1 / fLand2.
 * 0 when neither has an entry and AttrAdv is 0. Otherwise from the low byte
 * and bit 16 of the two results: 0x1010001 both low bytes 1, 0x1000001 first
 * only, 0x10001 second only, then 0x1010000 both bits 16, 0x10000 second
 * only, 0x1000000 first only, else 0. */
unsigned int Manage_AtkEfc(int nSide_1, int ID_1, int nSide_2, int ID_2, struct jp_570b5 *pDat);

/* 0x2626f0: prototype and parameter names from the Japanese build's debug
 * information. SzDuel_GetUnitKind is defined in state_grid; signature as
 * declared there. */
extern int SzDuel_GetUnitKind(int side, int pos);
int Do_Match_AtkEfcObj(int eObj, int nSide, int nID);

#endif

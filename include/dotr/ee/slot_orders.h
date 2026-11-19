#ifndef DOTR_EE_SLOT_ORDERS_H
#define DOTR_EE_SLOT_ORDERS_H

/* Order outcomes chosen by the 0x250740 slot word of the order's position (0x147b90), and the
 * Ta_SetEffectDependOnFlagLst family.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* The order record as these functions use it (same layout as DotrRuleOrder
 * in lower_rules.h): position is (first << 8) | second. */
struct DotrSlotOrder {
    unsigned short position;
    unsigned short unknown_02;
    unsigned short unknown_04;
    unsigned short unknown_06;
    union {
        unsigned short half;
        int word;
    } flags;
};

/* Assumed prototypes; SearchUnitByRange (0x24ca40) and TaLogic_GetLandChange (0x27c770) are viewed
 * here with int parameters. */
extern int func_00250360_pair(int a, int b);
extern unsigned *func_00250740_word(int a, int b, int n);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int func_001df540_word(int a, int b);
extern int TaLogic_GetLandChange(int, int, int, int, int, int);
extern int SearchUnitByRange(int, int, int, int, int, int);
extern int Ta_GetUnitNumFrom22bitFlagList(unsigned flagList);
extern int Ta_GetUnitOwnerIDFromLstCount(int flagList, int n);
extern int TaLogic013_IsUseHarpySweaper(int side, int col, int row);

/* 0x147b90: by the Ta_GetUnitEffect(position, 0) word >> 8, 0x5ff when it is null.
 * 20 stores 0xcff. 2 with (unsigned char)flag of 1 asks TaLogic_GetLandChange(0x1df540
 * answer, cell low, cell high, 2, 6, 2) and for a non-negative answer stores
 * 0x6ff and the answer in the high half, then still falls to the 0x5ff store.
 * 4 with the flag walks the (0, 0, pair, 2, 30, 14) set: for each entry's
 * 0x1e1090 cell, TaLogic013_IsUseHarpySweaper(pair, own low, its high) and then (pair, its low,
 * own high); the first that is 1 stores 0x6ff and that pair of bytes in the
 * high half. Everything else stores 0x5ff. */
void TaLogic013_CommandSpellUnit(struct DotrSlotOrder *order, int flag);

/* 0x25f720: prototype, local names and both layouts from the Japanese build's debug information
 * (unnamed original tags keep their jp_<n> tag). Only the members up to oNum are declared here;
 * the original structure is 0x540 bytes and eMod is a 0x4c-byte structure kept as bytes. */
struct jp_4e65f {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
struct jp_4e704 {
    unsigned short wCount_1;
    unsigned short wCount_2;
    unsigned short wLvl;
    unsigned char eID[24];
    int ePos[24];
    unsigned char eMod[0x4c];
    unsigned int fObj;
    unsigned int fObj2;
    unsigned char oNum;
};
/* Ta_Do_Effect (low_leaves): int return assumed (the caller stores its low byte). */
extern int Ta_Do_Effect(struct jp_4e65f *eInfo, struct jp_4e704 *Temp);
void Ta_SetEffectDependOnFlagLst(struct jp_4e65f *eInfo, struct jp_4e704 *Temp, struct jp_4e65f *pCall, int eNO);
/* 0x25f820 / 0x25f990: same parameters assumed as Ta_SetEffectDependOnFlagLst (names from the symbols);
 * they walk the 22 bits of eInfo->dwPar / eInfo->dwFrom themselves. */
void Ta_SetEffectDependOnFlagLst2(struct jp_4e65f *eInfo, struct jp_4e704 *Temp, struct jp_4e65f *pCall, int eNO);
void Ta_SetEffectDependOnFlagLst3(struct jp_4e65f *eInfo, struct jp_4e704 *Temp, struct jp_4e65f *pCall, int eNO);

#endif

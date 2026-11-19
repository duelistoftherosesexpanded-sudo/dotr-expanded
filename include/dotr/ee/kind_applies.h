#ifndef DOTR_EE_KIND_APPLIES_H
#define DOTR_EE_KIND_APPLIES_H

/* Functions that look an id up in the kind bytes at 0x29d1f0 and the halfword table at
 * 0x29cf32 and apply a per-kind change to a grid cell through state_grid / state_queries
 * callees.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* Tables indexed by id (c & 0x3fff in 0x248460); element widths observed, bounds unknown. */
extern unsigned short gAnmPat_Bakura[];
extern unsigned char EnchantId[];

/* 0x249320; prototype from the Japanese build's debug information (Enchant.c). */
int TaEnc_IsEncDo(int eNO);

/* Callees are defined in state_grid, state_queries and state_records; Enchant is this
 * group's function. SzDuel_GetUnitEffect's result is used as two words here (state_grid
 * declares a struct pointer for the same address). */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int Ta_GetEnchantOfs(void);
extern int Do_ChkEncMatch(unsigned side, int pos, int cardNumber);
extern int Ta_GetForceEffection(int side, int b);
extern int TaUtil_CalcUnitAP(int side, int pos, int operation, int val);
extern int TaUtil_CalcUnitDP(int a, int b, int op, int val);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern unsigned *SzDuel_GetUnitEffect(int side, int pos, int effInd);
extern void Ta_SetUnitCurseRest(int a, int b, unsigned char n);
extern int SzDuel_SetUnitSpellRest(int side, int pos, int value);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
int Enchant(int side, int pos, int c, int teamId);

/* Ta_GetHandEnchant3 (0x249040): name and prototype from the Japanese build's debug
 * information. func_0019a5c8 (library block) is assumed to be a random number
 * source returning int; the low 16 bits of TaEnc_GetRandomElf's result are used. */
extern int func_0019a5c8(void);
int TaEnc_GetRandomElf(void);
int Ta_GetHandEnchant3(int cNo1, int cNo2);

/* Chk_Enc (0x248af0, Enchant.c): name, prototype and the local's name from the Japanese
 * build's debug information. 999 when Do_ChkEncMatch refuses or no case applies; otherwise
 * eNO with a code above bit 14 chosen by the kind byte (EnchantId[eNO]), the value word
 * (gAnmPat_Bakura[eNO]) and, for kind 4 values 8 and 9, the unit's card number; four card
 * numbers draw the code from func_0019a5c8, one from TaEnc_GetRandomElf. What the codes
 * mean is not established. */
unsigned int Chk_Enc(int nSide, int cID, int eNO);

/* Do_Chk_Enc (0x2487a0, Enchant.c): name and prototype from the Japanese build's debug
 * information. Chk_Enc's body with the card number passed in and the match test done in
 * place: bit (eNO - 752) of the card's seven-byte row in EnchMatch (the row width and
 * element type are assumed from the indexing). */
extern unsigned char EnchMatch[];
unsigned int Do_Chk_Enc(int cNO, int eNO);

/* SelfHeap (0x247f40, Heap.c): name, prototype, local names and the structure layout from the
 * Japanese build's debug information (the structure's tag is unnamed there). Fills *eInfo for
 * one unit placed on another and returns 1 or 0x10001; what the event numbers (95, 100, 36,
 * 43, 37) and the two return values mean is not established. The callee prototypes are the
 * ones their own groups declare. */
struct jp_1e41 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
extern int SzDuel_GetUnitKind(int side, int pos);
extern int Fusion(int cardA, int cardB);
extern int Ta_GetCurrentSide(int side, int pos);
int SelfHeap(int nSide_1, int ID_1, int nSide_2, int ID_2, struct jp_1e41 *eInfo);

#endif

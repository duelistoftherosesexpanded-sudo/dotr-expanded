#ifndef DOTR_EE_BATTLE_RESOLVE_H
#define DOTR_EE_BATTLE_RESOLVE_H

/* Battle (0x246810): resolves an attack between two field units into a four-word result and
 * experience calls.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* The attack-effect record Manage_AtkEfc (0x261f20) fills: member names from the Japanese
 * build's debug information (struct jp_570b5 in atk_efc.h). MATCHING DEVICE: the two flag
 * groups are written as unsigned char bit-fields;
 * the debug information declares unsigned int bit-fields, which load and store whole words. */
struct DotrBattleResolveHead {
    unsigned char Do_APDP : 1;
    unsigned char fDestroy_1 : 1;
    unsigned char fDestroy_2 : 1;
    unsigned char bit3 : 1;
    unsigned char bit4 : 1;
    unsigned char bit5 : 1;
    unsigned char bit6 : 1;
    unsigned char bit7 : 1;
};

struct DotrBattleResolveFlags {
    unsigned char fGuard1 : 1;
    unsigned char fGuard2 : 1;
    unsigned char fRemove : 1;
    unsigned char fNoEq1 : 1;
    unsigned char fNoEq2 : 1;
    unsigned char fLand1 : 1;
    unsigned char fLand2 : 1;
    unsigned char bit7 : 1;
};

struct DotrBattleResolveAtkEfc {
    struct DotrBattleResolveHead head; /* +0x00 */
    int POfs1;                         /* +0x04 */
    int POfs2;                         /* +0x08 */
    int AttrAdv;                       /* +0x0c */
    struct DotrBattleResolveFlags flags; /* +0x10 */
};

/* Callees, all defined in other groups (assumed: every value is a plain int; SzDuel_GetUnit's
 * result is only passed on to Ta_GetParamOfs, so it is kept as an int here). Groups: state_grid
 * (unit accessors), atk_efc, perm_efc, small_misc, kind_pairs, side_table, state_queries,
 * kind_applies. */
extern int SzDuel_GetUnitKind(int side, int pos);
extern int Manage_AtkEfc(int nSide_1, int ID_1, int nSide_2, int ID_2, struct DotrBattleResolveAtkEfc *pDat);
extern int SzDuel_GetUnit(int side, int pos);
extern int Ta_GetParamOfs(int unit, int nX, int nZ, int fAP);
extern int SzDuel_GetUnitAP(int side, int pos, int ignoreBoosts);
extern int SzDuel_GetUnitDP(int side, int pos, int ignoreBoosts);
extern int GetParCap(int stat);
extern void SzDuel_CalcUnitAP(int side, int pos, int operation, int value);
extern int SzDuel_CalcUnitDP(int side, int pos, int operation, int value);
extern int Ta_IsLDAbl(int ability, int side);
extern int SzDuel_GetUnitAttr(int side, int pos);
extern int ChkAttrAdv(int attrA, int attrB);
extern int SzDuel_GetUnitCurseRest(int side, int pos);
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int SzDuel_GetUnitLevel(int side, int pos, int ignoreBoosts);
extern int SzData_CalcExp(int side, int cardNo, int operation, int expChange);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int Ta_IsLDRangeAvility(int side, int pos, int ability);
extern int Ta_IsResistAll(int side, int pos);
extern int Chk_Enc(int nSide, int cID, int eNO);
extern int SzDuel_SetUnitStatus(int side, int pos, int bit, int value);
extern int Ta_GetCurrentSide(int side, int pos);

/* 0x246810 Battle: name and parameter names from the Japanese build's debug information
 * (Battle at 0x244780). eInfo receives four words: an event code (0x5d, 0x5e, 0x5f, 0x24,
 * 0x25, 0x2b) and three packed operands. Assumed: the return value is a status word
 * (0 nothing happened, 1 or 0x10001). */
int Battle(int nSide_1, int ID_1, int nSide_2, int ID_2, int nX, int nZ, void *eInfo);

#endif

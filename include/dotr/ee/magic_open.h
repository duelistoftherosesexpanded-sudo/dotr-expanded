#ifndef DOTR_EE_MAGIC_OPEN_H
#define DOTR_EE_MAGIC_OPEN_H

/* Ta_Open from the Japanese build's Magic.c (0x252440): the open/flip step of a unit. Its
 * callees are declared here because small_misc.h declares Ta_GetCurrentSide differently.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Layouts and member names from the Japanese build's debug information for Ta_Open; the
 * `jp_<n>` tags are unnamed there. Only the members Ta_Open touches are confirmed by its code. */
struct jp_1073d {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};
struct jp_107e2 {
    unsigned char bType;
    unsigned char bKind;
};
struct jp_108cd {
    struct jp_1073d eInfo;
    struct jp_107e2 pInfo;
    int fOpen : 1;
};
struct jp_10845 {
    unsigned int dwID;
    unsigned int dwParam;
};

/* Assumed prototypes: argument counts and widths as Ta_Open passes them. Ta_Manage_ActEfc and
 * Ta_Do_Magic are defined elsewhere (Ta_Do_Magic in do_magic); Ta_Manage_ActEfc's
 * parameter types are assumed from the values passed (its ninth and tenth arguments go on the
 * stack). */
extern int SzDuel_GetUnitKind(int side, int pos);
extern struct jp_10845 *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int SzDuel_GetUnitStatus(int side, int pos, int n);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern void TaUtil_DoOpen(int side1, int pos2, int mode);
extern int Ta_Manage_ActEfc(struct jp_10845 *, int, int, int, int, int, int, struct jp_108cd *,
                            struct jp_107e2 *, int);
extern int Ta_SearchUncTypeNaturalEffectUser(int a, int b);
extern int Ta_DelUnit(int side, int pos);
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_IsActiveTrapB(int side, int trapEffect);
extern int SzDuel_GetUnitOpenRate(int side, int pos);
extern int Ta_Do_Magic(int nSide, int nID, struct jp_1073d *eInfo, struct jp_107e2 *pInfo, int PrNo,
                       struct jp_10845 *temp);

int Ta_Open(int nSide, int nID, struct jp_108cd *pDat, int PrNo);

#endif

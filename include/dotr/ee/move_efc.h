#ifndef DOTR_EE_MOVE_EFC_H
#define DOTR_EE_MOVE_EFC_H

/* util.c functions that carry out a unit's effect when it moves (Ta_Do_MoveEfc and its
 * relatives).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel-rules.md. */

/* Names, prototype, locals and the two structure layouts of Ta_Do_MoveEfc come from the Japanese
 * build's debug information (docs/topics/jp-build-symbols.md); jp_<n> tags are unnamed there. */
struct jp_9026 {
    int eNO;
    unsigned int dwFrom;
    unsigned int dwTo;
    unsigned int dwPar;
};

struct jp_8cf7 {
    unsigned int dwID;
    unsigned int dwParam;
};

/* Callee prototypes are assumed as in the groups that define them (state_grid.h,
 * state_queries.h, small_misc.h, low_leaves.h, side_table.h, state_records.h); Ta_GetUnitEffect's
 * result is viewed as the two words this function reads. */
extern int Ta_IsLDAbl(int ability, int side);
extern int SzDuel_GetUnitKind(int side, int pos);
extern int Ta_GetLDChgLand(int Race);
extern int SzDuel_SetField(int x, int y, int value);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int Ta_GetCurrentSide(int side, int pos);
extern int Ta_GetLDRange(int side);
extern int Ta_IsSquareRange(int nX, int nZ, int mX, int mZ, int R);
extern int Ta_IsLD_DestObj(int side, int objKind);
extern unsigned ConvUnitIDtoFlagList(unsigned flagList, int teamId, int unitArrayPos);
extern int Ta_IsLD_CurseObj(int side, int objKind);
extern int SzDuel_GetUnitSpellRest(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern struct jp_8cf7 *Ta_GetUnitEffect(int side, int pos, int effInd);
extern int Ta_GetLPRevByNaturalEffect(int lpChangeType, int lifeAmount);
extern int SzDuel_CalcLP(int side, int operation, int amount);
extern int SzDuel_GetField(int x, int y);
extern int WallFusion(int side, int pos);

/* 0x24f830: the effect of unit (nSide, nID) arriving on square (nX, nZ); fills eInfo and returns
 * nonzero when an effect is to be shown. */
int Ta_Do_MoveEfc(int nSide, int nID, int nX, int nZ, struct jp_9026 *eInfo);

#endif

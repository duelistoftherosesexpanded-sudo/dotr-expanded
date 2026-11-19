#ifndef DOTR_EE_LOGIC012_LINE_H
#define DOTR_EE_LOGIC012_LINE_H

/* TaLogic012 line-unit command (0x142f50) and its line helper (0x143970): they read the order
 * record, the unit location and line lists through callees.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The order record as 0x142f50 uses it: a packed position (hi << 8 | lo), a command word at +4
 * and a result word at +8 whose low half is kept. Same layout as order_tries.h's DotrTryOrder.
 * MATCHING DEVICE: the word is unsigned and the union's first member (with a signed word
 * first, registers are allocated differently). */
struct DotrLineOrder {
    unsigned short position;
    unsigned short unknown_02;
    unsigned short unknown_04;
    unsigned short unknown_06;
    union {
        unsigned int word;
        unsigned short half;
    } flags;
};

/* Local list entry: 22 four-byte entries get 0xffff at +0 and 0xff at +2 and are not read again. */
struct DotrLineCell {
    unsigned short id;
    unsigned char kind;
};

/* Twelve bytes of two four-bit fields, all set to 15, handed to TaLogic_GetInvadeRoot2 as pMovePiece. */
struct DotrLineNibbles {
    unsigned char lo : 4;
    unsigned char hi : 4;
};

/* A unit handle kept in an integer local, as 0x142f50 keeps it (32-bit on the target,
 * pointer-sized on the host). */
#ifdef __MWERKS__
typedef int DotrLineWord;
#else
#include <stdint.h>
typedef intptr_t DotrLineWord;
#endif

/* Callees. The unit handle is typed as TaLogic_GetInvadeRoot2's pVarInfo (it is only tested and
 * passed on); argument meanings are assumed from the callees' own declarations. */
struct jp_9a533;
struct jp_9b2bd;
extern struct jp_9a533 *SzDuel_GetUnit(int side, int pos);
extern int Ta_GetCurrentSide(int side, int pos);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int TaLogic_IsRootOpen(int side, int pos, int c, int d, int e, int f, int g, int h, unsigned char maxDistance);
extern int SzDuel_GetField(int x, int y);
extern int CheckLandAgreement(int field, struct jp_9a533 *unit);
extern int TaLogic012_GetAgreeLineZ(struct jp_9a533 *item);
extern void TaLogic012_CommandBtlUnit(struct DotrLineOrder *order);
extern int TaLogic012_GetLineUnitLst(unsigned short *out, int y, int side, int mask);
extern int TaLogic_GetWarnHideLvl__001461c0(int side);
extern int TaLogic_IsUnitWin4(struct jp_9a533 *unit1, int ownerId, int warnHideLvl, int col, int row, unsigned char canSeeFaceDown);
extern int TaLogic_GetInvadeRoot2(struct jp_9a533 *pVarInfo, struct jp_9b2bd *pMovePiece, int nX, int nZ, int gX, int gZ,
                                  int gUnitID, int fView, unsigned char bAbort, unsigned short bWarnLvl);
extern int GetCap(int limit, int value);
/* 0x143970: the first line y of 0..6 on which no map unit is controlled by the
 * handle's controller and whose field at x 0 agrees with the unit, else -1. The handle type is
 * assumed from the callees; Ta_IsUnitControl2's answer is compared with Ta_IsUnitControl's. */
extern int Ta_IsUnitControl2(struct jp_9a533 *vInfo);
extern int SzDuel_GetMapUnitID(int x, int y);
extern int Ta_IsUnitControl(int a, int b);
int TaLogic012_GetEmptyLine(struct jp_9a533 *item);
extern int func_0019a5c8(void);               /* library block, unnamed */

/* TaLogic012_CommandLineUnit (name from the Japanese build's symbols): chooses the command for
 * the unit at order->position from the open root to the other leader, the land agreement of its cell and
 * the enemy units listed on its line and the next. Parameter type assumed. */
void TaLogic012_CommandLineUnit(struct DotrLineOrder *order);

#endif

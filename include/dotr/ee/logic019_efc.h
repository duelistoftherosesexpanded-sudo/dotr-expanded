#ifndef DOTR_EE_LOGIC019_EFC_H
#define DOTR_EE_LOGIC019_EFC_H

/* TaLogic019 effect-monster command (TaLogic019_CommandEfcMonster, 0x15e950).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

#include "dotr/ee/position_order.h"

/* Same four-byte cell as DotrPositionCell in position_rules.h (not included here). */
struct DotrLogic019Cell {
    unsigned short id;
    unsigned char kind;
    unsigned char unknown_03;
};

/* Callee views as position_rules.h has them. */
extern int func_00250360_pair(int a, int b);           /* Ta_IsUnitControl */
extern int SzDuel_GetUnitCardNo(int side, int pos);
extern int func_001df540_word(int a, int b);           /* SzDuel_GetUnit, result kept as a word */
extern int SzDuel_GetUnitLoc(int, int);
extern void func_0027e770_buf(void *buf);              /* TaLogic_InitIgnoreLst */
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_GetMapUnitID(int x, int y);
int TaLogic_GetWarnHideLvl__00162580(int side);
/* TaLogic_GetNearUnitL2 (0x2770e0) and TaLogic_GetNearUnitL3 (0x277160): argument lists as
 * passed at these calls. */
extern int func_002770e0_cells(int a, int b, int c, int d, int e, int f, int g, int h,
    struct DotrLogic019Cell *cells, unsigned short *j, unsigned short *k, unsigned short *l, unsigned char *buf);
extern int TaLogic_GetNearUnitL3(int a, int b, int c, struct DotrLogic019Cell *cells, int e, int f, int g, int h, int i, int j,
    unsigned short k);
/* Also declared in order_tries.h (DotrTryOrder there) and position_rules.h. */
void TaLogic019_CommandAtkUnit(struct DotrPositionOrder *order);
void TaLogic019_CommandLineUnit(struct DotrPositionOrder *order, unsigned char deep);
void TaLogic019_CommandBtlUnit(struct DotrPositionOrder *order);
void TaLogic019_CommandLDGuardUnit(int row, int column, struct DotrPositionOrder *order);

/* 0x15e950: chooses the command of an effect monster by its card number (43, 87, 297; any
 * other falls to the battle command). One parameter observed; void return assumed. */
void TaLogic019_CommandEfcMonster(struct DotrPositionOrder *order);

#endif

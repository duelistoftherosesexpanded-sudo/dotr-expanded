#ifndef DOTR_EE_INPUT_QUEUE_H
#define DOTR_EE_INPUT_QUEUE_H

/* Builds the pad-input queue of a rule set from one chosen task (0x276290).
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/talogic.md. */

/* The control block and the task as 0x276290 uses them. They are the storage that
 * lower_rules.h declares as DotrRuleSet and DotrRuleStep. The views are kept apart because
 * that header declares this function with an int third argument, which its 21 callers there
 * need to compile to the original, while the function itself needs an unsigned short
 * parameter (as the Japanese build's debug information records it). Assumed from use:
 * `ids` is the queue of pad words, `mode` counts up once per queued task, the bytes of
 * `position` and `target` are a side/row in the high byte and a column/id in the low byte. */
struct DotrInputQueueCtl {
    unsigned char unknown_000[0x160];
    unsigned short ids[32];
    unsigned char mode : 5;
    unsigned char unknown_1a0_5 : 3;
    unsigned char unknown_1a1;
    unsigned short count_1a2;
};
typedef char DotrInputQueueCtlSizeMustBe420[(sizeof(struct DotrInputQueueCtl) == 0x1a4) ? 1 : -1];
struct DotrInputQueueTask {
    unsigned short position;
    unsigned char kind;
    unsigned short target;
    unsigned char slots[5];
    int shouldFlipUp;
    unsigned char atkDefPosition;
};
typedef char DotrInputQueueTaskSizeMustBe20[(sizeof(struct DotrInputQueueTask) == 0x14) ? 1 : -1];

/* Callees as their groups declare them. */
extern int SzDuel_GetTurnOwner(void);
extern int SzDuel_GetUnitLoc(int side, int pos);
extern int SzDuel_GetUnitStatus(int side, int pos, int bit);
extern int SzDuel_InqHandPhase(int side);
extern int TaLogic_GetQueueXY_Move1(int nSide, int nID, int nX, int nZ, int gX, int gZ);

int TaLogic_GetInputQueFromMinimumTask(struct DotrInputQueueCtl *pCtl, struct DotrInputQueueTask *pTask, unsigned short wCursorPos);

#endif

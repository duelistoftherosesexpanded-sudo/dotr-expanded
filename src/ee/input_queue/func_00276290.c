/* Evidence: docs/modules/talogic.md. Names and prototypes are provisional. */
#include "dotr/ee/input_queue.h"

/* Parameter and local names and their declaration order (reversed here) come from the
 * Japanese build's debug information; the macros are this reconstruction's. */
/* Pad words: 2 R2, 4 L1, 8 R1, 64 CROSS, 128 SQUARE, 0x1000 UP, 0x2000 RIGHT, 0x4000 DOWN,
 * 0x8000 LEFT. Task kinds: 6 move to a location, 10 summon, 12 revive, 13 revolt,
 * 14 do nothing. */
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MOVE_X(d) \
    for (i = 0; i < ABS(d); i++) { \
        pCtl->ids[QueCount++] = (d) > 0 ? (tSide == 0 ? 0x8000 : 0x2000) : (tSide == 1 ? 0x8000 : 0x2000); \
    }
#define MOVE_Z(d) \
    for (i = 0; i < ABS(d); i++) { \
        pCtl->ids[QueCount++] = (d) > 0 ? (tSide == 0 ? 0x1000 : 0x4000) : (tSide == 1 ? 0x1000 : 0x4000); \
    }

#define DX (sX - (pTask->target & 0xff))
#define DZ (sZ - (pTask->target >> 8))

int TaLogic_GetInputQueFromMinimumTask(struct DotrInputQueueCtl *pCtl, struct DotrInputQueueTask *pTask, unsigned short wCursorPos)
{
    int i;
    int j;
    int tSide;
    int LOC;
    int HandCursor;
    int PadKey;
    int QueCount;
    int sX;
    int sZ;

    tSide = SzDuel_GetTurnOwner();
    QueCount = 0;
    if (pTask->kind == 255 || pTask->kind == 14) {
        return -1;
    }
    LOC = SzDuel_GetUnitLoc(pTask->position >> 8, pTask->position & 0xff);
    sX = (wCursorPos & 0xff) - (LOC & 0xff);
    MOVE_X(sX)
    sZ = (wCursorPos >> 8) - (LOC >> 8);
    MOVE_Z(sZ)
    switch (pTask->kind) {
    case 6: /* move */
        pCtl->ids[QueCount++] = 64;
        sX = (LOC & 0xff) - (pTask->target & 0xff);
        sZ = (LOC >> 8) - (pTask->target >> 8);
        if (pTask->position & 0xff) {
            if (pTask->shouldFlipUp == 1) {
                pCtl->ids[QueCount++] = 2;
            }
            if (ABS(sX) == 0 && ABS(sZ) == 0) {
                if (pTask->atkDefPosition == 0 && SzDuel_GetUnitStatus(pTask->position >> 8, pTask->position & 0xff, 5)
                    && SzDuel_GetUnitStatus(pTask->position >> 8, pTask->position & 0xff, 5)) {
                    pCtl->ids[QueCount++] = 4;
                } else if (pTask->atkDefPosition == 1 && !SzDuel_GetUnitStatus(pTask->position >> 8, pTask->position & 0xff, 5)
                           && !SzDuel_GetUnitStatus(pTask->position >> 8, pTask->position & 0xff, 5)) {
                    pCtl->ids[QueCount++] = 8;
                }
            }
        }
        switch (TaLogic_GetQueueXY_Move1(pTask->position >> 8, pTask->position >> 8, LOC & 0xff, LOC >> 8, pTask->target & 0xff,
                                         pTask->target >> 8)) {
        case 1:
            MOVE_X(sX)
            MOVE_Z(sZ)
            break;
        default:
            MOVE_Z(sZ)
            MOVE_X(sX)
            break;
        }
        pCtl->ids[QueCount] = 64;
        break;
    case 10: /* summon */
        if (SzDuel_InqHandPhase(tSide)) {
            pCtl->ids[QueCount++] = 128;
            sX = (LOC & 0xff) - (pTask->target & 0xff);
            MOVE_X(sX)
            sZ = (LOC >> 8) - (pTask->target >> 8);
            MOVE_Z(sZ)
            pCtl->ids[QueCount++] = 64;
            HandCursor = 0;
            for (i = 0; i < 5; i++) {
                if (pTask->slots[i] < 255) {
                    if (pTask->slots[i] - HandCursor >= 0) {
                        PadKey = 0x2000;
                    } else {
                        PadKey = 0x8000;
                    }
                    for (j = 0; j < ABS(pTask->slots[i] - HandCursor); j++) {
                        pCtl->ids[QueCount++] = PadKey;
                    }
                    pCtl->ids[QueCount++] = 0x1000;
                    HandCursor = pTask->slots[i];
                }
            }
            pCtl->ids[QueCount++] = 64;
            pCtl->ids[QueCount++] = 64;
        }
        break;
    case 12: /* revive */
        pCtl->ids[QueCount++] = 64;
        pCtl->ids[QueCount++] = 2;
        pCtl->ids[QueCount++] = 64;
        if (pTask->target >> 8 != SzDuel_GetTurnOwner()) {
            pCtl->ids[QueCount++] = SzDuel_GetTurnOwner() == 0 ? 0x2000 : 0x8000;
        }
        for (i = 0; i < (pTask->target & 0xf); i++) {
            pCtl->ids[QueCount++] = 0x4000;
        }
        pCtl->ids[QueCount] = 64;
        break;
    case 13: /* revolt */
        pCtl->ids[QueCount++] = 64;
        pCtl->ids[QueCount++] = 2;
        pCtl->ids[QueCount++] = 64;
        for (i = 0; i < pTask->target; i++) {
            pCtl->ids[QueCount++] = 0x4000;
        }
        pCtl->ids[QueCount] = 64;
        break;
    default:
        pCtl->ids[QueCount] = 64;
        break;
    }
    pCtl->mode++;
    return 1;
}

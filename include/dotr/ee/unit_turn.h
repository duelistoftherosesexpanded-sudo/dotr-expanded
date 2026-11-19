#ifndef DOTR_EE_UNIT_TURN_H
#define DOTR_EE_UNIT_TURN_H

/* SzDuel_TrimUnitDir2 (0x1ddac0): turns a unit step by step toward a board cell.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/state_grid.h"

/* The unit's two halfwords at +0x34 / +0x36 (state_records.h has them as four bytes):
 * an offset from its cell's centre, added to the cell position here. */
#define DOTR_UNIT_OFFSET_X(item) (*(short *)&(item)->unknown_34[0])
#define DOTR_UNIT_OFFSET_Y(item) (*(short *)&(item)->unknown_34[2])

/* Library block; assumed atan2f. */
extern float func_001a6c18(float a, float b);

/* 0x1ddac0 SzDuel_TrimUnitDir2 (name from the Japanese build's symbol table).
 * Turns unit (a, b) toward cell (x, y), at most 2048 per call. When the unit on that cell is
 * the unit itself it forwards to SzDuel_TrimUnitDir1. Otherwise the target point is the
 * cell centre ((n - 3) * 50 through func_001a22c0) plus the offset of the unit standing
 * there, the unit's own point is its cell centre plus its offset, and the wanted direction
 * is the angle between them in 65536ths of a turn, plus half a turn when the byte at +0x44
 * is set, rounded to a multiple of 2048. The direction word at +0x30 moves toward it by the
 * shorter way and is kept in -32767..32768. Returns 1 when it moved, 0 when the unit is
 * missing or already there. Field meanings are assumed from use. */
int SzDuel_TrimUnitDir2(int a, int b, int x, int y);

#endif

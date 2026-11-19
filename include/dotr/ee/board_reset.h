#ifndef DOTR_EE_BOARD_RESET_H
#define DOTR_EE_BOARD_RESET_H

/* Whole-state resets: clear the control block at 0x317490, the 7 x 7 cells and the 3 x 11 item
 * table, then start the next stage.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/state_grid.h"

/* Callees as declared in small_misc.h (not included: it cannot be combined with
 * state_grid.h). SzDuel_InitCardInfo (0x1dcd70) is used through its state_records.h declaration. */
void SzDuel_InitCardRequest(void);
void SzDuel_InitCardControl(void);

/* 0x1ad960: clears the 3440 bytes of the control block (RhMem_FillMemory), calls
 * SzDuel_InitCardRequest and SzDuel_InitCardControl, resets each of the 7 x 7 cells
 * (SzDuel_InitMapInfo on SzDuel_GetMapInfo(x, y)) and stores (x + y) % 10 in cell byte +0x16, resets the 33 items at 0x318280 with id -1, stores
 * 100 in control bytes +0x33c and +0x68c and returns SzDuel_Init(0, 0, 0). 0x1add00 is
 * SzDuel_Init (void in duel_init.h); it is declared here as returning an int that is passed
 * on, because v0 is left untouched after the call and boot_sequence.h declares this
 * function as returning int. */
extern int SzDuel_Init(int map, int side, int fresh);
int SzDuel_Initialize(void);

#endif

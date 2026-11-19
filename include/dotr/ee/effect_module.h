#ifndef DOTR_EE_EFFECT_MODULE_H
#define DOTR_EE_EFFECT_MODULE_H

/* The overlay module loader of the effect system: finds or claims one of 32 module slots and
 * queues the read of a card's code module.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/effects.md. */

#include "dotr/ee/pool_table.h"

/* 0x1f3910 SzEffect_LoadModule (name from the Japanese build's symbol table; declared in
 * pool_table.h). The 32 eight-byte entries of the effect pool are module slots: state 0 empty, 1
 * loading, 2 ready, 3 kept but unused. Slot n owns the 32 KiB window at 0x1ee0000 + n * 0x8000.
 * Assumed: the meaning of the states; the byte at 0x31bea8 (`bCachePos$75` in the Japanese build, a
 * function static) as the rotating start of the search for a slot to reuse; the format at 0x2cfbb0
 * ("%s/Merge/%cModule.Mrg") and the label at 0x2cfbc6 ("HOME") as character data. The remainders
 * are written `% 32L`, which the compiler turns into calls of the 64-bit __moddi3 (0x1a0cf8 in the
 * library block). SzFile_ReadRequestParam is seen here with the overlay callback and a pointer as
 * its fourth argument, so func_001fecb0_module is a second view of the address pool_table.h
 * declares. */
extern unsigned char data_0031bea8;
extern char data_002cfbb0[];
extern int SzDuel_Xax2New(int index);
extern void *func_001fecb0_module(int type, int priority, char *name, void *address, int byte_size, int byte_offset,
                                  int unknown_010, void (*callback)(struct DotrPoolOverlayRecord *record, int completed),
                                  int slot);

#endif

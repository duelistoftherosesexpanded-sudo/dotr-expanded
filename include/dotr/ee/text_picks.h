#ifndef DOTR_EE_TEXT_PICKS_H
#define DOTR_EE_TEXT_PICKS_H

/* Leaf functions that choose a number in the 2482..2523 range and a small code from a kind and a
 * flag word, through a jump table and compare chains.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/game-modes.md. */

/* 0x1ac290. Stores a number through `out` and answers a code 0..3.
 * `on` is bit 7 of flags. The store is first 2482 + on; then by flags & 0x7f:
 *   1: 2521, code 2;  2, 4: 2485 + on, code 1;  3, 14: 2506 + on, code 2;
 *   5: 2491 when kind is 6, else 2490 (2493 when on), code 1;
 *   6: 2488 + on, code 1;  7: 2495 (2497 when on), code 1;
 *   8, 10: 2503 for kind 0, 3 or 4, else 2520, code 1;
 *   9: 2501 for kind 4, 2518 for kind 5, 2499 + on for kind 0 or 3, else
 *      2516 + on, code 0;
 *   11: 2510 + on, code 0;  12: 2512 + on, code 3;  13: 2514 + on, code 1;
 *   0 and 15 or more: nothing further, code 0.
 * Last, by flags & 0xff00: 0x600 stores 2523; 0x100..0x500 store 2522 for
 * kind 4, 2502 for kind 0 or 3, else 2519; other values leave the store.
 * Assumed: int kind, flags and result, int *out. The jump table is at
 * 0x2be790 (15 entries). */
int lSzGame_GetMCardMsg(int kind, int flags, int *out);

#endif

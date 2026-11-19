#ifndef DOTR_EE_VECTOR_DEGREES_H
#define DOTR_EE_VECTOR_DEGREES_H

/* Converts a 2D vector into a whole-degree angle (0 to 360) by quadrant and a linear 45-degree
 * ratio, without trigonometry.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/other.md. */

/* func_00195070 (library block, 20 bytes, declared only): the absolute value of an int (the
 * Japanese build names it abs). MATCHING DEVICE: it is called through this address-named
 * declaration; the name `abs` would be expanded inline by the compiler. The 64-bit divisions
 * below are compiled into calls of __divdi3 (0x1a0660, library block). */
extern int func_00195070(int v);

/* 0x1082b0 tanD_360_ (original name): the direction of (x, y) in whole degrees, 0 for the zero
 * vector. Quadrant from y >= 0 and x > 0; equal magnitudes give 45, 135, 225 or 315; otherwise
 * 45 * smaller / larger (64-bit, truncating) is laid off from 0, 90, 180, 270 or 360, so the
 * result runs from 0 to 360. ASSUMED: x and y are 64-bit (the original tests the whole
 * registers and narrows them only for abs). */
int tanD_360_(long x, long y);

#endif

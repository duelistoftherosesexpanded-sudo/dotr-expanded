#ifndef DOTR_EE_BLOCK_SETUP_H
#define DOTR_EE_BLOCK_SETUP_H

/* 0x1f7550: writes the starting values of the block at table slot 0x31beb0[index] and calls four
 * block_table routines.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/camera.md. */

#include "dotr/ee/block_table.h"

/* lSzView_InitVars: the float values stored are the decoded constants (0x42480000 = 50.0f
 * and so on); the four words at +0x580 of element 9 are stored as integers
 * (SzView_SetStraightLightDirection hands them to RhGeom_GetRotXYZMatrix as int angles).
 * The vectors at +0x20 and +0x30 are the ones block_table.h names in struct DotrBlockDelta;
 * the floats at +0x48..+0x54 have no member and go through a cast. */
int lSzView_InitVars(int index);

#endif

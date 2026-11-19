#ifndef DOTR_EE_UNIT_REQUEST_H
#define DOTR_EE_UNIT_REQUEST_H

/* The per-unit request step of the duel map (lSzDuel_DoUnitRequest at 0x1dd2d0): fades of a state
 * item's brightness, angles, alpha and scale, applied to its model; state_grid.h types.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/duel.md. */

#include "dotr/ee/state_grid.h"

/* Second view of the object an item points to (struct DotrStateItemObject in
 * state_grid.h): lSzDuel_DoUnitRequest stores the words at +0x14 and +0x18 as
 * integers (angles in 1/65536 turns, assumed), where state_grid.h's view has
 * floats. Cast at the single use. */
struct DotrUnitRequestObject {
    unsigned char unknown_00[0x14];
    int unknown_14;
    int unknown_18;
    unsigned char unknown_1c[0x14];
    struct DotrStateVector unknown_30;
    struct DotrStateCellQuad unknown_40;
};

/* Callees, with the parameter types this caller shows. SzModel_SetScale is
 * given a 16-byte stack vector (object_table.h: struct DotrObjectVector4 *,
 * the same layout); the group does not include object_table.h. */
extern int SzGame_GetMode(int a);
extern int SzModel_IsModelStandby(int index);
extern int SzModel_SetAlpha(int index, int value);
extern int SzModel_SetScale(int index, struct DotrStateVector *in);

/* lSzDuel_DoUnitRequest itself is declared in state_grid.h (its caller's
 * view: the unit is passed as the cell's first word). */

#endif

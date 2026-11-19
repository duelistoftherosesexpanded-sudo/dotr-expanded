#ifndef DOTR_EE_VIEW_REBUILD_H
#define DOTR_EE_VIEW_REBUILD_H

/* Functions that recompute the matrices of the selected 416-byte view entry at 0x32a2a0 from its
 * stored frame and scalars.
 * Structure layouts and prototypes are inferred, not the original declarations.
 * Evidence: docs/modules/render-files.md. */

#include "dotr/ee/view_state.h"

/* As declared in draw_state.h. */
extern int RhBase_GetZMinMax(int *a, int *b);
extern float RhBase_GetAspectY(int *out);
extern int RhBase_GetScreenCenter(int *a, int *b);
/* In view_state (0x20d5e0); argument types assumed from the one call (two float groups inside the
 * entry's frame and the word at frame +0x80). */
extern void RhGeom_CreateCameraMatrix2(struct DotrViewMatrix *out, float *a, float *b, int c);

#endif

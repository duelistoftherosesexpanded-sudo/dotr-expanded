#ifndef DOTR_EE_CURSOR_PAD_GEOM_H
#define DOTR_EE_CURSOR_PAD_GEOM_H

/* Geometry callees of lSzDuel_DrawWindow_DualShock2_Pipe (0x1b7820), declared as in
 * leader_base.h. They are not in cursor_packets.h because view_state.h, which some includers
 * of that header also include, declares RhGeom_PushMatrix with an int argument. The
 * prototypes are assumed, not recovered declarations. */
extern void RhGeom_GetRotXYZMatrix(float *m, int *ang);
extern void RhGeom_PushMatrix(float *matrix);
extern void RhGeom_MulMatrixVectorArray(float *out, float *m, float *in, int count);
extern void RhGeom_PopMatrix(void);

#endif

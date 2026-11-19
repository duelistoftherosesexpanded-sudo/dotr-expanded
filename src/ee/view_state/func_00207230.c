#include "dotr/ee/view_state.h"

/* Projects count vertices in place. A null matrix is replaced by the one
 * RhView_GetViewScreenMatrix builds. Each position gets w = 1, goes through RhGeom_MulMatrixVector,
 * and x * 16, y * 16 and z, each divided by the resulting w, are converted by
 * func_001a23b0 into the packed members; the top member is cleared. */
void Ex3d_NodeV2S(struct DotrViewMatrix *matrix, int count, struct DotrViewVertex *vertex)
{
    struct DotrViewMatrix local;
    int i;
    struct DotrViewPacked *screen;
    float scale;

    if (matrix == 0) {
        RhView_GetViewScreenMatrix(&local);
        matrix = &local;
    }
    RhGeom_PushMatrix((int)(DotrViewWord)matrix);
    for (i = 0; i < count; i++) {
        screen = &vertex->screen;
        vertex->position.unknown_w = 1.0f;
        RhGeom_MulMatrixVector(&vertex->position, 0, &vertex->position);
        scale = 1.0f / vertex->position.unknown_w;
        screen->x = func_001a23b0(16.0f * (vertex->position.x * scale));
        screen->y = func_001a23b0(16.0f * (vertex->position.y * scale));
        screen->z = (unsigned)func_001a23b0(vertex->position.z * scale);
        screen->f = 0;
        vertex++;
    }
    RhGeom_PopMatrix();
}

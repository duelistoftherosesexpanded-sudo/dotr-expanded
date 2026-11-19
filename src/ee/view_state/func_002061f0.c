#include "dotr/ee/view_state.h"

/* Same as Ex3d_GetViewVector with a 48-byte stride: the mean of (origin - point) over count
 * points, with a null origin read as zero, stored in out, normalized, and 1 returned; 0 with
 * out cleared if count is 0. */
int Ex3d_GetViewVectorForNode(struct DotrViewVector *origin, int count, struct DotrViewVectorTriple *points, struct DotrViewVector *out)
{
    int i;
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    out->unknown_w = 0.0f;
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        out->x += (origin ? origin->x : 0.0f) - points->unknown_00.x;
        out->y += (origin ? origin->y : 0.0f) - points->unknown_00.y;
        out->z += (origin ? origin->z : 0.0f) - points->unknown_00.z;
        points++;
    }
    out->x /= count;
    out->y /= count;
    out->z /= count;
    RhGeom_NormalizeVector(out);
    return 1;
}

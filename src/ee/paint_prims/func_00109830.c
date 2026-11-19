#include "dotr/ee/paint_prims.h"

/* 0x109830 R3D_RRR_XYZ (name from the Japanese build's symbols): out = a - b for x, y and z;
 * w is a's. MATCHING DEVICE: y and z are each reached as the first member of a vector view at
 * their address. */
void R3D_RRR_XYZ(struct DotrPaintVector *out, struct DotrPaintVector *a_in, struct DotrPaintVector *b_in)
{
    struct DotrPaintVector a;
    struct DotrPaintVector b;

    a = *a_in;
    b = *b_in;
    a.x = a.x - b.x;
    ((struct DotrPaintVector *)&a.y)->x = ((struct DotrPaintVector *)&a.y)->x - b.y;
    ((struct DotrPaintVector *)&a.z)->x = ((struct DotrPaintVector *)&a.z)->x - b.z;
    *out = a;
}

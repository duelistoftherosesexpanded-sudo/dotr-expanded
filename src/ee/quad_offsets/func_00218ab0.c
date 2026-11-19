#include "dotr/ee/quad_offsets.h"

/* As Taku_G4_Set, with width added to the x of the second and fourth points
 * and height to their y. */
DotrDrawWord Taku_G4_Set2(DotrDrawWord cursor, struct DotrQuadRecord *quad, short dx, short dy, short width, short height)
{
    return Taku_Prim_Set_G4(cursor, quad->points[0] + dx, quad->points[1] + dy, quad->points[2] + dx + width,
                         quad->points[3] + dy + height, quad->points[4] + dx, quad->points[5] + dy,
                         quad->points[6] + dx + width, quad->points[7] + dy + height, quad->bytes[0], quad->bytes[1], quad->bytes[2], quad->bytes[3], quad->bytes[4], quad->bytes[5], quad->bytes[6], quad->bytes[7], quad->bytes[8], quad->bytes[9], quad->bytes[10], quad->bytes[11], quad->bytes[12], quad->bytes[13], quad->bytes[14], quad->bytes[15],
                         quad->mode, quad->unknown_22);
}

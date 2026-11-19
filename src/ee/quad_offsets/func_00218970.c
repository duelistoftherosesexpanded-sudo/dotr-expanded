#include "dotr/ee/quad_offsets.h"

/* Forwards the record to Taku_Prim_Set_G4 with (dx, dy) added to each of its four
 * points; every sum is truncated to a halfword. */
DotrDrawWord Taku_G4_Set(DotrDrawWord cursor, struct DotrQuadRecord *quad, short dx, short dy)
{
    return Taku_Prim_Set_G4(cursor, quad->points[0] + dx, quad->points[1] + dy, quad->points[2] + dx,
                         quad->points[3] + dy, quad->points[4] + dx, quad->points[5] + dy,
                         quad->points[6] + dx, quad->points[7] + dy, quad->bytes[0], quad->bytes[1], quad->bytes[2], quad->bytes[3], quad->bytes[4], quad->bytes[5], quad->bytes[6], quad->bytes[7], quad->bytes[8], quad->bytes[9], quad->bytes[10], quad->bytes[11], quad->bytes[12], quad->bytes[13], quad->bytes[14], quad->bytes[15],
                         quad->mode, quad->unknown_22);
}

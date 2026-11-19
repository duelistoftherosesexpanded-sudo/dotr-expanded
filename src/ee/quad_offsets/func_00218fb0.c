#include "dotr/ee/quad_offsets.h"
/* As Taku_G4_Set2, for a record that also names a row of the 0x2950a0 table:
 * the row's eight halfwords and its halfword at +16 are forwarded as well. */
DotrDrawWord Taku_GT4_Set(DotrDrawWord cursor, struct DotrQuadIndexedRecord *record, short dx, short dy, short width, short height)
{
    struct DotrQuadSheetRow *row = (struct DotrQuadSheetRow *)&tex_uv_data[record->row];

    return Taku_Prim_Set_GT4(cursor, record->quad.points[0] + dx, record->quad.points[1] + dy, record->quad.points[2] + dx + width,
                         record->quad.points[3] + dy + height, record->quad.points[4] + dx, record->quad.points[5] + dy,
                         record->quad.points[6] + dx + width, record->quad.points[7] + dy + height,
                         row->values[0], row->values[1], row->values[2], row->values[3], row->values[4], row->values[5], row->values[6], row->values[7],
                         record->quad.bytes[0], record->quad.bytes[1], record->quad.bytes[2], record->quad.bytes[3], record->quad.bytes[4], record->quad.bytes[5], record->quad.bytes[6], record->quad.bytes[7],
                         record->quad.bytes[8], record->quad.bytes[9], record->quad.bytes[10], record->quad.bytes[11], record->quad.bytes[12], record->quad.bytes[13], record->quad.bytes[14], record->quad.bytes[15],
                         record->quad.mode, row->unknown_10, record->quad.unknown_21, record->quad.unknown_22);
}

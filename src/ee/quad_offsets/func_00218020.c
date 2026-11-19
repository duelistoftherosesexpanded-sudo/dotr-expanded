#include "dotr/ee/quad_offsets.h"
/* A textured quad from a record that names a row of the 0x2950a0 table. Mode 0
 * forwards the row's eight halfwords as they are; any other mode goes to the
 * second builder, with 1 taken off the even halfwords (mode 1), the odd ones
 * (mode 2) or both (mode 3). */
DotrDrawWord Taku_FT4_Set(DotrDrawWord cursor, struct DotrQuadIndexedRecord *record, short dx, short dy, unsigned short mode)
{
    struct DotrQuadSheetRow *row = (struct DotrQuadSheetRow *)&tex_uv_data[record->row];
    int du = 0;
    int dv = 0;

    if (mode == 0) {
        cursor = Taku_Prim_Set_FT4(cursor, record->quad.points[0] + dx, record->quad.points[1] + dy, record->quad.points[2] + dx,
                                   record->quad.points[3] + dy, record->quad.points[4] + dx, record->quad.points[5] + dy,
                                   record->quad.points[6] + dx, record->quad.points[7] + dy,
                                   row->values[0], row->values[1], row->values[2], row->values[3], row->values[4], row->values[5], row->values[6], row->values[7],
                                   record->quad.bytes[0], record->quad.bytes[1], record->quad.bytes[2], record->quad.bytes[3],
                                   record->quad.mode, row->unknown_10, record->quad.unknown_21, record->quad.unknown_22);
    } else {
        if (mode == 1) {
            du = 1;
        } else if (mode == 2) {
            dv = 1;
        } else if (mode == 3) {
            du = 1;
            dv = 1;
        }
        cursor = Taku_Prim_Set_FT4_ST(cursor, record->quad.points[0] + dx, record->quad.points[1] + dy, record->quad.points[2] + dx,
                                      record->quad.points[3] + dy, record->quad.points[4] + dx, record->quad.points[5] + dy,
                                      record->quad.points[6] + dx, record->quad.points[7] + dy,
                                      row->values[0] - du, row->values[1] - dv, row->values[2] - du, row->values[3] - dv,
                                      row->values[4] - du, row->values[5] - dv, row->values[6] - du, row->values[7] - dv,
                                      record->quad.bytes[0], record->quad.bytes[1], record->quad.bytes[2], record->quad.bytes[3],
                                      record->quad.mode, row->unknown_10, record->quad.unknown_21, record->quad.unknown_22);
    }
    return cursor;
}

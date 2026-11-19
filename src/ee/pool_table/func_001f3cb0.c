#include "dotr/ee/pool_table.h"

void lSzEffect_LoadOverlayCB(struct DotrPoolOverlayRecord *record, int completed)
{
    struct DotrPoolEntry8 *entry = &gSzEffect_el_pool.unknown_000[record->unknown_1c];
    struct DotrPoolEntryState *state = (struct DotrPoolEntryState *)entry;
    char name[128];


    if (completed != 0) {
        if (func_0019c100(record->unknown_14, data_002cfbcb, 3) != 0) {
            if (record->flag_00c) {
                SzMem_Free(record->unknown_14);
            }
            completed = 0;
        }
        if (state->state != 1) {
            completed = 0;
        }
    }
    if (completed == 0) {
        state->state = 0;
    } else {
        *(short *)entry->unknown_02 = record->byte_size;
        if (entry->unknown_bits_3_5 == 0) {
            int offset = entry->unknown_bits_6_15 << 11;
            func_0019b648(name, data_002cfbcf, SzFile_GetEnv(data_002cfbc6));
            record = (struct DotrPoolOverlayRecord *)SzFile_ReadRequestParam(3072, 0, name, 0, 2048, offset, 2048,
                                        lSzEffect_LoadSECtlCB, record->unknown_1c);
            if (record == 0) {
                state->state = 3;
            }
        } else {
            state->state = 3;
        }
    }
}

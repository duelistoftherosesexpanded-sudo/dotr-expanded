#include "dotr/ee/resource_process.h"

int lSzFile_DoRequestMain(int index)
{
    struct DotrResourceSubmitState *state = &dotr_ee_resource_submit_state_0031c2f0;
    struct DotrResourceSubmitRecord *record;
    char path[128];
    int result;
    struct DotrResourceReadMode mode;

    if (index < 0) {
        return 0;
    }
    record = (struct DotrResourceSubmitRecord *)&state->records[index];
    if (record->sector_mode) {
        int sector;
        int sectors;
        if (state->flags.bit_1) {
            return 0;
        }
        if (!record->flag_008) {
            record->flag_008 = -1;
        }
        if (func_00191a58(1) != 2) {
            return 0;
        }
        mode = data_00293cb0[0];
        if (record->byte_size && !record->unknown_014) {
            if (!(record->unknown_014 = SzMem_Alloc(record->byte_size))) {
                lSzFile_TermRequest(index, 0);
                return 0;
            }
            record->flag_00c = -1;
        }
        if (record->unknown_010 < 0) {
            record->unknown_010 = record->byte_size - record->byte_offset;
        }
        sector = record->status + (record->byte_offset >> 11);
        sectors = record->unknown_010 >> 11;
        func_0017bb60(0);
        if (!func_00191d18(sector, sectors, record->unknown_014, &mode)) {
            return 0;
        }
        state->flags.bit_1 = -1;
    } else {
        state->flags.bit_1 = -1;
        if (!record->flag_008) {
            func_0019b648(path, data_002d0648, data_002d064d, record->handle);
            if (record->byte_size < 0) {
                if ((record->status = func_0017fa98(path, 1)) < 0) {
                    lSzFile_TermRequest(index, 0);
                    return 0;
                }
                if ((record->byte_size = func_0017fea0(record->status, 0, 2)) < 0) {
                    lSzFile_TermRequest(index, 0);
                    return 0;
                }
                func_0017fd20(record->status);
            }
            record->status = func_0017fa98(path, 0x8001);
            if (record->status < 0) {
                lSzFile_TermRequest(index, 0);
                return 0;
            }
            if (record->byte_size) {
                record->byte_size = (record->byte_size + 15) & ~15;
                if (!record->unknown_014) {
                    if (!(record->unknown_014 = SzMem_Alloc(record->byte_size))) {
                        lSzFile_TermRequest(index, 0);
                        return 0;
                    }
                    record->flag_00c = -1;
                }
            }
            if (record->unknown_010 < 0) {
                record->unknown_010 = record->byte_size - record->byte_offset;
            }
            record->unknown_010 = (record->unknown_010 + 15) & ~15;
            if (record->byte_offset) {
                if (func_0017fea0(record->status, record->byte_offset, 0) < 0) {
                    lSzFile_TermRequest(index, 0);
                    return 0;
                }
                do {
                    if (func_00180608(record->status, 1, &result) < 0) {
                        lSzFile_TermRequest(index, 0);
                        return 0;
                    }
                } while (result);
            }
            func_0017bb60(0);
            if (func_001800d8(record->status, record->unknown_014, record->unknown_010) < 0) {
                lSzFile_TermRequest(index, 0);
                return 0;
            }
            record->flag_008 = -1;
        }
        if (func_00180608(record->status, 1, &result) < 0) {
            lSzFile_TermRequest(index, 0);
            return 0;
        }
        if (!result) {
            func_0017bb60(0);
            lSzFile_TermRequest(index, 1);
            return 1;
        }
    }
    return 0;
}

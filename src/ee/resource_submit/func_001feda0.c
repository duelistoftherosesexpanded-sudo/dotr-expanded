#include "dotr/ee/resource_submit.h"

int lSzFile_SetReadRequestParam(struct DotrResourceSubmitRecord *record, int fileType, int priority, char *name,
                  int unknown_014, int byte_size, int byte_offset, int unknown_010,
                  DotrResourceSubmitCallback callback, int unknown_01c)
{
    char path[128];
    char *p;
    unsigned int i;
    int length;

    if (!record) {
        return 0;
    }
    if (!name) {
        return 0;
    }
    if (func_00197548(data_002d0638) && !((DotrResourceSubmitWord)name & 0x80000000)) {
        func_0019bdd8(path, name);
        if ((p = func_0019c478(path, '/')) != 0 || (p = func_0019c478(path, '\\')) != 0) {
            length = func_0019bef0(p + 1);
            if (length >= 8 && fileType == 0x1400
                && (p[1] == 'M' || p[1] == 'm')
                && p[2] >= '0' && p[2] <= '9'
                && p[3] >= '0' && p[3] <= '9'
                && p[4] >= '0' && p[4] <= '9'
                && p[5] == '.'
                && (p[6] == 'G' || p[6] == 'g')
                && (p[7] == 'R' || p[7] == 'r')
                && (p[8] == 'P' || p[8] == 'p')) {
                byte_offset += SzDuel_Xax2New((p[2] - '0') * 100 + (p[3] - '0') * 10 + (p[4] - '0'))
                               * 257 << 11;
                length = func_0019bef0((p = data_002d063d) + 1);
            }
            for (i = 0; i < 97; i++) {
                char *q = func_0019c478(dotr_ee_resource_submit_entries_00293090[i].name, '\\');
                if (q) {
                    if (!func_0019c028(p + 1, q + 1, length)) {
                        name = (char *)(dotr_ee_resource_submit_entries_00293090[i].first_sector
                                        | 0x80000000);
                        if (byte_size < 0) {
                            byte_size = dotr_ee_resource_submit_entries_00293090[i].sector_count << 11;
                        }
                        break;
                    }
                }
            }
        }
    }
    if ((DotrResourceSubmitWord)name & 0x80000000) {
        if (byte_size < 0) {
            return 0;
        }
        if (byte_size > 0 && byte_size % 2048) {
            return 0;
        }
        if (byte_offset % 2048) {
            return 0;
        }
        if (unknown_010 > 0 && unknown_010 % 2048) {
            return 0;
        }
        record->handle = 0;
        record->sector_mode = -1;
        record->status = (DotrResourceSubmitWord)name & 0x7fffffff;
    } else {
        if (!(record->handle = SzMem_StrDup(name))) {
            return 0;
        }
        lSzFile_StrNormalize((char *)(DotrResourceSubmitWord)record->handle);
        record->sector_mode = 0;
        record->status = 0;
    }
    record->selector = fileType;
    record->priority = priority > 0 ? priority : (priority = 0);
    record->byte_size = byte_size;
    record->byte_offset = byte_offset;
    record->unknown_010 = unknown_010;
    record->flag_008 = 0;
    record->flag_00c = 0;
    record->unknown_014 = unknown_014;
    record->callback = callback;
    record->unknown_01c = unknown_01c;
    return 1;
}

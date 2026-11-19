#include "dotr/ee/text_load.h"

/* MATCHING DEVICE: the statement order after the group test (slot, data, unit, kind, remain),
 * one SzText_AddFile copy per case and the goto. */
void lSzText_LoadFileCB(void *record, int completed)
{
    struct DotrTextLoadRecord *req = record;
    struct DotrTextLoadEntry *entry = req->entry;

    int *data;
    int kind;
    int index;
    int remain;
    int slot;
    int offset;
    int *bound;
    unsigned short *ids;
    int step;
    int count;
    int id;
    int unit;

    if (completed) {
        if (*req->data != 0x324852) { /* the "RH2" image magic */
            if (req->owned) {
                SzMem_Free((DotrTextLoadWord)req->data);
            }
            completed = 0;
        }
    }
    if (completed == 0) {
        if (entry->unknown_004) {
            SzMem_Free(entry->unknown_004);
            entry->unknown_004 = 0;
        }
        entry->unknown_000 = 0;
        return;
    }
    entry->unknown_000 = (DotrTextLoadWord)req->data;
    if (entry->group != 0) {
        goto bind;
    }
    slot = 0;
    data = (int *)entry->unknown_000;
    unit = data[2];
    kind = entry->id & 0xf;
    remain = req->byte_size;
    if (kind == 0) {
        slot = (entry->id >> 4) * 100;
    }
    offset = slot * unit;
    index = 0;
    ids = picpackArts;
    count = unit >> 4;
    for (;;) {
        id = index;
        bound = 0;
        step = unit;
        if (*data == 0x324852) {
            bound = data;
            if (kind == 0 || kind == 2) {
                id = slot;
                if (slot != 0) {
                    bound = DOTR_TEXT_LOAD_COPY_AT(offset);
                    RhMem_CopyMemory16(bound, data, count);
                }
                offset += unit;
                slot++;
                if (kind == 0) {
                    step = 0x4800;
                } else {
                    step = 0x1000;
                }
            } else if (kind == 1) {
                id = *ids;
            } else {
                step = data[2];
            }
            RhText_BindAddress(bound);
        } else if (remain < unit) {
            return;
        }
        switch (kind) {
        case 0:
            SzText_AddFile(1, id, 0, (DotrTextLoadWord)bound);
            break;
        case 1:
            SzText_AddFile(1, id, 0, (DotrTextLoadWord)bound);
            break;
        case 2:
            SzText_AddFile(2, id, 0, (DotrTextLoadWord)bound);
            break;
        case 3:
            SzText_AddFile(3, id, 0, (DotrTextLoadWord)bound);
            break;
        case 4:
            SzText_AddFile(4, id, 0, (DotrTextLoadWord)bound);
            break;
        case 5:
            SzText_AddFile(7, id, 0, (DotrTextLoadWord)bound);
            break;
        case 6:
            SzText_AddFile(8, id, 0, (DotrTextLoadWord)bound);
            break;
        case 7:
        case 8:
        case 9:
            SzText_AddFile(9, id, 0, (DotrTextLoadWord)bound);
            break;
        }
        data = (int *)((char *)data + step);
        remain -= step;
        if (remain <= 0) {
            return;
        }
        ids++;
        index++;
    }
bind:
    RhText_BindAddress((void *)entry->unknown_000);
}

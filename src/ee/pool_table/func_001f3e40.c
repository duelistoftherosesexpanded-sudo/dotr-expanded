#include "dotr/ee/pool_table.h"

void lSzEffect_LoadSECtlCB(struct DotrPoolLoadRecord *record, int completed)
{
    struct DotrPoolEntryState *entry =
        (struct DotrPoolEntryState *)&gSzEffect_el_pool.unknown_000[record->unknown_1c];

    if (entry->state == 1) {
        entry->state = 3;
    }
    if (completed != 0) {
        entry->unknown_04 = record->unknown_14;
    }
}

#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

void lSzModel_LoadMotionCB(struct DotrResourceSubmitRecord *record, int completed)
{
    int modelId;
    int slot;
    struct DotrResourceSlots *model;

    modelId = record->unknown_01c >> 8;
    slot = record->unknown_01c & 0xff;
    if (modelId < 0 || modelId >= 84) {
        return;
    }
    model = gSzModel_ahModel_slots[modelId];
    if (model != 0 && completed != 0) {
        model->heads[slot] = (struct DotrSlotHead *)(DotrObjectWord)record->unknown_014;
        NjaReloc_Adjust_M3DMOT(model->heads[slot]);
        model->values[slot] = ((int *)(DotrObjectWord)model->heads[slot]->first)[1];
    }
}

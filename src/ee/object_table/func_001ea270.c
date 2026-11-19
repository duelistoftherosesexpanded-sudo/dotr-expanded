#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

void lSzModel_LoadEffectTextureFileCB(struct DotrResourceSubmitRecord *record, int completed)
{
    int *target = (int *)(DotrObjectWord)record->unknown_01c;

    if (completed != 0) {
        *target = record->unknown_014;
        RhText_BindAddress((void *)(DotrObjectWord)*target);
    }
}

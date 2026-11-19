#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

extern void lSzModel_LoadModelCB(struct DotrResourceSubmitRecord *record, int completed);

void lSzModel_LoadModelPackCB(struct DotrResourceSubmitRecord *record, int completed)
{
    char format[128];
    char name[128];
    int index;
    struct DotrObject *pModel;
    char *kind;
    index = record->unknown_01c;
    if (index < 0 || index >= 84) return;
    pModel = gSzModel_ahModel[index];
    if (pModel == 0) return;
    if (completed && *(int *)(DotrObjectWord)record->unknown_014 != 0x304852) completed = 0;
    if (completed == 0) {
        kind = gSzModel_apchBase[pModel->type];
        func_0019b648(format, data_002cfa1d, SzFile_GetEnv(data_002cfa17), kind, kind);
        func_0019b648(name, format, pModel->modelNo, pModel->modelNo);
        pModel->drawFlag.unknown_bit_0 = -1;
        /* MATCHING DEVICE: the request's result is kept in the `record` parameter; a separate
         * local leaves it in v0 and drops the copy to s2 the original has. */
        record = (struct DotrResourceSubmitRecord *)SzFile_ReadRequest(index, name, 0, -1, lSzModel_LoadModelCB, index);
        pModel->drawFlag.unknown_bit_3 = record ? 1 : 0;
    } else {
        lSzModel_Clean(index);
        lSzModel_BindModelByAddr(index, (struct DotrBindHeader *)(DotrObjectWord)record->unknown_014);
    }
}

#include "dotr/ee/model_callbacks.h"

void lSzModel_LoadModelCB(struct DotrResourceSubmitRecord *record, int completed)
{
    int index;
    struct DotrObject *pModel;

    index = record->unknown_01c;
    if (index < 0 || index >= 84) return;
    pModel = gSzModel_ahModel[index];
    if (pModel == 0) return;
    if (completed == 0) {
        pModel->drawFlag.unknown_bit_3 = 0;
        return;
    }
    lSzModel_Clean(index);
    pModel->modelBaseData = (struct DotrObjectLink *)(DotrObjectWord)record->unknown_014;
    *(char **)pModel->unknown_13c = (char *)pModel->modelBaseData + 16;
    ((int *)pModel->modelBaseData)[1] = 0;
    RhModel_BindAddress(pModel->modelBaseData);
    SzModel_LoadTextureFile(index);
    switch (pModel->type) {
    case 4:
    case 5:
    case 6: {
        char format[128];
        char name[128];
        int number = pModel->modelNo;
        char *kind = gSzModel_apchBase[pModel->type];
        int i;

        func_0019b648(format, data_002cfb94, SzFile_GetEnv(data_002cfa17), kind, kind);
        for (i = 0; i < 8; i++) {
            func_0019b648(name, format, number, number, i + 'a');
            lSzModel_LoadMotion(index, i, name);
        }
        SzModel_LoadEffectTextureFile(index);
        break;
    }
    case 0:
    case 3:
    case 7: {
        char format[128];
        char name[128];
        int number = pModel->modelNo;
        char *kind = gSzModel_apchBase[pModel->type];

        func_0019b648(format, data_002cfb94, SzFile_GetEnv(data_002cfa17), kind, kind);
        func_0019b648(name, format, number, number, 'a');
        lSzModel_LoadMotion(index, 0, name);
        break;
    }
    }
    SzFile_EntryGroup(index, (int)(DotrObjectWord)lSzModel_LoadCB);
}

#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

int SzModel_LoadEffectTextureFile(int modelId)
{
    char baseDirPath[128];
    char effectFilePath[128];
    struct DotrLoadObject *pModel;
    char *databaseDir;
    char *kind;
    int modelNumber;
    int i;

    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel_load[modelId];
    if (pModel == 0) {
        return 0;
    }
    databaseDir = SzFile_GetEnv(data_002cfa17);
    modelNumber = pModel->number;
    if (pModel->modelBaseData == 0) {
        return 1;
    }
    kind = gSzModel_apchBase[pModel->kind];
    pModel->main_part = 0;
    func_0019b648(baseDirPath, data_002cfb4f, databaseDir, kind);
    func_0019b648(effectFilePath, baseDirPath, modelNumber, modelNumber);
    SzFile_ReadRequest(modelId, effectFilePath, 0, -1, lSzModel_LoadEffectTextureFileCB, (int)(DotrObjectWord)&pModel->main_part);
    func_0019b648(baseDirPath, data_002cfb64, databaseDir, kind);
    for (i = 0; i < 8; i++) {
        pModel->parts[i] = 0;
        func_0019b648(effectFilePath, baseDirPath, modelNumber, modelNumber, i);
        SzFile_ReadRequest(modelId, effectFilePath, 0, -1, lSzModel_LoadEffectTextureFileCB, (int)(DotrObjectWord)&pModel->parts[i]);
    }
    return 1;
}

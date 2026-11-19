#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

int SzModel_LoadTextureFile(int modelId)
{
    char baseDirPath[128];
    char textureFilePath[128];
    char anmFilePath[128];
    struct DotrLoadObject *pModel;
    struct DotrObjPackView *view;
    struct DotrObjPackTable *table;
    struct DotrObjPackEntry *entry;
    char *base;
    int modelNumber;
    int textureCount;
    int i;
    char *kind;
    /* MATCHING DEVICE: the tag expression is written out at both calls; a named
     * local for it takes its saved register before `entry` (three registers rotate). */
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel_load[modelId];
    if (pModel == 0) {
        return 0;
    }
    base = SzFile_GetEnv(data_002cfa17);
    modelNumber = pModel->number;
    view = (struct DotrObjPackView *)pModel;
    if (view->base == 0) {
        return 1;
    }
    if (view->desc->unknown_2c == 0) {
        return 1;
    }
    kind = gSzModel_apchBase[pModel->kind];
    table = (struct DotrObjPackTable *)(view->base + view->desc->unknown_28);
    textureCount = table->count;
    entry = table->entries;
    func_0019b648(baseDirPath, data_002cfa30, base, kind);
    for (i = 0; i < textureCount; i++) {
        func_0019b648(textureFilePath, baseDirPath, modelNumber, &entry[i]);
        func_0019b9d0(textureFilePath, data_002cfa3a);
        func_0019b648(anmFilePath, baseDirPath, modelNumber, &entry[i]);
        func_0019b9d0(anmFilePath, data_002cfa3f);
        entry[i].a = 0;
        entry[i].c = 0;
        entry[i].d = 0;
        SzFile_ReadRequest(modelId, textureFilePath, 0, -1, lSzModel_LoadTextureFileCB, ((unsigned int)(DotrObjectWord)entry & 0x3ffffff) | ((i & 0x3f) << 26));
        SzFile_ReadRequest(modelId, anmFilePath, 0, -1, lSzModel_LoadTexAnimFileCB, ((unsigned int)(DotrObjectWord)entry & 0x3ffffff) | ((i & 0x3f) << 26));
    }
    return 1;
}

#include "dotr/ee/object_table.h"
#include "dotr/ee/resource_submit.h"

int lSzModel_LoadMotion(int modelId, int slot, char *filePath)
{
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    if (slot < 0 || slot >= 8) {
        return 0;
    }
    if (gSzModel_ahModel[modelId] == 0) {
        return 0;
    }
    lSzModel_FreeMotion(modelId, slot);
    SzFile_ReadRequest(modelId, filePath, 0, -1, lSzModel_LoadMotionCB, (modelId << 8) | slot);
    return 1;
}

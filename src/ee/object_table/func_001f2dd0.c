#include "dotr/ee/object_table.h"

int SzModel_SetRefID_RefNo(int modelId, int value, int slot)
{
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (slot < 0 || slot >= 8) {
        return 0;
    }
    if (value >= 0) {
        int high = (value & 0x7f00) >> 8;
        int current;
        if ((value & 0xff) >= pModel->refBoneNum) {
            return 0;
        }
        current = pModel->boneRefIds[slot];
        if ((current & 0x7fff) < 32767) {
            if (((current & 0x7f00) >> 8) != high) {
                return 0;
            }
        }
    }
    pModel->boneRefIds[slot] = value < 0 ? 32767 : value;
    return 1;
}

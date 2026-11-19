#include "dotr/ee/object_table.h"

/* MATCHING DEVICE: the declaration order of the locals. */
int SzModel_SetRefID(int modelId, int value, int flag)
{
    struct DotrObject *pModel;
    int high;
    int current;
    int i;
    int n;
    if (modelId < 0 || modelId >= 84) {
        return -1;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return -1;
    }
    if (flag) {
        if (value >= 0) {
            int kind = (value & 0x7f00) >> 8;
            for (i = 0; i < 8; i++) {
                if (((pModel->boneRefIds[i] & 0x7f00) >> 8) == kind) {
                    pModel->boneRefIds[i] = 32767;
                }
            }
        } else {
            for (i = 0; i < 8; i++) {
                pModel->boneRefIds[i] = 32767;
            }
        }
        pModel->boneRefSlotCursor = 0;
    }
    n = pModel->boneRefSlotCursor;
    if (n >= 8) {
        return -1;
    }
    if (value >= 0) {
        high = (value & 0x7f00) >> 8;
        if ((value & 0xff) >= pModel->refBoneNum) {
            return -1;
        }
        current = pModel->boneRefIds[(unsigned char)n];
        if ((current & 0x7fff) < 32767) {
            if (((current & 0x7f00) >> 8) != high) {
                return -1;
            }
        }
    }
    pModel->boneRefIds[(unsigned char)n] = value < 0 ? 32767 : value;
    return pModel->boneRefSlotCursor++;
}

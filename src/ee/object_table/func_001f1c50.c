#include "dotr/ee/object_table.h"

/* The original has no slot range check here. */
int SzModel_SetAnimFrameParam(int modelId, int animIndex, float requestedFrame)
{
    struct DotrObject *pModel;
    int currentAnim;
    float limit;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    currentAnim = pModel->unknown_1e0[animIndex];
    if (currentAnim < 0) {
        return 0;
    }
    if (pModel->animationDataPointers[currentAnim] == 0) {
        return 0;
    }
    limit = (float)pModel->animationFrameCountArray[currentAnim] - 1.0f;
    if (requestedFrame < 1.0f) {
        requestedFrame = 1.0f;
    }
    if (!(requestedFrame <= limit)) {
        requestedFrame = limit;
    }
    pModel->unknown_198[animIndex].y = requestedFrame;
    return 1;
}

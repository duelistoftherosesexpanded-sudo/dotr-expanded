#include "dotr/ee/object_table.h"

/* MATCHING DEVICE: the declaration order of the pointer locals. */
int SzModel_SetAnimIntrParam(int modelId, int animIndex, int unknown)
{
    struct DotrObject *pModel;
    struct DotrObjectVector3 *a, *b;
    signed char *current, *previous;
    float f, g;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    if (animIndex < 0 || animIndex >= 8) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    a = &pModel->unknown_198[0];
    b = &pModel->unknown_198[1];
    current = &pModel->unknown_1e0[0];
    previous = &pModel->unknown_1e0[1];
    if (pModel->animationDataPointers[animIndex] != 0) {
        f = pModel->animationFrameCountArray[animIndex];
        g = f - 1.0f;
        *b = *a;
        *previous = *current;
        *current = animIndex;
        if (pModel->animDir.unknown_bits_6_7 == -1) {
            a->y = g;
        } else {
            a->y = 1.0f;
        }
        a->z = 0.0f;
        b->z = 1.0f - a->z;
        pModel->blendTickTotal = unknown;
        pModel->motionDelay = 0;
    } else {
        return 0;
    }
    return 1;
}

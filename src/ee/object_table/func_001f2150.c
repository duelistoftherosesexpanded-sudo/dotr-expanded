#include "dotr/ee/object_table.h"

/* Same body as SzModel_IsModelStandby. */
static inline int active(int index)
{
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    return p->modelId >= 0 ? 1 : 0;
}

/* The body of lSzModel_GetGNode with unknown = 1 and no flag pointer. */
static inline DotrObjectWord find(int index)
{
    int local = 0;
    struct DotrObject *p;
    if (index < 0 || index >= 84) {
        return 0;
    }
    p = gSzModel_ahModel[index];
    if (p == 0) {
        return 0;
    }
    return lSzModel_TraceGNode(1, p->GNode, p->GNode, &local, 0, 0, 0);
}

int SzModel_GetCenterOfs(int modelId, struct DotrObjectVector4 *centerArray)
{
    struct DotrObject *pModel;
    float *gnode;
    float scale;

    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (!active(modelId)) {
        return 0;
    }
    if (pModel->refBoneNum <= 0) {
        return 0;
    }
    gnode = (float *)find(modelId);
    if (gnode != 0) {
        scale = (*(float **)pModel->unknown_13c)[21];
        /* gnode[8..10]: the node's bone origin x, y, z. */
        centerArray->x = gnode[8] / scale;
        centerArray->y = gnode[9] / scale;
        centerArray->z = gnode[10] / scale;
        centerArray->unknown_w = 1.0f;
    }
    return 1;
}

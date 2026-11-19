#include "dotr/ee/object_table.h"

int lSzModel_GetGNode(int modelId, int gNodeId, int *maxDepth)
{
    int traversalCounter = 0;
    struct DotrObject *pModel;
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (maxDepth != 0) {
        *maxDepth = 1;
    }
    return lSzModel_TraceGNode(gNodeId, pModel->GNode, pModel->GNode, &traversalCounter, 0, maxDepth, 0);
}

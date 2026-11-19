#include "dotr/ee/model_merge.h"

void lSzModel_LoadMergeCB(struct DotrMergeRecord *req, int ok)
{
    int user = req->user;
    struct DotrMergeTable first = data_00292ca0_merge;
    struct DotrMergeTable count = data_00292d40_merge;
    struct DotrMergeTable limit = data_00292de0_merge;
    struct DotrMergeOffsets offs = data_00292e80_merge;
    int j;
    struct DotrMergeObject *pModel;

    if (!ok) {
        for (j = 0; j < count.v[user]; j++) {
            pModel = gSzModel_ahModel_merge[first.v[user] + j];
            if (pModel) {
                pModel->bits.bit_3 = 0;
            }
        }
        return;
    }
    for (j = 0; j < count.v[user]; j++) {
        pModel = gSzModel_ahModel_merge[first.v[user] + j];
        if (pModel) {
            pModel->bits.bit_3 = 0;
            if (pModel->number < gSzModel_awIgnore_merge[pModel->kind] && j < limit.v[user] && *(int *)(req->data + offs.v[user][j]) == 0x304852) {
                pModel->bits.bit_0 = 0;
                lSzModel_Clean(first.v[user] + j);
                lSzModel_BindModelByAddr(first.v[user] + j, req->data + offs.v[user][j]);
            }
        }
    }
}

#include "dotr/ee/object_table.h"

void lSzModel_LoadCameraCB(struct DotrObjectAttachMessage *p, int on)
{
    int index = p->unknown_1c & 0x7f;
    int high = p->unknown_1c & 0x80;
    struct DotrObjectAttachView *pModel;
    int last;

    if (index < 0 || index >= 84)
        return;
    pModel = (struct DotrObjectAttachView *)gSzModel_ahModel[index];
    if (pModel == 0)
        return;
    if (pModel->modelBaseData == 0) {
        SzMem_Free((void *)p->unknown_14);
        on = 0;
    }
    if (on == 0) {
        if (high)
            pModel->pCameraMotion2 = 0;
        else
            pModel->pCameraMotion = 0;
        return;
    }
    if (high) {
        pModel->pCameraMotion2 = p->unknown_14;
        pModel->modelBaseData->unknown_04 &= ~0x100000;
    } else {
        pModel->pCameraMotion = p->unknown_14;
        last = ((unsigned short *)pModel->pCameraMotion)[6] - 1;
        pModel->cameraFrameTotal = last;
        pModel->cameraFrameCurrent = last;
        pModel->modelBaseData->unknown_04 &= ~0x80000;
    }
}

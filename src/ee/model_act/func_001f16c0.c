#include "dotr/ee/model_act.h"

/* 0x1f16c0. The model table is the view gSzModel_ahModel_modelact. */
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */
#ifndef __MWERKS__
/* Host build only: the unused local `scaled` is kept. */
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

int SzModel_SetAnimIntrReverseParam(int modelId, int arg1)
{
    signed char oldLevel;
    int scaled;
    unsigned char * bytes;
    struct DotrModelActVector3 temp;
    struct DotrModelActObject * pModel;
    struct DotrModelActVector3 * first;
    struct DotrModelActVector3 * second;
    signed char * firstLevel;
    signed char * secondLevel;
    if ((modelId < 0) || (modelId >= 0x54)) {
        return 0;
    }
    pModel = gSzModel_ahModel_modelact[modelId];
    if (pModel == NULL) {
        return 0;
    }
    bytes = (unsigned char *)pModel;
    first = (struct DotrModelActVector3 *)&pModel->slots[8];
    second = (struct DotrModelActVector3 *)&pModel->slots[11];
    firstLevel = &pModel->lv[0];
    secondLevel = &pModel->lv[1];
    if (*firstLevel >= 0 && (oldLevel = *secondLevel) >= 0) {
        temp = *second;
        *second = *first;
        *first = temp;
        *secondLevel = *firstLevel;
        *firstLevel = oldLevel;
        bytes[0x1FC] = (float)(bytes[0x1FC] * arg1) / (float)(unsigned int)bytes[0x1FB];
        bytes[0x1FB] = arg1;
        bytes[0x1FC] = bytes[0x1FB] - bytes[0x1FC];
        bytes[0x1FD] = 0;
    } else {
        return 0;
    }
    return 1;
}

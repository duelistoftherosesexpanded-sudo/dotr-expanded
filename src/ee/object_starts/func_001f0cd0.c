#include "dotr/ee/object_starts.h"

/* Looks up the object for side's packed cell; 0 when its index is outside 0-83 or the table
 * entry is empty. Otherwise sets it up through the callees, stores SzEffect_EntryCallback's result at
 * +0x1ec and, when flag is set and +0x16c is non-zero, sends kind 8 or 9 (alt) through
 * SzView_ReqCamera; returns 1. */
int SzModel_BattleSeq(int side, int flag, int alt)
{
    struct DotrStartObject *pModel;
    int own, other, index, i;

    own = SzBattle_InqEntry(side);
    other = SzBattle_InqEntry(side ^ 1);
    index = SzDuel_GetUnitModelID(own >> 8, own & 0xff);
    if (index < 0 || index >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel_start[index];
    if (pModel == 0) {
        return 0;
    }
    SzModel_SetAnimIntr(index, 2);
    SzText_FreeRequest(5, -1);
    SzEffect_ClearRefID2(own, 1);
    SzEffect_ClearRefID2(other, 1);
    for (i = 0; i < 8; i++) {
        SzText_AddFile(5, i, 0, pModel->effectTextureB[i]);
    }
    pModel->effectCallback = SzEffect_EntryCallback(1, pModel->cameraAxisStored, pModel->unknown_208, other, own, lSzModel_EffectCB, index);
    if (flag != 0) {
        if (pModel->pCameraMotion != 0) {
            func_001fa8a0_start(0, 1, (alt ? 1 : 0) + 8, pModel->cameraFrameCurrent, index, pModel->pCameraMotion);
        }
    }
    return 1;
}

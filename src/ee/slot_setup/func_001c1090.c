#include "dotr/ee/slot_setup.h"

void SzDuel_Init3DView(int value)
{
    unsigned char *view = data_00317490_bytes;

    if (value >= 0) {
        int id = SzDuel_New2Xax(value);
        SzModel_Load(29, -1, id, 1);
        SzSound_TransferSEGrp(2, id, 0);
    } else {
        union DotrSlotSetupQuad zero = data_00291370;
        union DotrSlotSetupElement element = data_00291380;
        union DotrSlotSetupQuad zero2 = data_00291410;
        struct DotrSlotSetupTriple scale = data_00291420;
        int i;

        for (i = 0; i < 84; i++) {
            SzModel_SetDrawFlag(i, 0);
        }
        for (i = 0; i < 40; i++) {
            SzModel_SetDrawFlag(i + 34, 1);
        }
        SzModel_SetDrawFlag(29, 1);
        SzModel_SetScale(29, 0);
        SzModel_SetRot(29, &zero);
        SzModel_SetRotC(29, &zero);
        SzModel_SetPos(29, &zero2);
        SzModel_SetDeform(29, 0);
        SzModel_SetDeformRatio(29, 0);
        SzModel_SetAnimDirect(29, 0);
        SzModel_SetAnimDir(29, 1);
        SzModel_SetAnimSpeed(29, 1.0f);
        SzModel_SetRgba(29, 0, 0);
        SzModel_SetFilter(29, 1, 0);
        SzModel_SetIntensity(29, &scale);
        SzView_SetStraightLight(0, 4, &element);
        SzView_SetCP(0, &zero2);
        view[0xd35] = 0;
    }
}

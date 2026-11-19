#include "dotr/ee/view_act_camera.h"

/* 0x1fcab0 SzView_ActCamera (name from the Japanese build's symbols).
 * MATCHING DEVICE: `hint` is always zero; with the `hint != 0 ? 0 : gSzView_pCamera` selections
 * both SzView_ReqCamera calls load gSzView_pCamera before gSzView_wFrame as the original does; without them the two load the other way round. */
void SzView_ActCamera(void)
{
    int value;
    int handle;
    int hint = 0;

    if (gSzView_pCamera != 0) {
        if (SzPad_Check(0, 2, 4) != 0) {
            value = gSzView_wFrame - 1;
            gSzView_wFrame = (value > 0) ? value : 0;
        }
        if (SzPad_Check(0, 2, 8) != 0) {
            value = gSzView_wFrame + 1;
            gSzView_wFrame = (value < 1000) ? value : 1000;
        }
        if (SzPad_Check(0, 0, 0x10) != 0) {
            SzView_ReqCamera(0, 0, 8, gSzView_wFrame, 7, hint != 0 ? 0 : gSzView_pCamera);
        }
        if (SzPad_Check(0, 0, 0xA0) != 0) {
            value = SzBattle_InqEntry(0);
            handle = SzDuel_GetUnitModelID(value >> 8, value & 0xFF);
            SzModel_SetAnimIntr(handle, SzPad_Check(0, 0, 0x80) ? 5 : 2);
            SzView_ReqCamera(0, 0, 8, gSzView_wFrame, handle, hint != 0 ? 0 : gSzView_pCamera);
        }
    }
}

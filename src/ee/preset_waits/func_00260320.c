#include "dotr/ee/preset_waits.h"

int Ta_Eff_WAIT(struct DotrWaitOwner *owner, unsigned short *queue, int mode)
{
    if (owner->unknown_08 != 0xffff) {
        SzDuel_SetWindowDraw(0);
    }
    if (mode >= 0) {
        InitEffectTemp((struct DotrPresetInit *)queue);
        if (owner->unknown_08 != 0xffff) {
            TaView_ReqFocusCamera(0, owner->unknown_08, 300, 300);
            SzDuel_OpenMsgPopup(0, -120, 80, 270);
        }
        queue[0]++;
    } else {
        switch (queue[2]) {
        case 0:
            if (queue[0]++ <= owner->unknown_0c) {
                return 1;
            }
            SzDuel_CloseMsgPopup();
            if (owner->unknown_08 != 0xffff) {
                TaView_RestoreOriginalView((struct DotrPresetPair *)queue, 16);
            }
            queue[2]++;
            break;
        default:
            if (owner->unknown_08 != 0xffff) {
                if (queue[129]++ > 16) {
                    if (SzDuel_GetWindowDisp() != 0) {
                        SzDuel_SetWindowDraw(1);
                    }
                    SzDuel_CloseMsgPopup();
                    return 0;
                }
            } else {
                SzDuel_CloseMsgPopup();
                return 0;
            }
            break;
        }
    }
    return 1;
}

#include "dotr/ee/preset_waits.h"

int Ta_Eff_023(void *owner, struct DotrWaitQueue *queue, int mode)
{
    int job[4];
    int *p;

    job[0] = 999;
    job[1] = 0;
    job[2] = 0;
    job[3] = 0;
    p = job;
    SzDuel_SetWindowDraw(0);
    if (mode >= 0) {
        InitEffectTemp((struct DotrPresetInit *)queue);
        queue->unknown_00 = 128;
        TaView_ReqFlatCompleteView(16);
        Ta_AllUnitFade(owner, (unsigned char *)queue, mode, -1, 1, 1);
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 523);
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 603);
        return 1;
    } else {
        switch (queue->unknown_04) {
        case 0:
            queue->unknown_102++;
            if (queue->unknown_00 > 63) {
                queue->unknown_00 -= 15;
            } else {
                queue->unknown_00 = 48;
                if (queue->unknown_102 >= 16 && TaEffect_IsLoadModule((struct DotrPresetObject *)queue) == 1) {
                    Ta_SetEffectDependOnFlagLst(owner, queue, p, 523);
                    queue->unknown_04++;
                }
            }
            SzEffect_SetBlackOutForDuel((unsigned char)queue->unknown_00);
            break;
        case 1:
            if (Ta_ManageDestroyEffect(owner, queue, p) != 0) {
                queue->unknown_04++;
            }
            break;
        case 2:
            if (Ta_IsFinishDestEfc(owner, queue, p) != 0) {
                Ta_AllUnitFade(owner, (unsigned char *)queue, mode, 1, 0, 1);
                TaView_RestoreOriginalView((struct DotrPresetPair *)queue, 16);
                queue->unknown_102 = 0;
                queue->unknown_04++;
            }
            break;
        default:
            queue->unknown_00 += 15;
            if (queue->unknown_00 > 128) {
                queue->unknown_00 = 128;
            }
            SzEffect_SetBlackOutForDuel((unsigned char)queue->unknown_00);
            if (queue->unknown_00 == 128) {
                if (queue->unknown_102++ > 16) {
                    if (Ta_IsFinishFade() == 1) {
                        if (SzDuel_GetWindowDisp() != 0) {
                            SzDuel_SetWindowDraw(1);
                        }
                        SzDuel_CloseMsgPopup();
                        return 0;
                    }
                }
            }
            break;
        }
    }
    return 1;
}

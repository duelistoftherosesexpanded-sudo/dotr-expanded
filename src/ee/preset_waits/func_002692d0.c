#include "dotr/ee/preset_waits.h"
int Ta_Eff_060(void *owner, struct DotrWaitQueue *queue, int mode)
{
    int job[4];
    int *p;
    int pos;
    struct DotrWaitFlagQueue *q = (struct DotrWaitFlagQueue *)queue;
    job[0] = 999;
    job[1] = 0;
    job[2] = 0;
    job[3] = 0;
    p = job;
    SzDuel_SetWindowDraw(0);
    if (mode >= 0) {
        InitEffectTemp((struct DotrPresetInit *)queue);
        queue->unknown_00 = 128;
        queue->unknown_02 = Ta_GetMinimumOrderUnitFromFlagList(((struct DotrWaitOwner *)owner)->unknown_08);
        Ta_AllUnitFade(owner, (unsigned char *)queue, mode, -1, 1, 1);
        TaView_ReqFocusCamera(30, queue->unknown_02, 100, 180);
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 560);
        return 1;
    } else {
        switch (queue->unknown_04) {
        case 0:
            queue->unknown_102++;
            if (queue->unknown_00 > 48) {
                queue->unknown_00 -= 15;
            } else {
                queue->unknown_00 = 48;
                if (TaEffect_IsLoadModule((struct DotrPresetObject *)queue) == 1 && queue->unknown_102 >= 30) {
                    p[0] = 560;
                    p[1] = 0;
                    p[2] = queue->unknown_02;
                    p[3] = 0;
                    queue->unknown_06[0] = Ta_Do_Effect(p, queue);
                    q->unknown_20[0] = 0;
                    queue->unknown_04++;
                }
            }
            if (queue->unknown_00 < 48) {
                queue->unknown_00 = 48;
            }
            SzEffect_SetBlackOutForDuel((unsigned char)queue->unknown_00);
            break;
        case 1:
            if (SzEffect_IsUse(queue->unknown_06[0]) == 1) {
                if (SzEffect_GetStatus(queue->unknown_06[0]) == 2 && q->unknown_20[0] == 0) {
                    pos = SzDuel_GetUnitLoc(queue->unknown_02 >> 8, queue->unknown_02 & 0xff);
                    Ta_DelUnit(queue->unknown_02 >> 8, queue->unknown_02 & 0xff);
                    SzDuel_AddUnit(Ta_IsUnitControl(queue->unknown_02 >> 8, queue->unknown_02 & 0xff), pos & 0xff, pos >> 8,
                                  ((struct DotrWaitOwner *)owner)->unknown_0c, 0);
                    SzEffect_SetBusyFlag2(queue->unknown_02, 0);
                    q->unknown_20[0] = 1;
                }
            } else {
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

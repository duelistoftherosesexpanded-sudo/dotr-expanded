#include "dotr/ee/preset_waits.h"
int Ta_Eff_019(void *owner, struct DotrWaitQueue *queue, int mode)
{
    int job[4];
    int *p;
    unsigned char id;
    int stored;
    int i;
    struct DotrWaitFlagQueue *q = (struct DotrWaitFlagQueue *)queue;
    job[0] = 999;
    job[1] = 0;
    job[2] = 0;
    job[3] = 0;
    p = job;
    SzDuel_SetWindowDraw(0);
    if (mode >= 0) {
        InitEffectTemp((struct DotrPresetInit *)queue);
        Ta_AllUnitFade(owner, (unsigned char *)queue, mode, -1, 1, 1);
        TaView_ReqStdCompleteView(16);
        queue->unknown_00 = 128;
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 56);
        return 1;
    } else {
        if (queue->unknown_04 == 0) {
            if (queue->unknown_00 > 63) {
                queue->unknown_00 -= 15;
            } else {
                queue->unknown_00 = 48;
                if (TaEffect_IsLoadModule((struct DotrPresetObject *)queue) == 1) {
                    p[0] = 56;
                    p[1] = 0;
                    p[2] = ((struct DotrWaitFlagOwner *)owner)->unknown_08;
                    p[3] = 0;
                    q->unknown_20[q->unknown_d4] = 1;
                    id = Ta_Do_Effect(p, queue);
                    queue->unknown_06[queue->unknown_d4] = id;
                    /* MATCHING DEVICE: always-true test of the stored byte, as in Ta_Eff_018. */
                    stored = id;
                    if (stored >= 0) {
                        queue->unknown_d4++;
                    }
                    queue->unknown_04++;
                }
            }
            SzEffect_SetBlackOutForDuel((unsigned char)queue->unknown_00);
        } else if (queue->unknown_04 == 1) {
            if (SzEffect_IsUse(queue->unknown_06[0]) == 1) {
                if (SzEffect_GetStatus(queue->unknown_06[0]) == 2) {
                    if (q->unknown_20[0] == 1) {
                        for (i = 0; i < 22; i++) {
                            if (((((struct DotrWaitFlagOwner *)owner)->unknown_08 >> (21 - i)) & 1) == 1) {
                                if (i < 11) {
                                    Ta_DelUnit(0, i);
                                } else {
                                    Ta_DelUnit(1, i - 11);
                                }
                            }
                        }
                        q->unknown_20[0] = 0;
                    }
                }
                return 1;
            }
            Ta_AllUnitFade(owner, (unsigned char *)queue, mode, 1, 0, 1);
            TaView_RestoreOriginalView((struct DotrPresetPair *)queue, 16);
            queue->unknown_102 = 0;
            queue->unknown_04++;
        } else {
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
        }
    }
    return 1;
}

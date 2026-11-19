#include "dotr/ee/preset_waits.h"

static inline void job_set(int *job, int id)
{
    job[0] = id;
    job[1] = 0;
    job[2] = 0;
    job[3] = 0;
}

int Ta_Eff_016(void *owner, struct DotrWaitQueue *queue, int mode)
{
    int job[4];
    int *p;
    unsigned char id;
    int stored;

    job[0] = 999;
    job[1] = 0;
    job[2] = 0;
    job[3] = 0;
    p = job;
    SzDuel_SetWindowDraw(0);
    if (mode >= 0) {
        InitEffectTemp((struct DotrPresetInit *)queue);
        TaView_ReqStdCompleteView(16);
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 516);
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 31);
        TaEffect_LoadModule((struct DotrPresetObject *)queue, 603);
        Ta_AllUnitFade(owner, (unsigned char *)queue, mode, -1, 1, 1);
        queue->unknown_00 = 128;
        return 1;
    } else {
        if (queue->unknown_04 == 0) {
            if (queue->unknown_00 > 63) {
                queue->unknown_00 -= 15;
            } else {
                queue->unknown_00 = 48;
                if (TaEffect_IsLoadModule((struct DotrPresetObject *)queue) == 1 && queue->unknown_102++ > 16) {
                    job_set(p, 516);
                    id = Ta_Do_Effect(p, queue);
                    queue->unknown_06[queue->unknown_d4] = id;
                    /* MATCHING DEVICE: the original tests the stored byte, zero-extended,
                     * for >= 0 (`andi; bltz`), which is always true. */
                    stored = id;
                    if (stored >= 0) {
                        queue->unknown_d4++;
                    }
                    queue->unknown_04++;
                    queue->unknown_102 = 0;
                }
            }
            SzEffect_SetBlackOutForDuel((unsigned char)queue->unknown_00);
        } else if (queue->unknown_04 == 1) {
            if (SzEffect_IsUse(queue->unknown_06[0]) == 1) {
                return 1;
            }
            Ta_SetEffectDependOnFlagLst(owner, queue, p, 31);
            queue->unknown_04++;
        } else if (queue->unknown_04 == 2) {
            if (Ta_ManageDestroyEffect(owner, queue, p) != 0) {
                queue->unknown_04++;
            }
        } else if (queue->unknown_04 == 3) {
            if (Ta_IsFinishDestEfc(owner, queue, p) != 0) {
                Ta_AllUnitFade(owner, (unsigned char *)queue, mode, 1, 0, 1);
                TaView_RestoreOriginalView((struct DotrPresetPair *)queue, 16);
                queue->unknown_102 = 0;
                queue->unknown_04++;
            }
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

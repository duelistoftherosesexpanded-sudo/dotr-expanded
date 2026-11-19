#include "dotr/ee/object_table.h"

/* For channel 2 or 3 and a usable object (index 0..83, non-null, +0x1f2 not
 * negative, +0x138 set, bit 3 of +0x20c clear, bit 1 set, bit 0 of +0x20d
 * clear, and either +0x1ec not negative or the two-bit field of +0x1f7 equal
 * to 1) with a cue list at +0x174: passes the code of every cue of the active
 * kind whose time lies in [now - step, now) to SzSound_PlaySE with the
 * channel, stopping at kind 15 or after 64 cues, and returns 1. now and the
 * kind are +0x19c and +0x1e0, or SzEffect_GetCount(+0x1ec) and 2 when +0x1ec is
 * not negative; step is SzGame_GetFrameSpeed. Returns 0 otherwise. */
int SzModel_ReqSE(int channel, int modelId)
{
    struct DotrObject *pModel;
    int kind;
    struct DotrObjectCue *cue;
    int i;
    float previous;
    float now;

    if (channel != 2 && channel != 3) {
        return 0;
    }
    if (modelId < 0 || modelId >= 84) {
        return 0;
    }
    pModel = gSzModel_ahModel[modelId];
    if (pModel == 0) {
        return 0;
    }
    if (pModel->modelId < 0) {
        return 0;
    }
    if (pModel->modelBaseData == 0) {
        return 0;
    }
    if (pModel->drawFlag.unknown_bit_3) {
        return 0;
    }
    if (!pModel->drawFlag.unknown_bit_1) {
        return 0;
    }
    if (pModel->stateFlags.unknown_bit_0) {
        return 0;
    }
    if (*(int *)&pModel->unknown_1e8[4] < 0 && pModel->animDir.unknown_bits_6_7 != 1) {
        return 0;
    }
    cue = *(struct DotrObjectCue **)pModel->ptrBlock;
    if (cue != 0) {
        kind = pModel->unknown_1e0[0];
        previous = func_001a8760_int();
        now = pModel->unknown_198[0].y;
        if (*(int *)&pModel->unknown_1e8[4] >= 0) {
            kind = 2;
            now = SzEffect_GetCount(*(int *)&pModel->unknown_1e8[4]);
        }
        previous = now - previous;
        for (i = 0; i < 64; i++, cue++) {
            if (cue->kind == 15) {
                break;
            }
            if (cue->kind == kind) {
                if (cue->time >= previous && cue->time < now) {
                    func_00217710_cue(channel, cue->code);
                }
            }
        }
        return 1;
    }
    return 0;
}

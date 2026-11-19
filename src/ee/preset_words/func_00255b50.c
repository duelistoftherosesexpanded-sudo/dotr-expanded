#include "dotr/ee/preset_words.h"

int TaEffect_IsLoadModule(struct DotrPresetObject *effectModule)
{
    int i;
    struct DotrJobPool *e;
    int state;
    for (i = 0; i < effectModule->unknown_80.unknown_bits_1_5; i++) {
        if (effectModule->unknown_82[i] == 0xffff) {
            continue;
        }
        e = SzEffect_GetModuleInfo(0, effectModule->unknown_82[i]);
        if (e) {
            state = e->state;
            if (state < 2) {
                if (state == 0) {
                    effectModule->unknown_b2[i] = 0;
                } else {
                    return -1;
                }
            } else {
                effectModule->unknown_b2[i] = state;
            }
        }
    }
    effectModule->unknown_80.unknown_bit_0 = -1;
    return 1;
}

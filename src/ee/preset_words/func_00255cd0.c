#include "dotr/ee/preset_words.h"

void TaEffect_ClearEffectTemp(struct DotrPresetObject *visualEffect)
{
    int i;
    visualEffect->unknown_80.unknown_bit_0 = 0;
    visualEffect->unknown_80.unknown_bits_1_5 = 0;
    for (i = 0; i < 24; i++) {
        visualEffect->unknown_82[i] = 0xffff;
        visualEffect->unknown_b2[i] = 0;
    }
}

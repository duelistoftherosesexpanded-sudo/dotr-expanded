#include "dotr/ee/preset_words.h"

void InitEffectTemp(struct DotrPresetInit *effectAnimationParams)
{
    int i;
    float *src_a;
    float *src_b;
    effectAnimationParams->word_00 = 0;
    effectAnimationParams->word_02 = 0;
    effectAnimationParams->word_04 = 0;
    effectAnimationParams->byte_d4 = 0;
    TaEffect_ClearEffectTemp((struct DotrPresetObject *)effectAnimationParams);
    for (i = 0; i < 24; i++) {
        effectAnimationParams->bytes_06[i] = 255;
        effectAnimationParams->words_20[i] = 0;
    }
    src_a = func_001f9d30_floats(0);
    src_b = func_001faf60_floats(0);
    effectAnimationParams->word_d0 = 0;
    effectAnimationParams->word_cc = 0;
    effectAnimationParams->byte_100 = 0;
    effectAnimationParams->word_102 = 0;
    effectAnimationParams->vec_e0[0] = src_a[0];
    effectAnimationParams->vec_e0[1] = src_a[1];
    effectAnimationParams->vec_e0[2] = src_a[2];
    effectAnimationParams->vec_e0[3] = src_a[3];
    effectAnimationParams->vec_f0[0] = src_b[0];
    effectAnimationParams->vec_f0[1] = src_b[1];
    effectAnimationParams->vec_f0[2] = src_b[2];
    effectAnimationParams->vec_f0[3] = src_b[3];
}

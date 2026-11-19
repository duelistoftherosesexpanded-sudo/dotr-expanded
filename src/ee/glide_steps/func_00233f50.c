#include "dotr/ee/glide_steps.h"

/* The stores address the block as a literal, like the other users of 0x1ed0000. */
void Taku_Camp_SINEMA_Init(signed char presetNum)
{
    DOTR_GLIDE_HEAD->unknown_1c = 0;
    switch (presetNum) {
    case 0:
        DOTR_GLIDE_HEAD->unknown_1a = 0;
        DOTR_GLIDE_HEAD->unknown_1e = 0;
        DOTR_GLIDE_HEAD->unknown_20 = 0;
        DOTR_GLIDE_HEAD->unknown_22 = 0;
        break;
    case 1:
        DOTR_GLIDE_HEAD->unknown_1a = 1;
        DOTR_GLIDE_HEAD->unknown_1e = 64;
        DOTR_GLIDE_HEAD->unknown_20 = 0;
        DOTR_GLIDE_HEAD->unknown_22 = 0;
        break;
    case 2:
        DOTR_GLIDE_HEAD->unknown_1a = 2;
        DOTR_GLIDE_HEAD->unknown_1e = 64;
        DOTR_GLIDE_HEAD->unknown_20 = 64;
        DOTR_GLIDE_HEAD->unknown_22 = 0;
        break;
    case 3:
        DOTR_GLIDE_HEAD->unknown_1a = 3;
        DOTR_GLIDE_HEAD->unknown_1e = 64;
        DOTR_GLIDE_HEAD->unknown_20 = 64;
        DOTR_GLIDE_HEAD->unknown_22 = 116;
        break;
    }
}

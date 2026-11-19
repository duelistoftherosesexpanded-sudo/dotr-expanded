#include "dotr/ee/obj_vectors.h"
int CARD_ON(int card, int step)
{
    unsigned char rgba[4];
    if (SzEffect_IsCardStandby2(card) != 0) {
        SzEffect_GetCardRgba2(card, rgba);
        rgba[3] += step;
        if (rgba[3] >= 128) {
            rgba[3] = 128;
        }
        SzEffect_SetCardRgba2(card, rgba);
        if (rgba[3] >= 128) {
            return 1;
        }
    }
    return 0;
}

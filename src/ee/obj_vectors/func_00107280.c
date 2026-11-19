#include "dotr/ee/obj_vectors.h"

int CARD_OFF(int card)
{
    unsigned char rgba[4];

    if (SzEffect_IsCardStandby2(card) != 0) {
        SzEffect_GetCardRgba2(card, rgba);
        if (rgba[3] > 20) {
            rgba[3] -= 19;
        } else {
            rgba[3] = 0;
        }
        SzEffect_SetCardRgba2(card, rgba);
        if (rgba[3] == 0) {
            return 1;
        }
    }
    return 0;
}

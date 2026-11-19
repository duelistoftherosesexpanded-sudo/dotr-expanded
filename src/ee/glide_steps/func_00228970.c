#include "dotr/ee/glide_steps.h"

int Taku_Tyumoku_ido(void)
{
    if (DOTR_GLIDE_STATE->step == 0) {
        DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
        DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
    }
    DOTR_GLIDE_STATE->step += 16;
    if (DOTR_GLIDE_STATE->step >= 256) {
        DOTR_GLIDE_STATE->step = 256;
    }
    DOTR_GLIDE_STATE->x = DOTR_GLIDE_STATE->from_x * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
        + DOTR_GLIDE_TARGET->x * (float)DOTR_GLIDE_STATE->step / 256.0f;
    DOTR_GLIDE_STATE->y = DOTR_GLIDE_STATE->from_y * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
        + DOTR_GLIDE_TARGET->y * (float)DOTR_GLIDE_STATE->step / 256.0f;
    if (DOTR_GLIDE_STATE->step >= 256) {
        DOTR_GLIDE_STATE->step = 0;
        DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
        DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
        DOTR_GLIDE_FLAGS->first = 0;
        DOTR_GLIDE_FLAGS->second = 0;
        return 1;
    }
    return 0;
}

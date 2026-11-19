#include "dotr/ee/glide_steps.h"

int Taku_Tyumoku_ido1(short side, short row)
{
    short list;

    list = 0;
    switch (DOTR_GLIDE_FLAGS->first) {
    case 0:
        if (*DOTR_GLIDE_LIMIT == 0) {
            Taku_Dodai_Root_init(side, row, 3, &list, 0);
            *DOTR_GLIDE_LIMIT += 1;
        }
        break;
    case 1:
        if (DOTR_GLIDE_STATE->step == 0) {
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
        }
        DOTR_GLIDE_STATE->x = DOTR_GLIDE_STATE->from_x * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
            + (float)DOTR_GLIDE_STATE->step * data_00298ba0_glide[row].x / 256.0f;
        DOTR_GLIDE_STATE->y = DOTR_GLIDE_STATE->from_y * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
            + (float)DOTR_GLIDE_STATE->step * data_00298ba0_glide[row].y / 256.0f;
        DOTR_GLIDE_STATE->step += 16;
        if (DOTR_GLIDE_STATE->step > 256) {
            DOTR_GLIDE_STATE->step = 0;
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
            DOTR_GLIDE_FLAGS->first = 0;
            DOTR_GLIDE_FLAGS->second = 0;
            return 1;
        }
        break;
    case 2:
        break;
    }
    return 0;
}

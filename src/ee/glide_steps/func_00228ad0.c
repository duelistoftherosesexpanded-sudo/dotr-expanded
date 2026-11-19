#include "dotr/ee/glide_steps.h"

int Taku_Tyumoku_ido0(short side, short row, short *list, short count)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;
    short i;
    short k;
    short n;
    short skip;
    float sx;
    float sy;

    switch (DOTR_GLIDE_FLAGS->first) {
    case 1:
        if (DOTR_GLIDE_STATE->step == 0) {
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
        }
        DOTR_GLIDE_STATE->step += 16;
        if (DOTR_GLIDE_STATE->step >= 256) {
            DOTR_GLIDE_STATE->step = 256;
        }
        DOTR_GLIDE_STATE->x = DOTR_GLIDE_STATE->from_x * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
            + (float)DOTR_GLIDE_STATE->step * data_00298ba0_glide[row].x / 256.0f;
        DOTR_GLIDE_STATE->y = DOTR_GLIDE_STATE->from_y * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
            + (float)DOTR_GLIDE_STATE->step * data_00298ba0_glide[row].y / 256.0f;
        if (DOTR_GLIDE_STATE->step >= 256) {
            DOTR_GLIDE_STATE->step = 0;
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
            DOTR_GLIDE_FLAGS->first += 1;
        }
        break;
    case 2:
        if (DOTR_GLIDE_STATE->step == 0) {
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
            sx = 0.0f;
            sy = 0.0f;
            skip = 0;
            for (i = 0; i < count; i++) {
                n = list[i];
                if (side == 0 && data_00298ba0_glide[row].marks[i] == 2) {
                    k = data_00299020_glide[side][n].other_cell;
                } else {
                    k = data_00299020_glide[side][n].cell;
                }
                if (k != 7) {
                    sx += data_00298ba0_glide[k].x;
                    sy += data_00298ba0_glide[k].y;
                } else {
                    skip = 1;
                }
            }
            if (skip) {
                count--;
            }
            DOTR_GLIDE_TARGET->unknown_10[0] = sx / (float)count;
            DOTR_GLIDE_TARGET->unknown_10[2] = sy / (float)count;
        }
        DOTR_GLIDE_STATE->x = DOTR_GLIDE_STATE->from_x * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
            + DOTR_GLIDE_TARGET->unknown_10[0] * (float)DOTR_GLIDE_STATE->step / 256.0f;
        DOTR_GLIDE_STATE->y = DOTR_GLIDE_STATE->from_y * (float)(256 - DOTR_GLIDE_STATE->step) / 256.0f
            + DOTR_GLIDE_TARGET->unknown_10[2] * (float)DOTR_GLIDE_STATE->step / 256.0f;
        DOTR_GLIDE_STATE->step += 8;
        if (DOTR_GLIDE_STATE->step > 256) {
            DOTR_GLIDE_STATE->step = 0;
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
            DOTR_GLIDE_FLAGS->first = 0;
            DOTR_GLIDE_FLAGS->second = 0;
            board->ticks = 0;
            return 1;
        }
        break;
    }
    return 0;
}

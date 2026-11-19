#include "dotr/ee/glide_steps.h"

int Taku_Tyumoku_ido_kettei(short unused, short unused2, short limit)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;

    (void)unused;
    (void)unused2;

    if ((DOTR_GLIDE_FLAGS->first == 0 && limit <= *DOTR_GLIDE_LIMIT && DOTR_GLIDE_FLAGS->unknown_0b != 0)
        || board->seen[board->kind[0]] >= 2
        || (board->kind[0] == 15 && board->seen[16] == 0)) {
        switch (DOTR_GLIDE_FLAGS->stage) {
        case 1:
            board->kind[2] = board->kind[0];
            DOTR_GLIDE_STATE->from_x = DOTR_GLIDE_STATE->x;
            DOTR_GLIDE_STATE->from_y = DOTR_GLIDE_STATE->y;
            DOTR_GLIDE_STATE->saved_width = DOTR_GLIDE_HEAD->width;
            DOTR_GLIDE_STATE->saved_height = DOTR_GLIDE_HEAD->height;
            DOTR_GLIDE_STATE->step = 0;
            DOTR_GLIDE_FLAGS->stage++;
            DOTR_GLIDE_TARGET->unknown_22 = 0;
            DOTR_GLIDE_TARGET->unknown_26 = 0;
            DOTR_GLIDE_TARGET->unknown_20 = 1;
            board->unknown_c9 = DOTR_GLIDE_FLAGS->unknown_52;
            break;
        case 2:
            DOTR_GLIDE_STATE->step += 8;
            DOTR_GLIDE_STATE->x = DOTR_GLIDE_STATE->from_x
                + (float)DOTR_GLIDE_STATE->step * (data_00298ba0_glide[board->kind[0]].x - DOTR_GLIDE_STATE->from_x) / 256.0f;
            DOTR_GLIDE_STATE->y = DOTR_GLIDE_STATE->from_y
                + (float)DOTR_GLIDE_STATE->step * (data_00298ba0_glide[board->kind[0]].y - DOTR_GLIDE_STATE->from_y) / 256.0f;
            if (DOTR_GLIDE_STATE->x == data_00298ba0_glide[board->kind[0]].x
                && DOTR_GLIDE_STATE->y == data_00298ba0_glide[board->kind[0]].y) {
                DOTR_GLIDE_STATE->step = 256;
            }
            if (DOTR_GLIDE_STATE->step >= 256) {
                DOTR_GLIDE_STATE->step = 0;
                DOTR_GLIDE_STATE->x = data_00298ba0_glide[board->kind[0]].x;
                DOTR_GLIDE_STATE->y = data_00298ba0_glide[board->kind[0]].y;
                DOTR_GLIDE_FLAGS->stage = 3;
                if (board->kind[0] == 15 && board->seen[16] == 0) {
                    board->ticks = 12040;
                }
                return 1;
            }
            break;
        case 3:
            if (DOTR_GLIDE_FLAGS->hold <= 0 && DOTR_GLIDE_TARGET->unknown_20 == 3) {
                Taku_Camp_Set_Go_Duel();
            }
            break;
        }
    }
    return 0;
}

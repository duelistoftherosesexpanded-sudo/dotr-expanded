#include "dotr/ee/glide_steps.h"

int Taku_Tyumoku_ido_katimake(int unused, short pick)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;
    short go;

    (void)unused;

    DOTR_GLIDE_FLAGS->picked = pick;
    switch (DOTR_GLIDE_FLAGS->phase) {
    case 0:
        if (board->seen[pick] == 0) {
            board->seen[pick] = 1;
        }
        DOTR_GLIDE_FLAGS->second = 1;
        DOTR_GLIDE_STATE->x = data_00298ba0_glide[pick].x;
        DOTR_GLIDE_STATE->y = data_00298ba0_glide[pick].y;
        DOTR_GLIDE_HEAD->width = 320;
        DOTR_GLIDE_HEAD->height = 190;
        DOTR_GLIDE_STATE->unknown_30 = 0;
        DOTR_GLIDE_STATE->wait = 0;
        DOTR_GLIDE_FLAGS->unknown_0b = 0;
        DOTR_GLIDE_FLAGS->unknown_0d = 0;
        DOTR_GLIDE_FLAGS->countdown = 100;
        board->kind[0] = pick;
        board->kind[1] = pick;
        board->kind[2] = pick;
        DOTR_GLIDE_TARGET->unknown_22 = 0;
        DOTR_GLIDE_TARGET->unknown_26 = 0;
        DOTR_GLIDE_TARGET->unknown_20 = 0;
        DOTR_GLIDE_FLAGS->unknown_96 = 0;
        DOTR_GLIDE_FLAGS->unknown_3b = 0;
        DOTR_GLIDE_FLAGS->fading = 0;
        DOTR_GLIDE_FLAGS->phase = 1;
        break;
    case 1:
        if (DOTR_GLIDE_STATE->wait < 256) {
            break;
        }
        DOTR_GLIDE_FLAGS->unknown_0b = 1;
        DOTR_GLIDE_FLAGS->unknown_0d = 1;
        DOTR_GLIDE_FLAGS->fade = 0;
        if (board->ticks >= 300) {
            go = 0;
            if (board->mode == 0) {
                switch (board->kind[0]) {
                case 2:
                case 4:
                case 5:
                case 6:
                case 8:
                case 9:
                case 10:
                    go = 1;
                    break;
                case 11:
                    if (board->unknown_c9 == 0) {
                        go = 1;
                    }
                    break;
                }
            } else {
                switch (board->kind[0]) {
                case 12:
                case 13:
                case 14:
                case 18:
                case 19:
                case 20:
                case 21:
                case 0:
                    go = 1;
                    break;
                }
            }
            if (go) {
                DOTR_GLIDE_FLAGS->phase = 2;
            } else {
                DOTR_GLIDE_FLAGS->phase = 3;
            }
        } else {
            DOTR_GLIDE_FLAGS->phase = 3;
        }
        break;
    case 2:
        if (DOTR_GLIDE_FLAGS->countdown > 0) {
            break;
        }
        DOTR_GLIDE_FLAGS->fading = 1;
        DOTR_GLIDE_FLAGS->total += 4096;
        if (DOTR_GLIDE_FLAGS->fade < 4096) {
            DOTR_GLIDE_FLAGS->fade += 32;
            if (DOTR_GLIDE_FLAGS->fade >= 4096) {
                DOTR_GLIDE_FLAGS->fade = 4096;
                DOTR_GLIDE_FLAGS->phase = 3;
                DOTR_GLIDE_FLAGS->fading = 0;
            }
        }
        break;
    }
    return 0;
}

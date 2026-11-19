#include "dotr/ee/glide_steps.h"

void Taku_Dodai_Root_init(short side, short row, signed char value, short *list, short count)
{
    struct DotrGlideBank *bank = DOTR_GLIDE_BANK;
    struct DotrGlideBoard *board = &data_00299e10_glide;
    short i, j, k, n, c, m;

    DOTR_GLIDE_STATE->step = 0;
    DOTR_GLIDE_FLAGS->first = 1;
    DOTR_GLIDE_FLAGS->stage = 0;
    DOTR_GLIDE_FLAGS->unknown_0b = 0;
    DOTR_GLIDE_FLAGS->hold = 0;
    DOTR_GLIDE_FLAGS->second = 1;
    board->kind[1] = row;
    DOTR_GLIDE_TARGET->unknown_22 = 0;
    DOTR_GLIDE_TARGET->unknown_26 = 0;
    DOTR_GLIDE_TARGET->unknown_20 = 0;
    DOTR_GLIDE_FLAGS->unknown_fc = 0;
    board->seen[row] = value;
    for (i = 0; i < count; i++) {
        n = list[i];
        if (side == 0 && data_00298ba0_glide[row].marks[i] == 2) {
            k = data_00299020_glide[side][n].other_cell;
        } else {
            k = data_00299020_glide[side][n].cell;
        }
        if (board->seen[k] == 0) {
            bank->unknown_52ec[k] = 0;
            bank->unknown_532c[k] = 0;
            board->seen[k] = 0;
            bank->records[side][n].unknown_202 = 0;
            bank->unknown_42ac[side][n] = 0;
            for (j = 0; j < 64; j++) {
                bank->unknown_42ec[side][n][j] = 0;
            }
        }
    }
    if (side == 0 && bank->records[side][12].unknown_202 == 0) {
        c = 0;
        for (m = 2; m <= 10; m++) {
            if (board->seen[m] >= 3) {
                c++;
                if (c >= 8) {
                    bank->saved_ticks = board->ticks;
                    board->ticks = 11120;
                }
            }
        }
    }
}

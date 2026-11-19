#include "dotr/ee/glide_steps.h"

static inline short may_score(struct DotrGlideBoard *board)
{
    if (board->unknown_01[0] != 0 && board->unknown_01[1] != 0) {
        board->ticks = 0;
        return 0;
    }
    if (board->seen[board->kind[0]] < 3) {
        return 1;
    }
    if (board->mode == 0 && board->kind[0] == 11) {
        if (board->unknown_c9 == 1) {
            return 1;
        }
        board->ticks = 0;
        return 0;
    }
    board->ticks = 0;
    return 0;
}

/* win is 1 after a won duel and 0 after a lost one. */
void Taku_Camp_What_KATIMAKE(int win)
{
    struct DotrGlideBoard *board = &data_00299e10_glide;

    data_00299e10_glide.kind[1] = data_00299e10_glide.kind[2];
    data_00299e10_glide.kind[0] = data_00299e10_glide.kind[2];
    data_00299e10_glide.streak++;
    if (data_00299e10_glide.streak >= 5) {
        board->streak = 5;
    }
    if (win != 0) {
        board->wins[0]++;
        if (board->wins[0] >= 9999) {
            board->wins[0] = 9999;
        }
        board->row_wins[0][board->unknown_c4]++;
        if (board->row_wins[0][board->unknown_c4] >= 9999) {
            board->row_wins[0][board->unknown_c4] = 9999;
        }
    } else {
        board->wins[1]++;
        if (board->wins[1] >= 9999) {
            board->wins[1] = 9999;
        }
        board->row_wins[1][board->unknown_c4]++;
        if (board->row_wins[1][board->unknown_c4] >= 9999) {
            board->row_wins[1][board->unknown_c4] = 9999;
        }
    }
    if (may_score(board)) {
        if (win != 0) {
            switch (board->kind[0]) {
            case 11:
                switch (board->unknown_c9) {
                case 0:
                    board->ticks = data_00298ba0_glide[board->kind[0]].marks[4];
                    break;
                default:
                    board->ticks = 313;
                    break;
                }
                break;
            default:
                board->ticks = data_00298ba0_glide[board->kind[0]].marks[4];
                break;
            }
        } else if ((signed char)win == 0) {
            switch (board->kind[0]) {
            case 11:
                switch (board->unknown_c9) {
                case 0:
                    board->ticks = data_00298ba0_glide[board->kind[0]].marks[5];
                    break;
                default:
                    board->ticks = 213;
                    break;
                }
                break;
            default:
                board->ticks = data_00298ba0_glide[board->kind[0]].marks[5];
                break;
            }
        }
    }
    ACT2_Rock = 1;
}

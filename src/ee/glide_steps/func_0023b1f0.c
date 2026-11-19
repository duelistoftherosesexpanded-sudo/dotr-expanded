#include "dotr/ee/glide_steps.h"

/* The two loops keep the block at 0x1ed0000 and the board at 0x299e10 in registers; the
 * tail addresses the block as a literal, like the other users of 0x1ed0000. The two
 * projection inputs take 16 bytes each on the stack although three floats are stored. */
void TkCamp_Act2(int force)
{
    struct DotrCampInitBlock *work;
    struct DotrGlideBoard *board;
    short state;
    short i;
    short on;
    struct DotrGlideVector4 pos;
    struct DotrGlideVector4 pos2;
    int sx;
    int sy;
    int dx;
    int dy;
    int reach;
    float px;
    float py;

    board = &data_00299e10_glide;
    work = DOTR_CAMP_INIT;
    state = 0;
    Taku_Coord_Set(&prim_data_glide);
    for (i = 0; i <= 21; i++) {
        pos.x = data_00298ba0_glide[i].x;
        pos.y = data_00298ba0_glide[i].unknown_04 + 8.65f;
        pos.z = data_00298ba0_glide[i].y;
        pos2.x = data_00298ba0_glide[i].x + 5.0f;
        pos2.y = data_00298ba0_glide[i].unknown_04 + 8.65f;
        pos2.z = data_00298ba0_glide[i].y;
        on = 0;
        if (force != 0) {
            on = 1;
        }
        if (*(signed char *)&work->unknown_18c != 0) {
            on = 1;
        } else if (board->mode == 0) {
            if (i == 0 || i == 11) {
                on = 1;
            } else if (i > 0 && i < 11 && board->seen[i] != 0) {
                on = 1;
            }
        } else {
            if (i == 0 || i == 11) {
                on = 1;
            } else if (i >= 12 && i < 22 && board->seen[i] != 0) {
                on = 1;
            }
        }
        if (on) {
            Taku_3D_2D_Set(&work->unknown_8c[i], &work->unknown_10c[i], &pos);
            Taku_3D_2D_Set(&sx, &sy, &pos2);
            reach = sx - work->unknown_8c[i];
            if (ACT2_Rock == 0) {
                if (work->unknown_5373 != 0) {
                    work->width = work->unknown_8c[board->kind[0]];
                    work->height = work->unknown_10c[board->kind[0]];
                }
                dy = work->height - work->unknown_10c[i];
                dx = work->width - work->unknown_8c[i];
                py = func_001a6c48(dy, 2.0f);
                px = func_001a6c48(dx, 2.0f);
                if (func_001a6c60(px + py) < reach) {
                    if (work->unknown_6c == 0 || work->unknown_5373 != 0) {
                        if (i != board->kind[0]) {
                            board->kind[0] = i;
                        }
                    }
                    if (board->kind[0] == i) {
                        state = 1;
                    }
                    if (work->unknown_5373 != 0) {
                        work->width = work->unknown_8c[board->kind[0]];
                        work->height = work->unknown_10c[board->kind[0]];
                    }
                }
            } else {
                state = 1;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (camp_sys_icon[i].w[0] - 2 <= work->width
            && work->width <= camp_sys_icon[i].w[2] + 2
            && camp_sys_icon[i].w[1] - 2 <= work->height
            && work->height <= camp_sys_icon[i].w[3] + 2) {
            work->unknown_80 = i;
            work->unknown_547a = 1;
            state = 2;
        }
    }
    if ((unsigned short)DOTR_CAMP_INIT->unknown_04 != 1) {
        switch (state) {
        case 0:
            DOTR_CAMP_INIT->unknown_6a = 0;
            DOTR_CAMP_INIT->unknown_7e = 0;
            DOTR_CAMP_INIT->unknown_04 = 0;
            break;
        case 1:
            if (board->kind[0] == 11) {
                switch (board->seen[board->kind[0]]) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    if (DOTR_CAMP_INIT->unknown_72 == 0 || DOTR_CAMP_INIT->unknown_537a != 0) {
                        DOTR_CAMP_INIT->unknown_6a = 1;
                        DOTR_CAMP_INIT->unknown_7e = 0;
                        DOTR_CAMP_INIT->unknown_04 = 0;
                        DOTR_CAMP_INIT->unknown_04 = 2;
                    }
                    break;
                }
            } else {
                switch (board->seen[board->kind[0]]) {
                case 1:
                case 2:
                case 3:
                    if (DOTR_CAMP_INIT->unknown_72 == 0 || DOTR_CAMP_INIT->unknown_537a != 0) {
                        DOTR_CAMP_INIT->unknown_6a = 1;
                        DOTR_CAMP_INIT->unknown_7e = 0;
                        DOTR_CAMP_INIT->unknown_04 = 2;
                    }
                    break;
                }
            }
            break;
        case 2:
            DOTR_CAMP_INIT->unknown_7e = 1;
            DOTR_CAMP_INIT->unknown_6a = 0;
            DOTR_CAMP_INIT->unknown_04 = 2;
            DOTR_CAMP_INIT->unknown_547a = 1;
            break;
        }
    } else {
        DOTR_CAMP_INIT->unknown_6a = 0;
        DOTR_CAMP_INIT->unknown_7e = 0;
    }
    if (DOTR_CAMP_INIT->height < 120) {
        DOTR_CAMP_INIT->unknown_224 -= 8;
        if (DOTR_CAMP_INIT->unknown_224 < 97) {
            DOTR_CAMP_INIT->unknown_224 = 96;
        }
    } else {
        DOTR_CAMP_INIT->unknown_224 += 8;
        if (DOTR_CAMP_INIT->unknown_224 >= 128) {
            DOTR_CAMP_INIT->unknown_224 = 128;
        }
    }
    if (DOTR_CAMP_INIT->height >= 340) {
        DOTR_CAMP_INIT->unknown_226 -= 8;
        if (DOTR_CAMP_INIT->unknown_226 < 97) {
            DOTR_CAMP_INIT->unknown_226 = 96;
        }
    } else {
        DOTR_CAMP_INIT->unknown_226 += 8;
        if (DOTR_CAMP_INIT->unknown_226 >= 128) {
            DOTR_CAMP_INIT->unknown_226 = 128;
        }
    }
    if (DOTR_CAMP_INIT->width >= 456) {
        DOTR_CAMP_INIT->unknown_228 -= 8;
        if (DOTR_CAMP_INIT->unknown_228 < 97) {
            DOTR_CAMP_INIT->unknown_228 = 96;
        }
    } else {
        DOTR_CAMP_INIT->unknown_228 += 8;
        if (DOTR_CAMP_INIT->unknown_228 >= 128) {
            DOTR_CAMP_INIT->unknown_228 = 128;
        }
    }
}

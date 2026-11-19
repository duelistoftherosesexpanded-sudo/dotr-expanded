#include "dotr/ee/camp_bun_hyouji.h"

/* 0x2311d0. The work area is addressed by its literal through AT; W is its base as a DotrBunWork. */
#ifndef __MWERKS__
/* Host build only: the code leaves some locals unset on some paths and passes function addresses
 * as untyped pointers, which the target compiler accepts. */
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

/* MATCHING DEVICE: the work area by its literal address (camp_bun_hyouji.h). */
#define AT(a) DOTR_BUN_HYOUJI_AT(a)

#define W ((DotrBunWork *)AT(0x01ED0000))

int Taku_Camp_BUN_HYOUJI(int p) {
    short flagB;
    short i;
    DotrBunWork *w;
    signed char *ctx;
    short j;
    short count;
    unsigned char col[4][4];
    DotrBunBox box;
    DotrBunRect rect;
    DotrBunG4 g4;
    DotrBunBox box2;
    DotrBunWin win;
    DotrBunWin win2;
    int msg;
    short flagA;

    w = W;
    msg = 0;
    flagB = 0;
    flagA = 0;
    ctx = &data_00299e10_bunhyouji;
    rect = data_0029aeb0_bunhyouji;
    for (i = 0; i < 4; i++) {
        col[i][0] = col[i][1] = col[i][2] = 0x80;
        col[i][3] = 0x80;
    }
    w->f5468 = 0;
    if (w->step != 0) {
        if (w->mode == 0) {
            DotrBunInfo *info;
            int x;
            short n;
            int ty;
            int y;

            g4 = data_00298af6_bunhyouji;
            p = SzMsg_LoadFont_Pipe(p, 1, w->msg, 0, 0x16);
            p = RhText_SetTextureFlush(p);
            info = SzMsg_GetInfo(1);
            box = info->box;
            rect.v[2] = box.w;
            count = box.h / 22;
            if (count > 3) {
                count = 3;
            }
            if (*(short *)(ctx + 0xC2) < 200) {
                w->f5468 = 0;
            } else if (*(short *)(ctx + 0xC2) < 300) {
                w->f5468 = 1;
            } else {
                w->f5468 = 2;
                if (*(short *)(ctx + 0xC4) == 14 && w->f537D == 1) {
                    w->f5468 = 3;
                }
            }
            switch (w->step) {
            case 1:
                w->f53A0 = 0;
                for (j = 0; j < 3; j++) {
                    for (i = 0; i < 4; i++) {
                        w->tbl[j + 1][i] = 0;
                    }
                }
                w->step++;
                break;
            case 2:
                w->f53A0 += 20;
                if (w->f53A0 >= 100) {
                    w->f53A0 = 100;
                    w->step++;
                }
                break;
            case 3:
                w->f540A = 10;
                for (j = 0; j < count; j++) {
                    for (i = 0; i < 4; i++) {
                        switch (i) {
                        case 1:
                            if (w->tbl[j + 1][2] >= 0x80) {
                                w->tbl[j + 1][i] += 0x20;
                            }
                            break;
                        case 2:
                            if (w->tbl[j + 1][0] > 0x40) {
                                w->tbl[j + 1][i] += 0x20;
                            }
                            break;
                        case 3:
                            if (w->tbl[j + 1][1] > 0x20) {
                                w->tbl[j + 1][i] += 0x20;
                            }
                            break;
                        default:
                            if (j == 0) {
                                w->tbl[j + 1][i] += 0x20;
                            } else if (w->tbl[j][1] >= 0x80) {
                                w->tbl[j + 1][i] += 0x20;
                            }
                            break;
                        }
                        if (w->tbl[j + 1][i] >= 0x80) {
                            w->tbl[j + 1][i] = 0x80;
                        }
                    }
                }
                if (w->tbl[count][3] >= 0x80) {
                    w->step++;
                }
                break;
            case 4:
                break;
            case 5:
                if (w->f537D < w->f537C) {
                    for (j = 0; j < count; j++) {
                        for (i = 0; i < 4; i++) {
                            w->tbl[j + 1][i] -= 0x10;
                            if (w->tbl[j + 1][i] <= 0) {
                                w->tbl[j + 1][i] = 0;
                            }
                        }
                    }
                    if (w->tbl[count][3] <= 0) {
                        for (j = 0; j < 3; j++) {
                            for (i = 0; i < 4; i++) {
                                w->tbl[j + 1][i] = 0;
                            }
                        }
                        w->step = 2;
                        w->f537D++;
                        Taku_Camp_BUN_HANTEI();
                    }
                } else {
                    w->step = 11;
                }
                break;
            case 11:
                for (j = 0; j < count; j++) {
                    for (i = 0; i < 4; i++) {
                        w->tbl[j + 1][i] -= 0x10;
                        if (w->tbl[j + 1][i] <= 0) {
                            w->tbl[j + 1][i] = 0;
                        }
                    }
                }
                if (w->tbl[count][3] <= 0) {
                    for (j = 0; j < 3; j++) {
                        for (i = 0; i < 4; i++) {
                            w->tbl[j + 1][i] = 0;
                        }
                    }
                    w->step = 12;
                }
                break;
            case 12:
                if (0 <= w->f53A0) {
                    w->f53A0 -= 20;
                    if (w->f53A0 <= 0) {
                        short ok;

                        w->f53A0 = 0;
                        if (w->f018C != 0 && *(short *)(ctx + 0xC2) >= 200) {
                            w->f537D = 0;
                            w->step = 0;
                            w->f537A = 0;
                            *(short *)(ctx + 0xC2) = 0;
                            w->f536C = 0;
                            w->f0060 = 0;
                            w->f5372 = 0;
                            w->f5373 = 0;
                            w->f5375 = 0;
                            break;
                        }
                        w->f53AB = 0;
                        ok = 1;
                        if (*(short *)(ctx + 0xC2) >= 300 && *(short *)(ctx + 0xC2) < 10000) {
                            if (w->f537A == 3) {
                                ok = 1;
                            } else {
                                ok = 0;
                            }
                        }
                        if (w->f5440 <= 0 && ok) {
                            w->f5440 = 0;
                            if (ctx[0] == 0 && *(short *)(ctx + 0xC2) == 0x139 &&
                                ctx[3 + w->f53C0] != 0 && *(short *)(ctx + 0xC2) >= 300 &&
                                *(short *)(ctx + 0xC2) < 10000) {
                                *(short *)(ctx + 0xC2) = 0x2BAC;
                                ctx[0xE] = 5;
                                break;
                            }
                            if (ctx[3 + w->f53C0] < 3 && *(short *)(ctx + 0xC2) >= 300 &&
                                *(short *)(ctx + 0xC2) < 10000) {
                                if (ctx[0] == 0) {
                                    if (*(short *)(ctx + 0xC2) == 0x134) {
                                        *(short *)(ctx + 0xC2) = 0x2B84;
                                        ctx[3 + ctx[0xC6]] = 3;
                                    } else if (*(short *)(ctx + 0xC2) == 0x138) {
                                        *(short *)(ctx + 0xC2) = 0x2B98;
                                        ctx[0xE] = 3;
                                    } else if (*(short *)(ctx + 0xC2) == 0x139) {
                                        *(short *)(ctx + 0xC2) = 0x2BAC;
                                        ctx[0xE] = 5;
                                    } else {
                                        *(short *)(ctx + 0xC2) -= 200;
                                        ctx[3 + ctx[0xC6]] = 3;
                                    }
                                } else if (*(short *)(ctx + 0xC2) == 0x138 || ((short *)ctx)[0x61] == 0x139) {
                                    *(short *)(ctx + 0xC2) = 0x2F76;
                                    ctx[0xE] = 3;
                                } else {
                                    *(short *)(ctx + 0xC2) -= 200;
                                    ctx[3 + ctx[0xC6]] = 3;
                                }
                                w->f537D = 0;
                                w->step = 0;
                                w->f537A = 0;
                                w->f536C = 0;
                                w->f0060 = 0;
                                w->f5372 = 1;
                                w->f5373 = 0;
                                w->f5375 = 1;
                            } else if (*(short *)(ctx + 0xC2) >= 200) {
                                if (ctx[0] == 0 && *(short *)(ctx + 0xC2) == 0xD5) {
                                    ctx[0xE] = 4;
                                } else if (ctx[3 + ctx[0xC6]] == 1 && *(short *)(ctx + 0xC2) >= 200) {
                                    ctx[3 + ctx[0xC6]] = 2;
                                }
                                w->f537D = 0;
                                w->step = 0;
                                w->f537A = 0;
                                *(short *)(ctx + 0xC2) = 0;
                                w->f536C = 0;
                                w->f0060 = 0;
                                w->f5372 = 0;
                                w->f5373 = 0;
                                w->f5375 = 0;
                            }
                        }
                    }
                }
                break;
            }
            g4.v[5] = data_00298af6_bunhyouji.v[1] + w->f53A0 * (data_00298af6_bunhyouji.v[5] - data_00298af6_bunhyouji.v[1]) / 100;
            g4.v[7] = g4.v[5];
            p = Taku_G4_Set(p, &g4, 0, 0);
            for (i = 0; i < 4; i++) {
                col[i][0] = col[i][1] = col[i][2] = 0x80;
                col[i][3] = w->tbl[0][i];
            }
            n = count;
            x = data_00298af6_bunhyouji.v[0] + 6;
            ty = data_00298af6_bunhyouji.v[1] + 6;
            for (i = 0, y = 0; i < n; i++) {
                short yy = y;

                for (j = 0; j < 4; j++) {
                    col[j][3] = w->tbl[i + 1][j];
                }
                info->box.y = box.y + i * 0x16;
                info->box.h = 0x16;
                p = SzMsg_DrawRectG_Pipe(p, 1, x, ty + yy, &rect, col[0]);
                y = (short)(y + 0x16);
            }
            SzMsg_Free(1);
        } else if (w->mode == 1) {
            short done;

            done = 0;
            if (*(short *)(ctx + 0xC2) == 0x2724) {
                if (w->msg > 0x7F0) {
                    w->lv[0] += 2;
                    if (w->lv[0] >= 200) {
                        w->lv[0] = 200;
                    }
                }
                if (w->msg > 0x7F2) {
                    w->lv[1] += 2;
                    if (w->lv[1] >= 200) {
                        w->lv[1] = 200;
                    }
                }
                if (w->msg > 0x7F3) {
                    w->lv[2] += 2;
                    if (w->lv[2] >= 200) {
                        w->lv[2] = 200;
                    }
                }
                if (w->msg > 0x7F5) {
                    w->lv[3] += 2;
                    if (w->lv[3] >= 200) {
                        w->lv[3] = 200;
                    }
                }
                if (w->msg >= 0x7FB) {
                    w->lv[4] += 2;
                    if (w->lv[4] >= 200) {
                        w->lv[4] = 200;
                        done = 1;
                    }
                }
            }
            switch (w->step) {
            case 1:
                w->f53A0 = 0;
                for (j = 0; j < 4; j++) {
                    w->tbl[0][j] = 0;
                }
                w->step++;
                break;
            case 2:
            case 3:
                if (*(short *)(ctx + 0xC2) == 0x2724) {
                    switch (w->msg) {
                    case 0x7F0:
                        w->lv[0] += 2;
                        if (w->lv[0] >= 100) {
                            w->lv[0] = 100;
                            flagA = 1;
                        }
                        break;
                    case 0x7F1:
                        w->lv[1] += 2;
                        if (w->lv[1] >= 100) {
                            w->lv[1] = 100;
                            flagA = 1;
                        }
                        break;
                    case 0x7F3:
                        w->lv[2] += 2;
                        if (w->lv[2] >= 100) {
                            w->lv[2] = 100;
                            flagA = 1;
                        }
                        break;
                    case 0x7F4:
                        w->lv[3] += 2;
                        if (w->lv[3] >= 100) {
                            w->lv[3] = 100;
                            flagA = 1;
                        }
                        break;
                    case 0x7FA:
                        w->lv[4] += 2;
                        if (w->lv[4] >= 100) {
                            w->lv[4] = 100;
                            flagA = 1;
                        }
                        break;
                    default:
                        flagA = 1;
                        break;
                    }
                }
                for (j = 0; j < 4; j++) {
                    switch (j) {
                    case 1:
                    case 2:
                        if (w->tbl[0][0] > 0x40) {
                            w->tbl[0][j] += 0x20;
                        }
                        break;
                    case 3:
                        if (w->tbl[0][1] > 0x40) {
                            w->tbl[0][j] += 0x20;
                        }
                        break;
                    default:
                        w->tbl[0][j] += 0x20;
                        break;
                    }
                    if (w->tbl[0][j] >= 0x80) {
                        w->tbl[0][j] = 0x80;
                    }
                }
                if (*(short *)(ctx + 0xC2) == 0x2724) {
                    if (flagA && w->tbl[0][3] >= 0x80) {
                        w->step++;
                    }
                } else if (w->tbl[0][3] >= 0x80) {
                    w->step++;
                }
                break;
            case 4:
                break;
            case 5:
                if (*(short *)(ctx + 0xC2) == 0x2724 && w->msg == 0x7F6) {
                    w->lv[3] += 2;
                    if (w->lv[3] >= 200) {
                        w->lv[3] = 200;
                        flagA = 1;
                    }
                }
                if (w->f537D < w->f537C) {
                    for (j = 0; j < 4; j++) {
                        switch (j) {
                        case 1:
                        case 2:
                            if (w->tbl[0][0] < 0x40) {
                                w->tbl[0][j] -= 0x20;
                            }
                            break;
                        case 3:
                            if (w->tbl[0][1] < 0x40) {
                                w->tbl[0][j] -= 0x20;
                            }
                            break;
                        default:
                            w->tbl[0][j] -= 0x20;
                            break;
                        }
                        if (w->tbl[0][j] <= 0) {
                            w->tbl[0][j] = 0;
                            if (*(short *)(ctx + 0xC2) == 0x2724 && w->msg == 0x7F6) {
                                if (j == 3 && flagA) {
                                    w->step = 3;
                                    w->f537D++;
                                    Taku_Camp_BUN_HANTEI();
                                }
                            } else if (j == 3) {
                                w->step = 3;
                                w->f537D++;
                                Taku_Camp_BUN_HANTEI();
                            }
                        }
                    }
                } else {
                    w->step = 11;
                }
                break;
            case 11:
                for (j = 0; j < 4; j++) {
                    switch (j) {
                    case 1:
                    case 2:
                        if (w->tbl[0][0] < 0x40) {
                            w->tbl[0][j] -= 0x20;
                        }
                        break;
                    case 3:
                        if (w->tbl[0][1] < 0x40) {
                            w->tbl[0][j] -= 0x20;
                        }
                        break;
                    default:
                        w->tbl[0][j] -= 0x20;
                        break;
                    }
                    if (w->tbl[0][j] <= 0) {
                        w->tbl[0][j] = 0;
                        if (j == 3) {
                            if (*(short *)(ctx + 0xC2) == 0x2724) {
                                if (done) {
                                    w->step = 12;
                                }
                            } else {
                                w->step = 12;
                            }
                        }
                    }
                }
                break;
            }
            for (i = 0; i < 4; i++) {
                col[i][0] = col[i][1] = col[i][2] = 0x80;
                col[i][3] = w->tbl[0][i];
            }
            p = SzMsg_LoadFont_Pipe(p, 1, w->msg, 0, 0x16);
            p = RhText_SetTextureFlush(p);
            p = SzMsg_DrawX_Pipe(p, 1, 0x59, 0x150, col, SzMsg_DrawRectG_Pipe);
            SzMsg_Free(1);
        } else {
            int y;
            DotrBunInfo *info;
            int n;
            int x;
            int ty;

            p = SzMsg_LoadFont_Pipe(p, 1, w->msg, 0, 0x16);
            p = RhText_SetTextureFlush(p);
            info = SzMsg_GetInfo(1);
            box2 = info->box;
            rect.v[2] = box2.w;
            count = box2.h / 22;
            if (count > 3) {
                count = 3;
            }
            switch (w->step) {
            case 1:
                w->f53A0 = 0;
                for (j = 0; j < 3; j++) {
                    for (i = 0; i < 4; i++) {
                        w->tbl[j + 1][i] = 0;
                    }
                }
                w->step++;
                break;
            case 2:
                if (w->f53B4 != 0) {
                    if (w->f5407 == 0) {
                        w->f5407 = 1;
                    }
                    if (w->f53B6 < 0x400) {
                        w->f53B6 += 0x40;
                    }
                    if (w->f53B6 >= 0x400) {
                        w->f53B6 = 0x400;
                    }
                } else {
                    if (0 < w->f53B6) {
                        w->f53B6 -= 0x40;
                    }
                    if (w->f53B6 <= 0) {
                        w->f53B6 = 0;
                    }
                }
                if (w->f53B5 != 0) {
                    if (w->f5408 == 0) {
                        w->f5408 = 1;
                    }
                    if (w->f53B8 < 0x400) {
                        w->f53B8 += 0x40;
                    }
                    if (w->f53B8 >= 0x400) {
                        w->f53B8 = 0x400;
                    }
                } else {
                    if (0 < w->f53B8) {
                        w->f53B8 -= 0x40;
                    }
                    if (w->f53B8 <= 0) {
                        w->f53B8 = 0;
                    }
                }
                if (w->f53B4 != 0 && w->f53B5 != 0) {
                    if (w->f53B6 >= 0x400 && w->f53B8 >= 0x400) {
                        w->step++;
                    }
                } else if (w->f53B4 != 0 && w->f53B5 == 0) {
                    if (w->f53B6 >= 0x400 && w->f53B8 <= 0) {
                        w->step++;
                    }
                } else if ((signed char)w->f53B4 == 0 && w->f53B5 != 0) {
                    if (w->f53B6 <= 0 && w->f53B8 >= 0x400) {
                        w->step++;
                    }
                } else if (w->f53B6 <= 0 && w->f53B8 <= 0) {
                    w->step++;
                }
                break;
            case 3:
                if (w->mode == 2) {
                    if (w->f53B5 != 0) {
                        w->f5437 += 10;
                        if (w->f5437 >= 100) {
                            w->f5437 = 100;
                            w->f5408 = 10;
                            flagB = 1;
                        }
                    } else if (w->f5437 <= 0) {
                        w->f5437 += 10;
                        if (0 <= w->f5437) {
                            flagB = 1;
                            w->f5437 = 0;
                        }
                    } else {
                        w->f5437 -= 10;
                        if (w->f5437 <= 0) {
                            flagB = 1;
                            w->f5437 = 0;
                        }
                    }
                }
                if (w->mode == 3) {
                    if (w->f53B4 != 0) {
                        w->f5437 -= 10;
                        if (w->f5437 < -100) {
                            w->f5437 = -100;
                            w->f5407 = 10;
                            flagB = 1;
                        }
                    } else if (w->f5437 <= 0) {
                        w->f5437 += 10;
                        if (0 <= w->f5437) {
                            flagB = 1;
                            w->f5437 = 0;
                        }
                    } else {
                        w->f5437 -= 10;
                        if (w->f5437 <= 0) {
                            flagB = 1;
                            w->f5437 = 0;
                        }
                    }
                }
                if (flagB) {
                    for (j = 0; j < count; j++) {
                        for (i = 0; i < 4; i++) {
                            switch (i) {
                            case 1:
                                if (w->tbl[j + 1][2] >= 0x80) {
                                    w->tbl[j + 1][i] += 0x20;
                                }
                                break;
                            case 2:
                                if (w->tbl[j + 1][0] > 0x40) {
                                    w->tbl[j + 1][i] += 0x20;
                                }
                                break;
                            case 3:
                                if (w->tbl[j + 1][1] > 0x20) {
                                    w->tbl[j + 1][i] += 0x20;
                                }
                                break;
                            default:
                                if (j == 0) {
                                    w->tbl[j + 1][i] += 0x20;
                                } else if (w->tbl[j][1] >= 0x80) {
                                    w->tbl[j + 1][i] += 0x20;
                                }
                                break;
                            }
                            if (w->tbl[j + 1][i] >= 0x80) {
                                w->tbl[j + 1][i] = 0x80;
                            }
                        }
                    }
                }
                if (w->tbl[count][3] >= 0x80) {
                    w->step++;
                }
                break;
            case 4:
                break;
            case 5:
                if (w->f537D < w->f537C) {
                    for (j = 0; j < count; j++) {
                        for (i = 0; i < 4; i++) {
                            w->tbl[j + 1][i] -= 0x10;
                            if (w->tbl[j + 1][i] <= 0) {
                                w->tbl[j + 1][i] = 0;
                            }
                        }
                    }
                    if (w->tbl[count][3] <= 0) {
                        for (j = 0; j < 3; j++) {
                            for (i = 0; i < 4; i++) {
                                w->tbl[j + 1][i] = 0;
                            }
                        }
                        w->step = 2;
                        w->f537D++;
                        Taku_Camp_BUN_HANTEI();
                    }
                } else {
                    w->step = 11;
                }
                break;
            case 11:
                for (j = 0; j < count; j++) {
                    for (i = 0; i < 4; i++) {
                        w->tbl[j + 1][i] -= 0x10;
                        if (w->tbl[j + 1][i] <= 0) {
                            w->tbl[j + 1][i] = 0;
                        }
                    }
                }
                if (w->tbl[count][3] <= 0) {
                    for (j = 0; j < 3; j++) {
                        for (i = 0; i < 4; i++) {
                            w->tbl[j + 1][i] = 0;
                        }
                    }
                    w->step = 12;
                }
                break;
            case 13:
                if (w->f5437 <= 0) {
                    w->f5437 += 10;
                    if (0 <= w->f5437) {
                        flagB = 1;
                        w->f5437 = 0;
                    }
                } else {
                    w->f5437 -= 10;
                    if (w->f5437 <= 0) {
                        flagB = 1;
                        w->f5437 = 0;
                    }
                }
                if (flagB) {
                    if (0 < w->f53B6) {
                        w->f53B6 -= 0x40;
                    }
                    if (0 < w->f53B8) {
                        w->f53B8 -= 0x40;
                    }
                    if (w->f53B6 <= 0) {
                        w->f53B6 = 0;
                        if (w->f5407 != 0) {
                            ShFace_Close(w->face0);
                            w->f5407 = 0;
                        }
                    }
                    if (w->f53B8 <= 0) {
                        w->f53B8 = 0;
                        if (w->f5408 != 0) {
                            ShFace_Close(w->face1);
                            w->f5408 = 0;
                        }
                    }
                    if (w->f53B6 <= 0 && w->f53B8 <= 0) {
                        w->step = 14;
                        w->f53B4 = 0;
                        w->f53B5 = 0;
                    }
                }
                break;
            }
            if (w->mode == 2) {
                x = 0xA0;
                ty = 0x160;
            } else {
                x = 0x18;
                ty = 0x30;
            }
            n = count;
            for (i = 0, y = 0; i < n; i++) {
                short yy = y;

                for (j = 0; j < 4; j++) {
                    col[j][3] = w->tbl[i + 1][j];
                }
                info->box.y = box2.y + i * 0x16;
                info->box.h = 0x16;
                if (w->f5483 == 0) {
                    p = SzMsg_DrawRectG_Pipe(p, 1, x, ty + yy, &rect, col[0]);
                }
                y = (short)(y + 0x16);
            }
            SzMsg_Free(1);
        }
    }
    if (w->f006E != 0 && w->f006C == 0 && w->f5374 == 0 && w->f0004 == 0 &&
        w->f0072 == 0 && w->f547A == 0 && w->f5481 != 0 && SzEffect_IsUse2(-1, -1) == 0 &&
        w->f5376 != 0 && w->f5482 == 0) {
        DotrBunInfo *info;
        float sy;
        float sx;

        win = data_00297b72_bunhyouji;
        msg = 0x886;
        p = SzMsg_LoadFont_Pipe(p, 1, msg, 0, 0x16);
        p = RhText_SetTextureFlush(p);
        info = SzMsg_GetInfo(1);
        win.x1 = (short)info->box.w + 12;
        win.y1 = (short)info->box.h + 12;
        p = Taku_Window_Set(p, &win, 2, 0, 0xC4, 0x30, 0, 0x80, 0x40);
        for (i = 0; i < 4; i++) {
            col[i][0] = col[i][1] = col[i][2] = 0x80;
            col[i][3] = 0x80;
        }
        sy = sx = 1.0f;
        p = SzMsg_DrawRatioX_Pipe(p, 1, 0xCA, 0x36, sx, sy, col, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    }
    switch (*(short *)(ctx + 0xC2)) {
    case 0x2B70:
        msg = 0x8AB;
        break;
    case 0x2B74:
        msg = 0x8AD;
        break;
    case 0x2B8D:
        msg = 0x8DF;
        break;
    }
    if (w->f5483 != 0) {
        DotrBunInfo *info;
        int x;

        win2 = data_00297b72_bunhyouji;
        p = SzMsg_LoadFont_Pipe(p, 1, msg, 0, 0x16);
        p = RhText_SetTextureFlush(p);
        info = SzMsg_GetInfo(1);
        win2.x1 = (short)info->box.w + 12;
        win2.y1 = (short)info->box.h + 12;
        x = 0x140 - (info->box.w >> 1);
        p = Taku_Window_Set(p, &win2, 2, 0, x, 0xE0, 0, 0x80, 0x40);
        for (i = 0; i < 4; i++) {
            col[i][0] = col[i][1] = col[i][2] = 0x80;
            col[i][3] = 0x80;
        }
        p = SzMsg_DrawRatioX_Pipe(p, 1, x + 6, 0xE6, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe);
        SzMsg_Free(1);
    }
    return p;
}

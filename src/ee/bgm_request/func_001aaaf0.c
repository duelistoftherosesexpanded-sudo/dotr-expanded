#include "dotr/ee/bgm_request.h"

void SzGame_ReqBGM(int n, int b)
{
    int stage;
    int flag;
    int bgm;
    struct DotrBgmGame *g;

    g = &gSzGame_gl_bgm;
    switch (n % 1000) {
    case -2:
    case -1:
        func_00217a10_bgm(256);
        return;
    case 0:
        func_00217980_bgm(7, b);
        break;
    case 1:
        stage = g->stage;
        flag = g->flag;
        switch (stage) {
        case 0:
            stage = 7;
            break;
        case 1:
            stage = 10;
            break;
        case 19:
            stage = 11;
            break;
        case 20:
            stage = 12;
            break;
        case 21:
            stage = 44;
            break;
        default:
            if (stage < 22) {
                if (flag) {
                    stage = 9;
                } else {
                    stage = 8;
                }
            } else {
                stage = 40;
            }
            break;
        }
        if (stage >= 0) {
            stage = n / 1000 * 1000 + stage;
            func_00217980_bgm(stage, b);
        }
        break;
    case 2:
    case 3:
    case 4:
        break;
    case 5:
        bgm = -1;
        switch (g->place) {
        case 7:
            bgm = 13;
            break;
        case 3:
            bgm = 14;
            break;
        case 2:
            bgm = 15;
            break;
        case 4:
            bgm = 16;
            break;
        case 0:
            bgm = 17;
            break;
        case 1:
            bgm = 18;
            break;
        case 9:
            bgm = 19;
            break;
        case 5:
            bgm = 20;
            break;
        case 6:
            bgm = 21;
            break;
        case 8:
            bgm = 22;
            break;
        }
        if (bgm >= 0) {
            func_00217980_bgm(bgm, b);
        }
        break;
    case 6:
        func_00217980_bgm(5, b);
        break;
    case 7:
        func_00217980_bgm(5, b);
        break;
    case 8:
        func_00217980_bgm(41, b);
        break;
    case 9:
        func_00217980_bgm(3, b);
        break;
    case 10:
        func_00217980_bgm(2, b);
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        break;
    case 15:
        func_00217980_bgm(21, b);
        break;
    }
}

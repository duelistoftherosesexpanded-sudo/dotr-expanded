#include "dotr/ee/camp_tikei.h"

DotrTikeiWord Taku_Camp_SYS_ICON(DotrTikeiWord cursor)
{
    struct DotrTikeiItem item;
    struct DotrTikeiMenu *b;
    short y;
    int i;
    char *text;
    signed char n;

    y = 128 - ((DOTR_TIKEI_SLIDE << 7) >> 8);
    b = DOTR_TIKEI_MENU_BLOCK;
    cursor = Taku_G4_Set(cursor, &data_002982f8, 0, y);
    for (i = 0; i < 4; i++) {
        item = camp_sys_icon[i];
        if (i == b->pick && b->menu != 0) {
            b->grow[i] += 4;
            if (b->grow[i] >= 16) {
                b->grow[i] = 16;
            }
            item.w[0] = camp_sys_icon[i].w[0] - ((b->grow[i] * 6) >> 4);
            item.w[1] = camp_sys_icon[i].w[1] - ((b->grow[i] * 6) >> 4);
            item.w[2] = camp_sys_icon[i].w[2] + ((b->grow[i] * 6) >> 4);
            item.w[3] = camp_sys_icon[i].w[3] + ((b->grow[i] * 6) >> 4);
        } else if (0 < b->grow[i]) {
            b->grow[i] -= 4;
            if (b->grow[i] <= 0) {
                b->grow[i] = 0;
            }
        }
        cursor = Taku_Obj_Set(cursor, &item, 0, y);
        if (b->menu == 1) {
            if (i == b->pick && b->menu != 0) {
                if (i == 0) {
                    text = data_0030f102;
                    n = 4;
                } else if (i == 1) {
                    text = data_0030f107;
                    n = 4;
                } else if (i == 2) {
                    text = data_0030f10c;
                    n = 6;
                } else {
                    text = data_0030f113;
                    n = 4;
                }
                cursor = SzDuel_DrawWindow_Font_Pipe(cursor, 562, y + 335, 1, 1, n, text, 0);
            }
        }
    }
    if (DOTR_TIKEI_MENU == 0) {
        cursor = Taku_Obj_Set(cursor, &camp_sys_icon_botan[0], 0, y);
    } else {
        cursor = Taku_Obj_Set(cursor, &camp_sys_icon_botan[1], 0, y);
    }
    if (DOTR_TIKEI_MENU == 0) {
        cursor = SzDuel_DrawWindow_Font_Pipe(cursor, 562, y + 335, 1, 1, 6, data_0030f118, 0);
    }
    return cursor;
}

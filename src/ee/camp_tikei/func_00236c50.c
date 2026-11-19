#include "dotr/ee/camp_tikei.h"

DotrTikeiWord Taku_Camp_TIKEI_WINDOW(DotrTikeiWord cursor)
{
    signed char *st;
    short no;
    short x;
    signed char f;
    int i;
    int j;
    short y;

    no = SelectedBattleArena[0];
    st = data_00299e10;
    if (DOTR_TIKEI_OPEN != 0) {
        DOTR_TIKEI_STEP += 2;
        if (DOTR_TIKEI_STEP > 16) {
            DOTR_TIKEI_STEP = 16;
        }
    } else {
        DOTR_TIKEI_STEP -= 2;
        if (DOTR_TIKEI_STEP < 0) {
            DOTR_TIKEI_STEP = 0;
        }
    }
    x = 256 - ((DOTR_TIKEI_STEP << 8) >> 4);
    if (DOTR_TIKEI_MODE == 1 && st[198] == 11) {
        f = st[0] ^ 1;
    } else if (st[198] == 11) {
        f = st[0] ^ 1;
    } else if (data_00298ba0_tikei[st[198]].kind == 8) {
        f = 0;
    } else {
        f = 1;
    }
    if (DOTR_TIKEI_STEP != 0) {
        cursor = Taku_Window_Set(cursor, &camp_window_Map_data[0], f, 0, x, 0, 0, 128, DOTR_TIKEI_DEPTH);
        cursor = Taku_Camp_TIKEI_Map_Hyouji(cursor, no, x, 0);
        cursor = Taku_Window_Set(cursor, &camp_window_Map_data[1], f, 0, x, 0, 0, 128, 128);
        cursor = Taku_Window_Set(cursor, &camp_window_Map_data[2], st[0] ^ 1, 0, x, 0, 0, 128, 128);
        for (i = 0; i < 10; i++) {
            cursor = Taku_Obj_Set(cursor, &camp_window_tikei_partu[i], x, 0);
        }
        for (i = 0, y = 0; i < 10; i++, y += 16) {
            for (j = 0; j < 3; j++) {
                cursor = Taku_Obj_Set(cursor, &camp_window_tikei_partu_gejiwaku[j], x, y);
            }
        }
        cursor = Taku_FT4_Set(cursor, &camp_window_tikei_iro_ue[f], x, 0, 0);
        cursor = Taku_FT4_Set(cursor, &camp_window_tikei_iro_sita[st[0]], x, 0, 2);
    }
    return cursor;
}

#include "dotr/ee/camp_help.h"

DotrHelpWord Taku_Camp_Help_Window(DotrHelpWord cursor)
{
    signed char *st;
    short y;
    short i;
    int ix;
    short yy;
    struct DotrHelpWindows win;
    unsigned char col[4];
    short xx;
    int no;
    int px;
    signed char *blk;

    st = data_00299e10;
    win = data_0029b020;
    col[0] = col[1] = col[2] = 128;
    col[3] = 128;
    y = 512 - ((DOTR_HELP_STEP << 9) >> 3);
    blk = DOTR_HELP_BLOCK;
    if (DOTR_HELP_OPEN != 0) {
        cursor = Taku_Window_Set(cursor, &win.w[0], 2, 0, 128, y + 80, 0, 128, 101);
        cursor = Taku_Window_Set(cursor, &win.w[1], 2, 0, 128, y + 80, 0, 128, 101);
        cursor = Taku_Window_Set(cursor, &win.w[3], st[0] + 10, 0, 282, y + 94, 0, 128, 128);
        cursor = SzDuel_DrawWindow_Font_Pipe(cursor, 296, y + 102, 2, 1, 4, data_0030f113, 0);
        /* MATCHING DEVICE: the int / short / int chain keeps the line x (160, narrowed
         * to short) in a saved register and adds 16 inside the loop; a single variable
         * of either type is folded to the constant 176. */
        px = 160;
        xx = px;
        ix = xx;
        yy = 144;
        cursor = Taku_Window_Set(cursor, &win.w[2], 2, 0, px, y + ((short)(DOTR_HELP_PICK * 36) + yy), 0, 128, 0);
        yy += 2;
        for (i = 0; i < 5; i++) {
            if (i == blk[0x546b]) {
                col[0] = col[1] = col[2] = 255;
                col[3] = 128;
            } else {
                col[0] = col[1] = col[2] = 64;
                col[3] = 128;
            }
            switch (i) {
            case 0:
                no = 3049;
                break;
            case 1:
                no = 3050;
                break;
            case 2:
                no = 3051;
                break;
            case 3:
                no = 3052;
                break;
            default:
                no = 3053;
                break;
            }
            cursor = Taku_Bunsyou_Set(cursor, 1, no, ix + 16, (short)(yy + y), 0, col, 1.0f, 1.0f);
            yy += 36;
        }
    }
    return cursor;
}

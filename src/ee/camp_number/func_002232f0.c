#include "dotr/ee/camp_number.h"

DotrCampWord Taku_Camp_Kazu_Disp(DotrCampWord cursor, int a, int b, int x)
{
    struct DotrCampColor color;
    struct DotrCampRect r;
    struct DotrCampRect rect = data_0029a7d0;
    struct DotrCampInfo *info;
    short i;
    short col;
    short row;
    short xoff;
    int v;

    color.b = 255;
    color.g = 255;
    color.r = 255;
    color.a = 128;
    for (i = 0, xoff = 160; i < 4; i++) {
        v = 0;
        switch (i) {
        case 0:
            v = a % 10;
            break;
        case 1:
            v = 10;
            break;
        case 2:
            v = b / 10 % 10;
            break;
        case 3:
            v = b % 10;
            break;
        }
        if (v == 0) {
            col = 4;
            row = 1;
        } else if (v < 6) {
            col = v - 1;
            row = 0;
        } else if (v < 10) {
            col = v - 6;
            row = 1;
        } else {
            col = 2;
            row = 4;
        }
        cursor = SzMsg_LoadFont_Pipe(cursor, 1, 7, 0, 22);
        cursor = RhText_SetTextureFlush(cursor);
        info = SzMsg_GetInfo(1);
        r = info->rect;
        color.b = 255;
        color.g = 255;
        color.r = 255;
        color.a = 128;
        info->rect.w[0] = r.w[0] + col * 22;
        info->rect.w[1] = r.w[1] + row * 22;
        info->rect.w[2] = 22;
        info->rect.w[3] = 22;
        cursor = SzMsg_DrawRect_Pipe(cursor, 1, xoff + x, 272, &rect, &color);
        SzMsg_Free(1);
        xoff += 16;
    }
    return cursor;
}

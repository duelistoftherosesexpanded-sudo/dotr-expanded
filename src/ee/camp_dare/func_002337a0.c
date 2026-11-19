#include "dotr/ee/camp_dare.h"

/* The locals of each half are declared inside its block: with function-level
 * declarations the same code allocates the saved registers differently. */
DotrDareWord Taku_Camp_BUN_DARE(DotrDareWord cursor)
{
    short xx, yy;
    unsigned char col[4];

    col[0] = col[1] = col[2] = 128;
    col[3] = 128;
    xx = 0;
    yy = 0;
    if (DOTR_DARE_SLIDE0 != 0) {
        short x;
        short y;
        signed char n;
        int no;
        short w;
        struct DotrDareInfo *info;

        w = DOTR_DARE_SLIDE0;
        x = (w * 640 >> 10) - 640;
        y = (w * 256 >> 10) - 256;
        n = DOTR_DARE_SIDE0;
        cursor = Taku_Obj_Set(cursor, &tex_bara_data2[n], x + xx, 0);
        cursor = Taku_Window_Set(cursor, &camp_window_Teki_data[0], n, 0, x + xx, 0, 0, 128, 128);
        no = DOTR_DARE_NAME0 - 195;
        switch (DOTR_DARE_STATE0) {
        case 1:
            DOTR_DARE_FACE0->id = 255;
            /* fall through */
        case 2:
            DOTR_DARE_FACE0->no = no;
            DOTR_DARE_FACE0->kind = data_002999cc[no][DOTR_DARE_LOOK0];
            DOTR_DARE_FACE0->mode = 1;
            DOTR_DARE_FACE0->x = camp_window_Teki_data[3].x + xx;
            DOTR_DARE_FACE0->y = y + (camp_window_Teki_data[3].y + yy);
            ShFace_Request(DOTR_DARE_FACE0);
            DOTR_DARE_STATE0 = 2;
            break;
        case 10:
            DOTR_DARE_FACE0->no = no;
            DOTR_DARE_FACE0->kind = data_002999cc[no][DOTR_DARE_LOOK0];
            DOTR_DARE_FACE0->mode = 3;
            DOTR_DARE_FACE0->x = camp_window_Teki_data[3].x + xx;
            DOTR_DARE_FACE0->y = y + (camp_window_Teki_data[3].y + yy);
            ShFace_Request(DOTR_DARE_FACE0);
            DOTR_DARE_STATE0 = 2;
            break;
        case 100:
            ShFace_Close(DOTR_DARE_FACE0);
            DOTR_DARE_STATE0 = 0;
            break;
        }
        cursor = Taku_Window_Set(cursor, &camp_window_Teki_data[3], n, 0, xx, (unsigned short)(y + yy), 1, 128, 0);
        cursor = Taku_FT4_Set(cursor, &camp_window_teki3[2], xx, y + yy, 0);
        cursor = Taku_FT4_Set(cursor, &camp_window_teki3[3], xx, y + yy, 0);
        cursor = Taku_FT4_Set(cursor, &camp_window_teki3[n], xx, y + yy, 0);
        cursor = SzMsg_LoadFont_Pipe(cursor, 1, DOTR_DARE_NAME0, 0, 22);
        cursor = RhText_SetTextureFlush(cursor);
        info = SzMsg_GetInfo(1);
        if (info->w < 324) {
            cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, x + 53, 21, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe);
        } else {
            cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, x + 53, 21, 324.0f / (float)info->w, 1.0f, col, SzMsg_DrawRect_Pipe);
        }
        SzMsg_Free(1);
    }
    if (DOTR_DARE_SLIDE1 != 0) {
        short x;
        short y;
        signed char n;
        int no;
        short w;
        struct DotrDareInfo *info;

        w = DOTR_DARE_SLIDE1;
        x = 640 - (w * 640 >> 10);
        y = 256 - (w * 256 >> 10);
        n = DOTR_DARE_SIDE1;
        cursor = Taku_Obj_Set(cursor, &tex_bara_data2[n], x + 144, 304);
        cursor = Taku_Window_Set(cursor, &camp_window_Teki_data[0], n, 0, x + 144, 304, 0, 128, 128);
        xx = -480;
        yy = 320;
        no = DOTR_DARE_NAME1 - 195;
        switch (DOTR_DARE_STATE1) {
        case 1:
            DOTR_DARE_FACE1->id = 255;
            /* fall through */
        case 2:
            DOTR_DARE_FACE1->no = no;
            DOTR_DARE_FACE1->kind = data_002999cc[no][DOTR_DARE_LOOK1];
            DOTR_DARE_FACE1->mode = 1;
            DOTR_DARE_FACE1->x = camp_window_Teki_data[3].x + xx;
            DOTR_DARE_FACE1->y = y + (camp_window_Teki_data[3].y + yy);
            ShFace_Request(DOTR_DARE_FACE1);
            DOTR_DARE_STATE1 = 2;
            break;
        case 10:
            DOTR_DARE_FACE1->no = no;
            DOTR_DARE_FACE1->kind = data_002999cc[no][DOTR_DARE_LOOK1];
            DOTR_DARE_FACE1->mode = 3;
            DOTR_DARE_FACE1->x = camp_window_Teki_data[3].x + xx;
            DOTR_DARE_FACE1->y = y + (camp_window_Teki_data[3].y + yy);
            ShFace_Request(DOTR_DARE_FACE1);
            DOTR_DARE_STATE1 = 2;
            break;
        case 100:
            ShFace_Close(DOTR_DARE_FACE1);
            DOTR_DARE_STATE1 = 0;
            break;
        }
        cursor = Taku_Window_Set(cursor, &camp_window_Teki_data[3], n, 0, xx, (unsigned short)(y + yy), 1, 128, 0);
        cursor = Taku_FT4_Set(cursor, &camp_window_teki3[4], xx, y + 184, 2);
        cursor = Taku_FT4_Set(cursor, &camp_window_teki3[5], xx, y + 184, 2);
        cursor = Taku_FT4_Set(cursor, &camp_window_teki3[n], xx, y + 176, 0);
        cursor = SzMsg_LoadFont_Pipe(cursor, 1, DOTR_DARE_NAME1, 0, 22);
        cursor = RhText_SetTextureFlush(cursor);
        info = SzMsg_GetInfo(1);
        if (info->w < 324) {
            cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, x + 197, 325, 1.0f, 1.0f, col, SzMsg_DrawRect_Pipe);
        } else {
            cursor = SzMsg_DrawRatioX_Pipe(cursor, 1, x + 197, 325, 324.0f / (float)info->w, 1.0f, col, SzMsg_DrawRect_Pipe);
        }
        SzMsg_Free(1);
    }
    return cursor;
}

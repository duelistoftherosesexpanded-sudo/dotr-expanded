#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawBuildDeck_Password_Pipe(DotrCaptionWord cursor, int x, int y)
{
    char *text = data_002bf000_caption + 2;
    struct DotrCaptionMsgInfo *info;
    int px;
    int py;

    cursor = func_001b0ce0_caption(cursor, 2, -1, 148, x + 320, y + 142, 346, 164, func_0019bef0_caption(text), text, 0L);
    cursor = func_001b6f70_choice(cursor, x + 320, y + 236, 2, 2, 8, data_00317490_caption.pass, 0);
    px = x + 230 + data_00317490_caption.choice * 22;
    py = y + 231;
    cursor = func_001b4780_choice(cursor, -1, -1, 7, px, py, -26, -28, 1L);
    cursor = func_001fd680_caption(cursor, 1, 170, 0, 22);
    cursor = func_001fd680_caption(cursor, 2, 173, 0, 44);
    cursor = func_0020f290_caption(cursor);
    info = func_001feab0_caption(1);
    cursor = func_001fe7d0_caption(cursor, 1, x + 234 - (info->width >> 1), y + 272, 0, func_001fdfe0_caption);
    px = x + 1914 - (info->width >> 1);
    py = y + 2096;
    cursor = func_001b3cc0_caption(cursor, gSzDuel_Icon_caption + 2490, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    px = x + 1938 - (info->width >> 1);
    cursor = func_001b3cc0_caption(cursor, gSzDuel_Icon_caption + 2480, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    info = func_001feab0_caption(2);
    px = x + 406 - (info->width >> 1);
    py = y + 272;
    cursor = func_001fe7d0_caption(cursor, 2, px, py, 0, func_001fdfe0_caption);
    px = x + 2110 - (info->width >> 1);
    py = y + 2096;
    return func_001b3cc0_caption(cursor, gSzDuel_Icon_caption + 2500, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
}

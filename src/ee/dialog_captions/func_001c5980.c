#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawTrade_Exit_Pipe(DotrCaptionWord cursor, int x, int y)
{
    /* MATCHING DEVICE: the control block pointer declared first. */
    struct DotrCaptionControl *g = &data_00317490_caption;
    char *text = data_002bef96_caption + 2;
    char *text2 = data_002bef6d_caption + 2;
    char *text3 = data_002bef71_caption + 2;
    struct DotrCaptionMsgInfo *info;

    cursor = func_001b0ce0_caption(cursor, 2, -1, 126, x + 320, y + 131, 320, 186, func_0019bef0_caption(text), text, 0L);
    cursor = func_001fd680_caption(cursor, 1, 127, 0, 22);
    cursor = func_0020f290_caption(cursor);
    info = func_001feab0_caption(1);
    cursor = func_001fe7d0_caption(cursor, 1, x + 320 - (info->width >> 1), y + 281 - (info->height + 12), 0, func_001fdfe0_caption);
    func_001feaf0_caption(1);
    cursor = func_001b6f70_choice(cursor, x + 240, y + 281, 2, 3, func_0019bef0(text2), text2, g->choice ? 0 : data_002be944_caption);
    return func_001b6f70_choice(cursor, x + 400, y + 281, 2, 3, func_0019bef0(text3), text3, g->choice ? data_002be944_caption : 0);
}

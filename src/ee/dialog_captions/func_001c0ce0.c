#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordSuccess_Pipe(DotrCaptionWord cursor, int x, int y)
{
    char *text = data_002bf000_caption + 2;
    /* MATCHING DEVICE: the address of the id taken first, the name its own declaration. */
    unsigned short *id = &data_00317b60_caption->id;
    int name = func_001de210_caption(*id);
    float scale = 1.0f;
    struct DotrCaptionMsgInfo *info;

    cursor = func_001b0ce0_caption(cursor, 2, -1, -1, x + 320, y + 65, 460, 318, func_0019bef0_caption(text), text, 1);
    cursor = func_001b7d60_caption(cursor, *id, x + 256, y + 181, 128, 128, data_003180e8);
    cursor = func_001fd680_caption(cursor, 1, 149, 0, 22);
    cursor = func_001fd680_caption(cursor, 2, name, 0, 66);
    cursor = func_0020f290_caption(cursor);
    info = func_001feab0_caption(1);
    cursor = func_001fe7d0_caption(cursor, 1, x + 320 - (info->width >> 1), y + 125, 0, func_001fdfe0_caption);
    info = func_001feab0_caption(2);
    if (info->width > 436) {
        scale = 436.0f / (float)info->width;
    }
    cursor = func_001fe660_caption(cursor, 2, x + 320 - func_001a22c0((float)info->width * scale / 2.0f), y + 321, scale, 1.0f, 0,
                                   func_001fdfe0_caption);
    func_001feaf0_caption(1);
    func_001feaf0_caption(2);
    return cursor;
}

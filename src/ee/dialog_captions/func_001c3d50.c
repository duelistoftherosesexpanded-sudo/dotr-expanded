#include "dotr/ee/dialog_captions.h"

DotrCaptionTitleWord lSzDuel_DrawTrade_Title_Pipe(DotrCaptionTitleWord cursor, int x, int y)
{
    char *text = data_002bf009_caption + 2;
    int px = x + 224;
    int py = y + 32;
    cursor = func_001b62f0_caption(cursor, px, py, 6, 1);
    cursor = func_001b4780_caption(cursor, -1, 115, 4, px, py, 21, 2, 1);
    return func_001b6f70_caption(cursor, x + 320, y + 39, 2, 3, func_0019bef0(text), text, 0);
}

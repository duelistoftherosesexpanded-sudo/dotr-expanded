#include "dotr/ee/dialog_captions.h"

DotrCaptionTitleWord lSzDuel_DrawBuildDeck_Title_Pipe(DotrCaptionTitleWord cursor, int x, int y)
{
    int side;
    int deck;
    char *text;
    int px;
    int py;
    int qx;
    int qy;

    side = data_00317490_caption.side;
    deck = SzData_GetCurrentDeck(side);
    text = data_002bef80_caption + 2;
    px = x + 224;
    py = y + 32;

    cursor = func_001b62f0_caption(cursor, px, py, 6, 1);
    cursor = func_001b4780_caption(cursor, -1, 115, 4, px, py, 21, 2, 1);
    px = x + 320;
    py = y + 39;
    cursor = func_001b6f70_caption(cursor, px, py, 2, 3, func_0019bef0(text), text, 0);
    px = x + 432;
    py = y + 16;
    cursor = func_001b4780_caption(cursor, 12, -1, 11, px, py, -128, -48, 1);
    px = x + 2202;
    py = y + 1848;
    cursor = func_001b3cc0_title(cursor, gSzDuel_Icon_caption + 2330, px, py, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    qx = x + 2172;
    qy = y + 1866;
    cursor = func_001b3cc0_title(cursor, gSzDuel_Icon_caption + 2160, qx, qy, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    qx = x + 464;
    qy = y + 44;
    cursor = func_001b6ea0_caption(cursor, qx, qy, 1, 0, 4, SzData_GetDeckCost(side, deck));
    qx = x + 2236;
    qy = y + 1866;
    cursor = func_001b3cc0_title(cursor, gSzDuel_Icon_caption + 2050, qx, qy, 0, 1, 0, 0, 0, 0, 1, 0, -1, 128);
    qx = x + 528;
    qy = y + 44;
    return func_001b6ea0_caption(cursor, qx, qy, 1, 0, 2, SzData_GetDeckCount(side, deck));
}

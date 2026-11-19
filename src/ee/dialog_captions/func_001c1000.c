#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawBuildDeck_Caution_Pipe(DotrCaptionWord cursor, int x, int y)
{
    char *text = data_002bef65_caption + 2;
    return func_001b0ce0_caption(cursor, 2, -1, 124, x + 320, y + 163, 320, 122, func_0019bef0_caption(text), text, 1);
}

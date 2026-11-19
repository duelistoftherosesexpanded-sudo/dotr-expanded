#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawBuildDeck_PasswordDone_Pipe(DotrCaptionWord cursor, int x, int y)
{
    char *text = data_002bf000_caption + 2;
    return func_001b0ce0_caption(cursor, 2, -1, 151, x + 320, y + 141, 460, 166, func_0019bef0_caption(text), text, 1);
}

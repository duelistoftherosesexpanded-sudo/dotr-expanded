#include "dotr/ee/dialog_captions.h"

DotrCaptionWord lSzDuel_DrawCustomDuel_Exit_Pipe(DotrCaptionWord cursor, int x, int y)
{
    char *text = data_002bef96_caption + 2;
    char *text2 = data_002bef6d_caption + 2;
    char *text3 = data_002bef71_caption + 2;
    cursor = func_001b0ce0_caption(cursor, 2, -1, 145, x + 320, y + 148, 320, 152, func_0019bef0_caption(text), text, 0L);
    cursor = func_001b6f70_choice(cursor, x + 240, y + 264, 2, 3, func_0019bef0(text2), text2, data_00a9c08e_caption ? 0 : data_002be944_caption);
    return func_001b6f70_choice(cursor, x + 400, y + 264, 2, 3, func_0019bef0(text3), text3, data_00a9c08e_caption ? data_002be944_caption : 0);
}
